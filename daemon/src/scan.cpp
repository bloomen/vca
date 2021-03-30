#include "scan.h"

#include <iostream>

namespace vca
{

void
scan(const AppConfig& app_config,
     const UserConfig& user_config,
     UserDb& user_db)
{
    for (const auto& p :
         fs::recursive_directory_iterator{user_config.root_dir()})
    {
        const fs::path path = p;
        if (app_config.matches_ext(path))
        {
            const vca::FileContents contents{{path.filename().stem().string()}};
            user_db.update_file(path, contents);
        }
    }
}

} // namespace vca
