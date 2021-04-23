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

    void
    add_root_dir(const fs::path& dir)
    {
        SQLite::Statement ins_dir_stm{
            db, "INSERT INTO roots (id, dir) VALUES (?, ?)"};
        SQLite::bind(ins_dir_stm, roots_id, dir.u8string());
        ins_dir_stm.exec();
        root_dirs.emplace(dir, roots_id);
        ++roots_id;
    }

    void
    remove_root_dir(const fs::path& dir)
    {
        SQLite::Statement del_dir_stm{db, "DELETE FROM roots WHERE dir = ?"};
        SQLite::bind(del_dir_stm, dir.u8string());
        del_dir_stm.exec();
        root_dirs.erase(dir);
    }

    std::pair<fs::path, int>
    relative(const fs::path& path) const
    {
        for (const auto& [dir, id] : root_dirs)
        {
            if (is_parent_of(dir, path))
            {
                auto p = fs::relative(path, dir);
                return std::make_pair(std::move(p), id);
            }
        }
        VCA_CHECK(false) << "No root_dir found for: " << path;
        return {};
    }

    int files_id = 0;
    fs::path path;
    SQLite::Database db;
    FileLock file_lock;
    int roots_id = 0;
    std::map<fs::path, int> root_dirs;
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
SqliteUserDb::create(const std::set<fs::path>& root_dirs)
{
    VCA_INFO << "Create user db";
    std::lock_guard<FileLock> file_lock{m_impl->file_lock};
    SQLite::Transaction transaction{m_impl->db};

    m_impl->db.exec("CREATE TABLE IF NOT EXISTS roots (id INTEGER PRIMARY KEY, "
                    "dir TEXT NOT NULL UNIQUE)");

    m_impl->db.exec("CREATE TABLE IF NOT EXISTS files (id INTEGER PRIMARY KEY, "
                    "roots_id INTEGER NOT NULL, "
                    "path TEXT NOT NULL, FOREIGN KEY (roots_id) REFERENCES "
                    "roots (id) ON DELETE CASCADE)");

    m_impl->db.exec("CREATE TABLE IF NOT EXISTS words (files_id INTEGER "
                    "NOT NULL, word TEXT NOT "
                    "NULL, FOREIGN KEY (files_id) REFERENCES files (id) ON "
                    "DELETE CASCADE)");

    m_impl->db.exec("DELETE FROM roots");
    m_impl->root_dirs.clear();

    for (const auto& dir : root_dirs)
    {
        m_impl->add_root_dir(dir);
    }

    transaction.commit();
}

void
SqliteUserDb::add_root_dir(const fs::path& root_dir)
{
    if (m_impl->root_dirs.count(root_dir) > 0)
    {
        return;
    }
    VCA_INFO << __func__ << ": " << root_dir;
    std::lock_guard<FileLock> file_lock{m_impl->file_lock};
    SQLite::Transaction transaction{m_impl->db};
    m_impl->add_root_dir(root_dir);
    transaction.commit();
}

void
SqliteUserDb::remove_root_dir(const fs::path& root_dir)
{
    if (m_impl->root_dirs.count(root_dir) == 0)
    {
        return;
    }
    VCA_INFO << __func__ << ": " << root_dir;
    std::lock_guard<FileLock> file_lock{m_impl->file_lock};
    SQLite::Transaction transaction{m_impl->db};
    m_impl->remove_root_dir(root_dir);
    transaction.commit();
}

void
SqliteUserDb::update_file(const fs::path& path, const FileContents& contents)
{
    VCA_DEBUG << __func__ << ": " << path;
    const auto [p, roots_id] = m_impl->relative(path);
    std::lock_guard<FileLock> file_lock{m_impl->file_lock};
    SQLite::Transaction transaction{m_impl->db};

    SQLite::Statement del_stm{
        m_impl->db, "DELETE FROM files WHERE path = ? AND roots_id = ?"};
    SQLite::bind(del_stm, p.u8string(), roots_id);
    del_stm.exec();

    SQLite::Statement ins_stm{
        m_impl->db, "INSERT INTO files (id, roots_id, path) VALUES (?, ?, ?)"};
    SQLite::bind(ins_stm, m_impl->files_id, roots_id, p.u8string());
    ins_stm.exec();

    for (const auto& word : contents.words)
    {
        SQLite::Statement ins_word_stm{
            m_impl->db, "INSERT INTO words (files_id, word) VALUES (?, ?)"};
        SQLite::bind(ins_word_stm, m_impl->files_id, word);
        ins_word_stm.exec();
    }

    ++m_impl->files_id;
    transaction.commit();
}

void
SqliteUserDb::remove_file(const fs::path& path)
{
    VCA_DEBUG << __func__ << ": " << path;
    const auto [p, roots_id] = m_impl->relative(path);
    std::lock_guard<FileLock> file_lock{m_impl->file_lock};
    SQLite::Transaction transaction{m_impl->db};
    SQLite::Statement del_stm{m_impl->db, "DELETE FROM files WHERE path = ?"};
    SQLite::bind(del_stm, p.u8string());
    del_stm.exec();
    transaction.commit();
}

void
SqliteUserDb::move_file(const fs::path& old_path, const fs::path& path)
{
    VCA_DEBUG << __func__ << ": " << old_path << " - " << path;
    const auto [old_p, old_roots_id] = m_impl->relative(old_path);
    const auto [p, roots_id] = m_impl->relative(path);
    std::lock_guard<FileLock> file_lock{m_impl->file_lock};
    SQLite::Transaction transaction{m_impl->db};

    SQLite::Statement up_stm{
        m_impl->db, "UPDATE files SET roots_id = ?, path = ? WHERE path = ?"};
    SQLite::bind(up_stm, roots_id, p.u8string(), old_p.u8string());
    up_stm.exec();

    transaction.commit();
}

std::vector<fs::path>
SqliteUserDb::search(const FileContents& contents) const
{
    std::lock_guard<FileLock> file_lock{m_impl->file_lock};

    std::map<fs::path, size_t> path_map;
    for (const auto& word : contents.words)
    {
        VCA_DEBUG << __func__ << ": " << word;
        SQLite::Statement query_stm{
            m_impl->db,
            "SELECT dir, path FROM files JOIN roots ON roots.id = "
            "files.roots_id JOIN words ON files.id = words.files_id "
            "WHERE words.word LIKE ?"};
        SQLite::bind(query_stm, "%" + word + "%");
        while (query_stm.executeStep())
        {
            const auto root_dir = fs::u8path(query_stm.getColumn(0).getText());
            const auto path = fs::u8path(query_stm.getColumn(1).getText());
            path_map[root_dir / path]++;
        }
    }

    std::vector<fs::path> paths;
    for (const auto& pair : path_map)
    {
        if (pair.second == contents.words.size())
        {
            paths.emplace_back(pair.first);
        }
    }

    return paths;
}

} // namespace vca
