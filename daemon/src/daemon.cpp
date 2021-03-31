#include <csignal>
#include <iostream>
#include <set>
#include <vector>

#include <gcl.h>

#include <vca/config.h>
#include <vca/filesystem.h>
#include <vca/logging.h>
#include <vca/sqlite_userdb.h>
#include <vca/utils.h>

#include "file_watcher.h"
#include "scan.h"

vca::FileWatcher* g_file_watcher = nullptr;

void
sigint_handler(int)
{
    VCA_INFO << "Caught SIGINT";
    if (g_file_watcher)
    {
        g_file_watcher->stop();
    }
    VCA_INFO << "Terminating daemon";
}

int
main(const int argc, char** argv)
{
    std::signal(SIGINT, sigint_handler);

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

        vca::AppConfig app_config;

        gcl::Async async{std::thread::hardware_concurrency()};

        vca::UserConfig user_config{work_dir / "user.json"};
        VCA_INFO << "User root dir: " << user_config.root_dir();

        vca::SqliteUserDb user_db{work_dir / "user.db",
                                  vca::UserDb::OpenType::ReadWrite};
        user_db.create();

        vca::FileWatcher file_watcher{app_config, user_config, user_db};
        g_file_watcher = &file_watcher;

        auto scan_task = gcl::task([&] {
            vca::scan(app_config, user_config, user_db);
            VCA_INFO << "Scanning finished";
        });

        auto file_watcher_task = gcl::task([&] { file_watcher.run(); });

        auto final_task = gcl::when(scan_task, file_watcher_task);
        final_task.schedule_all(async);
        final_task.wait();

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
