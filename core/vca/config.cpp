#include "config.h"

#include <fstream>

#include <nlohmann/json.hpp>

#include "utils.h"

using json = nlohmann::json;

namespace vca
{

UserConfig::UserConfig(const fs::path& path)
{
    std::ifstream file{path};
    VCA_CHECK(file);
    json j;
    file >> j;
    VCA_CHECK(!file.bad());
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

} // namespace vca
