#pragma once

#include <set>
#include <string>

#include "file_lock.h"
#include "filesystem.h"

namespace vca
{

class UserConfig
{
public:
    explicit UserConfig(const fs::path& path);

    const fs::path&
    root_dir() const;

    // TODO: Implement config changing
    //    void
    //    add_changed_callback();

private:
    FileLock m_file_lock;
    fs::path m_root_dir;
};

class AppConfig
{
public:
    const std::set<std::string>&
    extensions() const;

    bool
    matches_ext(const fs::path& path) const;

private:
    std::set<std::string> m_extensions = {".txt"};
};

} // namespace vca
