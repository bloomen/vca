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

        vca::AppConfig app_config;

        gcl::Async async{std::thread::hardware_concurrency()};

        vca::UserConfig user_config{work_dir / "user.json"};
        VCA_INFO << "User root dir: " << user_config.root_dir();

        vca::SqliteUserDb user_db{work_dir / "user.db"};

        vca::FileWatcher file_watcher{app_config, user_config, user_db};

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
        VCA_ERROR << "Exception: " << e.what();
        return EXIT_FAILURE;
    }
    catch (...)
    {
        VCA_ERROR << "Unknown exception";
        return EXIT_FAILURE;
    }

    //    user_db.update_file("/foo/bar.txt", {{"guan", "christian"}});
    //    user_db.update_file("/foo/assi.doc", {{"dani", "peter", "guan"}});
    //    user_db.update_file("/foo/spinner.pdf", {{"basti", "peter", "dani"}});

    //    const auto paths = user_db.search({{"guan", "christian"}});

    //    for (const auto& p : paths)
    //    {
    //        std::cout << p << std::endl;
    //    }

    //    user_db.truncate();

    //    const auto paths2 = user_db.search({{"guan", "christian"}});

    //    for (const auto& p : paths2)
    //    {
    //        std::cout << p << std::endl;
    //    }

    //    // Inherits from the abstract listener class, and implements the the file
    //    // action handler
    //    class UpdateListener : public efsw::FileWatchListener
    //    {
    //    public:
    //        UpdateListener()
    //        {
    //        }

    //        void
    //        handleFileAction(efsw::WatchID watchid,
    //                         const std::string& dir,
    //                         const std::string& filename,
    //                         efsw::Action action,
    //                         std::string oldFilename = "")
    //        {
    //            switch (action)
    //            {
    //            case efsw::Actions::Add:
    //                std::cout << "DIR (" << dir << ") FILE (" << filename
    //                          << ") has event Added" << std::endl;
    //                break;
    //            case efsw::Actions::Delete:
    //                std::cout << "DIR (" << dir << ") FILE (" << filename
    //                          << ") has event Delete" << std::endl;
    //                break;
    //            case efsw::Actions::Modified:
    //                std::cout << "DIR (" << dir << ") FILE (" << filename
    //                          << ") has event Modified" << std::endl;
    //                break;
    //            case efsw::Actions::Moved:
    //                std::cout << "DIR (" << dir << ") FILE (" << filename
    //                          << ") has event Moved from (" << oldFilename <<
    //                          ")"
    //                          << std::endl;
    //                break;
    //            default:
    //                std::cout << "Should never happen!" << std::endl;
    //            }
    //        }
    //    };

    //    // Create the file system watcher instance
    //    // efsw::FileWatcher allow a first boolean parameter that indicates if it
    //    // should start with the generic file watcher instead of the platform
    //    // specific backend
    //    efsw::FileWatcher* fileWatcher = new efsw::FileWatcher();

    //    // Create the instance of your efsw::FileWatcherListener implementation
    //    UpdateListener* listener = new UpdateListener();

    //    // Add a folder to watch, and get the efsw::WatchID
    //    // It will watch the /tmp folder recursively ( the third parameter indicates that is recursive )
    //    // Reporting the files and directories changes to the instance of the
    //    // listener
    //    efsw::WatchID watchID = fileWatcher->addWatch("/tmp", listener, true);

    //    // Adds another directory to watch. This time as non-recursive.
    //    efsw::WatchID watchID2 = fileWatcher->addWatch("/usr", listener, false);

    //    // Start watching asynchronously the directories
    //    fileWatcher->watch();

    //    while (true)
    //        ;

    //    try
    //    {
    //        // Open a database file
    //        SQLite::Database db("example.db3");

    //        // Compile a SQL query, containing one parameter (index 1)
    //        SQLite::Statement query(db, "SELECT * FROM test WHERE size > ?");

    //        // Bind the integer value 6 to the first parameter of the SQL query
    //        query.bind(1, 6);

    //        // Loop to execute the query step by step, to get rows of result
    //        while (query.executeStep())
    //        {
    //            // Demonstrate how to get some typed column value
    //            int id = query.getColumn(0);
    //            const char* value = query.getColumn(1);
    //            int size = query.getColumn(2);

    //            std::cout << "row: " << id << ", " << value << ", " << size
    //                      << std::endl;
    //        }
    //    }
    //    catch (std::exception& e)
    //    {
    //        std::cout << "exception: " << e.what() << std::endl;
    //    }
}
