#include "config.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <mutex>

#include "json.h"
#include "utils.h"

using json = nlohmann::json;

namespace vca
{

UserConfig::UserConfig(const fs::path& path)
    : m_file_lock{path}
{
    std::lock_guard<FileLock> file_lock{m_file_lock};
    if (fs::exists(path))
    {
        read(path);
    }
    else
    {
        m_root_dir = user_documents_dir();
        if (!fs::exists(m_root_dir))
        {
            m_root_dir = m_root_dir.parent_path();
            VCA_CHECK(fs::exists(m_root_dir))
                << "root dir does not exist: " << m_root_dir;
        }
        write(path);
    }
}

const fs::path&
UserConfig::root_dir() const
{
    return m_root_dir;
}

void
UserConfig::read(const fs::path& path)
{
    json j;
    {
        std::ifstream file{path};
        VCA_CHECK(file);
        j = json::parse(file);
        VCA_CHECK(!file.bad());
    }
    m_root_dir = fs::u8path(j["root_dir"].get<std::string>());
}

void
UserConfig::write(const fs::path& path) const
{
    json j;
    j["root_dir"] = m_root_dir.u8string();
    std::ofstream{path} << j;
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
