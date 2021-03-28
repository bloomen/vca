#include <iostream>

#include <efsw/efsw.hpp>

#include <SQLiteCpp/SQLiteCpp.h>

int
main(const int argc, char** argv)
{

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
