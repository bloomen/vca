#include <atomic>
#include <csignal>
#include <fstream>
#include <iostream>
#include <set>
#include <thread>
#include <vector>

#include <vca/command_queue.h>
#include <vca/config.h>
#include <vca/file_lock.h>
#include <vca/filesystem.h>
#include <vca/logging.h>
#include <vca/sqlite_userdb.h>
#include <vca/time.h>
#include <vca/utils.h>

#include "file_processor.h"
#include "file_scanner.h"
#include "file_watcher.h"
#include "http_server.h"
#include "pdf_tokenizer.h"
#include "txt_tokenizer.h"
#include "xml_tokenizer.h"
#include "zipxml_tokenizer.h"

std::atomic<int> g_signal_status{0};

void
signal_handler(const int signal)
{
    g_signal_status = signal;
}

int
main(const int, char**)
{
    try
    {
        std::signal(SIGINT, signal_handler);
        std::signal(SIGTERM, signal_handler);

        const auto work_dir = vca::user_config_dir() / "findle";
        fs::create_directories(work_dir);

        vca::init_logging(work_dir / "logs" / "findled.log");
        vca::set_log_level(vca::Logger::Level::Debug);

        VCA_INFO << "Starting findled";
        VCA_INFO << "work_dir: " << work_dir;

        vca::CommandQueue commands;

        vca::AppConfig app_config;

        vca::UserConfig user_config{commands, work_dir / "user.json"};

        vca::SqliteUserDb user_db{work_dir / "user.db",
                                  vca::UserDb::OpenType::ReadWrite};
        user_db.create(user_config.root_dirs());

        vca::FileProcessor file_processor{app_config};
        file_processor.set_default_tokenizer(
            std::make_unique<vca::TxtTokenizer>());
        file_processor.add_tokenizer(U".xml",
                                     std::make_unique<vca::TxtTokenizer>(true));
        file_processor.add_tokenizer(U".svg",
                                     std::make_unique<vca::XmlTokenizer>());
        file_processor.add_tokenizer(
            U".docx",
            std::make_unique<vca::ZipxmlTokenizer>("word/document.xml"));
        file_processor.add_tokenizer(
            U".xlsx",
            std::make_unique<vca::ZipxmlTokenizer>("xl/sharedStrings.xml"));
        file_processor.add_tokenizer(U".pdf",
                                     std::make_unique<vca::PdfTokenizer>());

        vca::FileWatcher file_watcher{
            commands, user_config, user_db, file_processor};

        vca::FileScanner file_scanner{
            commands, user_config, user_db, file_processor};

        vca::HttpServer http_server{commands, app_config, user_config, user_db};

        const auto app_config_path = work_dir / "app.json";
        {
            vca::FileGuard app_config_lock{app_config_path};
            app_config.write(app_config_path);
        }

        while (g_signal_status == 0)
        {
            vca::Timer timer;
            commands.sync(g_signal_status);
            const auto interval = timer.us();
            constexpr size_t frame_min_us = 16666;
            if (interval < frame_min_us)
            {
                std::this_thread::sleep_for(
                    std::chrono::microseconds{frame_min_us - interval});
            }
        }

        VCA_INFO << "Received signal: " << g_signal_status;
        VCA_INFO << "Terminating findled";
        vca::FileGuard app_config_lock{app_config_path};
        fs::remove(app_config_path);
        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        VCA_EXCEPTION(e) << e.what();
        return EXIT_FAILURE;
    }
    catch (...)
    {
        VCA_ERROR << "Unknown exception";
        return EXIT_FAILURE;
    }
}
