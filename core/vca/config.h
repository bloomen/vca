#pragma once

#include <set>
#include <string>

#include "filesystem.h"

namespace vca
{

class UserConfig
{
public:
    explicit UserConfig(const fs::path& path);

    const fs::path&
    root_dir() const;

private:
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
