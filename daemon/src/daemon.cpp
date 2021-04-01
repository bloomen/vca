#include <fstream>
#include <iostream>
#include <set>
#include <thread>
#include <vector>

#include <vca/config.h>
#include <vca/filesystem.h>
#include <vca/logging.h>
#include <vca/sqlite_userdb.h>
#include <vca/utils.h>

#include "default_tokenizer.h"
#include "file_processor.h"
#include "file_scanner.h"
#include "file_watcher.h"

int
main(const int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./vca_daemon <work_dir>" << std::endl;
        return EXIT_FAILURE;
    }

    try
    {
        const auto work_dir = fs::u8path(argv[1]);
        fs::create_directories(work_dir);

        vca::init_logging(work_dir / "logs" / "vca_daemon.log");
        vca::set_log_level(vca::Logger::Level::Debug);

        VCA_INFO << "Starting vca_daemon";
        const auto daemon_file = work_dir / "daemon";
        std::ofstream{daemon_file};

        vca::AppConfig app_config;

        vca::UserConfig user_config{work_dir / "user.json"};
        VCA_INFO << "User root dir: " << user_config.root_dir();

        vca::SqliteUserDb user_db{work_dir / "user.db",
                                  vca::UserDb::OpenType::ReadWrite};
        user_db.create();

        vca::FileProcessor file_processor;
        file_processor.add_tokenizer(std::make_unique<vca::DefaultTokenizer>());

        vca::FileWatcher file_watcher{
            app_config, user_config, user_db, file_processor};

        vca::FileScanner file_scanner{
            app_config, user_config, user_db, file_processor};

        while (fs::exists(daemon_file))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }

        VCA_INFO << "Terminating daemon";
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
