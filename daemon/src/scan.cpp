#include "scan.h"

#include <iostream>

namespace vca
{

void
scan(const AppConfig& app_config,
     const UserConfig& user_config,
     UserDb& user_db,
     const FileProcessor& file_processor)
{
    for (const auto& p :
         fs::recursive_directory_iterator{user_config.root_dir()})
    {
        const fs::path path = p;
        if (app_config.matches_ext(path))
        {
            vca::FileContents contents;
            contents.words = file_processor.process(path);
            user_db.update_file(path, contents);
        }
    }
}

} // namespace vca
