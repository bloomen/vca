#include "scan.h"

#include <algorithm>
#include <cctype>

#include <iostream>

namespace vca
{

bool
matches_ext(const fs::path& path, const std::set<std::string>& extensions)
{
    auto ext = path.extension().u8string();
    // TODO: Does this work with UTF-8?
    std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return extensions.count(ext) > 0;
}

void
scan(const AppConfig& app_config,
     const UserConfig& user_config,
     UserDb& user_db)
{
    for (const auto& p :
         fs::recursive_directory_iterator{user_config.root_dir()})
    {
        const fs::path path = p;
        if (matches_ext(path, app_config.extensions()))
        {
            const vca::FileContents contents{{path.filename().stem().string()}};
            user_db.update_file(path, contents);
        }
    }
}

} // namespace vca
