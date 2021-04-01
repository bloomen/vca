#include "config.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <mutex>

#include <nlohmann/json.hpp>

#include "utils.h"

using json = nlohmann::json;

namespace vca
{

UserConfig::UserConfig(const fs::path& path)
    : m_file_lock{path}
{
    json j;
    {
        std::lock_guard<FileLock> file_lock{m_file_lock};
        std::ifstream file{path};
        VCA_CHECK(file);
        file >> j;
        VCA_CHECK(!file.bad());
    }
    m_root_dir = fs::u8path(j["root_dir"].get<std::string>());
}

const fs::path&
UserConfig::root_dir() const
{
    return m_root_dir;
}

const std::set<std::string>&
AppConfig::extensions() const
{
    return m_extensions;
}

bool
AppConfig::matches_ext(const fs::path& path) const
{
    if (!fs::is_regular_file(path))
    {
        return false;
    }
    auto ext = path.extension().u8string();
    // TODO: Does this work with UTF-8?
    std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return m_extensions.count(ext) > 0;
}

} // namespace vca
