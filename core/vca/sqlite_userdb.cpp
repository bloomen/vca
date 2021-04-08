#include "sqlite_userdb.h"

#include <mutex>
#include <set>

#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>

#include "file_lock.h"
#include "logging.h"
#include "utils.h"

namespace vca
{

namespace
{

int
toSQLiteOpenType(const UserDb::OpenType open_type)
{
    switch (open_type)
    {
    case UserDb::OpenType::ReadOnly:
        return SQLite::OPEN_READONLY;
    case UserDb::OpenType::ReadWrite:
        return SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE;
    }
    return SQLite::OPEN_READONLY;
}

} // namespace

struct SqliteUserDb::Impl
{
    explicit Impl(fs::path path, const UserDb::OpenType open_type)
        : path{make_path(std::move(path))}
        , db{this->path.u8string(), toSQLiteOpenType(open_type)}
        , file_lock{this->path.parent_path()}
    {
        VCA_CHECK(db.getHandle());
    }

    static fs::path
    make_path(fs::path path)
    {
        fs::create_directories(path.parent_path());
        return path;
    }

    fs::path path;
    SQLite::Database db;
    FileLock file_lock;
    fs::path root_dir;
};

SqliteUserDb::SqliteUserDb(fs::path path, const OpenType open_type)
    : m_impl{std::make_unique<Impl>(std::move(path), open_type)}
{
    m_impl->db.exec("PRAGMA foreign_keys = ON");
    m_impl->db.exec("PRAGMA synchronous = OFF");
    m_impl->db.exec("PRAGMA cache_size = 100000");
}

SqliteUserDb::~SqliteUserDb() = default;

const fs::path&
SqliteUserDb::path() const
{
    return m_impl->path;
}

void
SqliteUserDb::create(const fs::path& root_dir)
{
    if (m_impl->root_dir != root_dir)
    {
        VCA_INFO << "Create user db for: " << root_dir;
        m_impl->root_dir = root_dir;
        std::lock_guard<FileLock> file_lock{m_impl->file_lock};
        SQLite::Transaction transaction{m_impl->db};
        m_impl->db.exec("DROP TABLE IF EXISTS files");
        m_impl->db.exec("CREATE TABLE files (id INTEGER PRIMARY KEY "
                        "AUTOINCREMENT, path TEXT NOT NULL UNIQUE, ext TEXT)");
        m_impl->db.exec("DROP TABLE IF EXISTS words");
        m_impl->db.exec(
            "CREATE TABLE words (id INTEGER PRIMARY KEY "
            "AUTOINCREMENT, files_id INTEGER NOT NULL, word TEXT NOT "
            "NULL, FOREIGN KEY (files_id) REFERENCES files (id) ON "
            "DELETE CASCADE)");
        transaction.commit();
    }
}

void
SqliteUserDb::update_file(const fs::path& path, const FileContents& contents)
{
    VCA_DEBUG << __func__ << ": " << path;
    std::lock_guard<FileLock> file_lock{m_impl->file_lock};
    SQLite::Transaction transaction{m_impl->db};
    SQLite::Statement del_stm{m_impl->db, "DELETE FROM files WHERE path = ?"};
    SQLite::bind(del_stm, path.u8string());
    del_stm.exec();
    SQLite::Statement ins_stm{m_impl->db,
                              "INSERT INTO files (path, ext) VALUES (?, ?)"};
    SQLite::bind(ins_stm, path.u8string(), path.extension().u8string());
    ins_stm.exec();
    SQLite::Statement query_stm{m_impl->db,
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
            m_impl->db, "INSERT INTO words (files_id, word) VALUES (?, ?)"};
        SQLite::bind(ins_word_stm, files_id, word);
        ins_word_stm.exec();
    }
    transaction.commit();
}

void
SqliteUserDb::remove_file(const fs::path& path)
{
    VCA_DEBUG << __func__ << ": " << path;
    std::lock_guard<FileLock> file_lock{m_impl->file_lock};
    SQLite::Transaction transaction{m_impl->db};
    SQLite::Statement del_stm{m_impl->db, "DELETE FROM files WHERE path = ?"};
    SQLite::bind(del_stm, path.u8string());
    del_stm.exec();
    transaction.commit();
}

void
SqliteUserDb::move_file(const fs::path& old_path, const fs::path& path)
{
    VCA_DEBUG << __func__ << ": " << old_path << " - " << path;
    std::lock_guard<FileLock> file_lock{m_impl->file_lock};
    SQLite::Transaction transaction{m_impl->db};
    SQLite::Statement up_stm{m_impl->db,
                             "UPDATE files SET path = ? WHERE path = ?"};
    SQLite::bind(up_stm, path.u8string(), old_path.u8string());
    up_stm.exec();
    transaction.commit();
}

std::vector<fs::path>
SqliteUserDb::search(const FileContents& contents)
{
    std::lock_guard<FileLock> file_lock{m_impl->file_lock};
    std::set<int> files_ids;
    for (const auto& word : contents.words)
    {
        if (word.empty())
        {
            continue;
        }
        SQLite::Statement query_stm{
            m_impl->db, "SELECT files_id FROM words WHERE word LIKE ?"};
        SQLite::bind(query_stm, "%" + word + "%");
        while (query_stm.executeStep())
        {
            const int files_id = query_stm.getColumn(0).getInt();
            files_ids.insert(files_id);
        }
    }
    std::vector<fs::path> paths;
    for (const auto files_id : files_ids)
    {
        SQLite::Statement query_stm{m_impl->db,
                                    "SELECT path FROM files WHERE id = ?"};
        SQLite::bind(query_stm, files_id);
        if (query_stm.executeStep())
        {
            const auto path = query_stm.getColumn(0).getText();
            paths.emplace_back(fs::u8path(path));
        }
    }
    return paths;
}

} // namespace vca
