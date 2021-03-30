#include "file_watcher.h"

namespace vca
{

FileWatcher::FileWatcher(const AppConfig& app_config, const UserConfig& user_config, UserDb& user_db)
    : m_app_config{app_config}
    , m_user_config{user_config}
    , m_user_db{user_db}
{}

void FileWatcher::run()
{

}

}
