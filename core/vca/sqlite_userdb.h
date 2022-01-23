#pragma once

#include <memory>

#include "userdb.h"
#include "utils.h"

namespace vca
{

class SqliteUserDb : public UserDb
{
public:
    explicit SqliteUserDb(fs::path path, OpenType open_type);

    VCA_DELETE_COPY(SqliteUserDb)
    VCA_DEFAULT_MOVE(SqliteUserDb)

    ~SqliteUserDb();

    const fs::path&
    path() const override;

    void
    create(const std::set<fs::path>& root_dir) override;

    void
    add_root_dir(const fs::path& root_dir) override;

    void
    remove_root_dir(const fs::path& root_dir) override;

    void
    update_file(const fs::path& path, const FileContents& contents) override;

    void
    remove_file(const fs::path& path) override;

    void
    move_file(const fs::path& old_path, const fs::path& path) override;

    std::vector<SearchResult>
    search(const FileContents& contents) const override;

    std::chrono::time_point<std::chrono::system_clock>
    last_file_update() const override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace vca
