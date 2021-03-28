#include <iostream>
#include <set>
#include <vector>

#include <vca/filesystem.h>
#include <vca/utils.h>

#include <efsw/efsw.hpp>

#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>

struct FileContents
{
    std::vector<std::string> words;
};

class UserDb
{
public:
    virtual ~UserDb() = default;

    virtual const fs::path&
    path() const = 0;

    virtual void
    truncate() = 0;

    virtual void
    update_file(const fs::path& path, const FileContents& contents) = 0;

    virtual void
    remove_file(const fs::path& path) = 0;

    virtual std::vector<fs::path>
    search(const FileContents& contents) = 0;
};

class SqliteUserDb : public UserDb
{
public:
    explicit SqliteUserDb(fs::path path)
        : m_path{std::move(path)}
        , m_db{m_path.u8string(), SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE}
    {
        m_db.exec("PRAGMA foreign_keys = ON");
        SQLite::Transaction transaction{m_db};
        m_db.exec("CREATE TABLE IF NOT EXISTS files (id INTEGER PRIMARY KEY "
                  "AUTOINCREMENT, path TEXT NOT NULL UNIQUE, ext TEXT)");
        m_db.exec("CREATE TABLE IF NOT EXISTS words (id INTEGER PRIMARY KEY "
                  "AUTOINCREMENT, files_id INTEGER NOT NULL, word TEXT NOT "
                  "NULL, FOREIGN KEY (files_id) REFERENCES files (id) ON "
                  "DELETE CASCADE)");
        transaction.commit();
    }

    void
    truncate() override
    {
        SQLite::Transaction transaction{m_db};
        m_db.exec("DELETE FROM files");
        transaction.commit();
    }

    void
    update_file(const fs::path& path, const FileContents& contents) override
    {
        SQLite::Transaction transaction{m_db};
        SQLite::Statement del_stm{m_db, "DELETE FROM files WHERE path = ?"};
        SQLite::bind(del_stm, path.u8string());
        del_stm.exec();
        SQLite::Statement ins_stm{
            m_db, "INSERT INTO files (path, ext) VALUES (?, ?)"};
        SQLite::bind(ins_stm, path.u8string(), path.extension().u8string());
        ins_stm.exec();
        SQLite::Statement query_stm{m_db,
                                    "SELECT last_insert_rowid() FROM files"};
        int files_id = -1;
        if (query_stm.executeStep())
        {
            files_id = query_stm.getColumn(0).getInt();
        }
        VCA_CHECK(files_id > -1);
        for (const auto& word : contents.words)
        {
            SQLite::Statement ins_word_stm{
                m_db, "INSERT INTO words (files_id, word) VALUES (?, ?)"};
            SQLite::bind(ins_word_stm, files_id, word);
            ins_word_stm.exec();
        }
        transaction.commit();
    }

    void
    remove_file(const fs::path& path) override
    {
        SQLite::Transaction transaction{m_db};
        SQLite::Statement del_stm{m_db, "DELETE FROM files WHERE path = ?"};
        SQLite::bind(del_stm, path.u8string());
        del_stm.exec();
        transaction.commit();
    }

    std::vector<fs::path>
    search(const FileContents& contents) override
    {
        SQLite::Transaction transaction{m_db};
        std::set<int> files_ids;
        for (const auto& word : contents.words)
        {
            SQLite::Statement query_stm{
                m_db, "SELECT files_id FROM words WHERE word LIKE ?"};
            SQLite::bind(query_stm, word);
            while (query_stm.executeStep())
            {
                const int files_id = query_stm.getColumn(0).getInt();
                files_ids.insert(files_id);
            }
        }
        std::vector<fs::path> paths;
        for (const auto files_id : files_ids)
        {
            SQLite::Statement query_stm{m_db,
                                        "SELECT path FROM files WHERE id = ?"};
            SQLite::bind(query_stm, files_id);
            if (query_stm.executeStep())
            {
                const auto path = query_stm.getColumn(0).getText();
                paths.emplace_back(fs::u8path(path));
            }
        }
        transaction.commit();
        return paths;
    }

    const fs::path&
    path() const override
    {
        return m_path;
    }

private:
    fs::path m_path;
    SQLite::Database m_db;
};

int
main(const int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./vca_daemon <work_dir>" << std::endl;
    }
    const auto work_dir = fs::u8path(argv[1]);

    SqliteUserDb user_db{work_dir / "user.db"};

    user_db.update_file("/foo/bar.txt", {{"guan", "christian"}});
    user_db.update_file("/foo/assi.doc", {{"dani", "peter", "guan"}});
    user_db.update_file("/foo/spinner.pdf", {{"basti", "peter", "dani"}});

    const auto paths = user_db.search({{"guan", "christian"}});

    for (const auto& p : paths)
    {
        std::cout << p << std::endl;
    }

    user_db.truncate();

    const auto paths2 = user_db.search({{"guan", "christian"}});

    for (const auto& p : paths2)
    {
        std::cout << p << std::endl;
    }

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
