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
    create(const fs::path& root_dir) override;

    virtual std::string
    property(const std::string& key) const override;

    void
    update_file(const fs::path& path, const FileContents& contents) override;

    void
    remove_file(const fs::path& path) override;

    void
    move_file(const fs::path& old_path, const fs::path& path) override;

    std::vector<fs::path>
    search(const FileContents& contents) const override;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace vca
