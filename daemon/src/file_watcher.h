#pragma once

#include <vca/config.h>
#include <vca/userdb.h>

namespace vca
{

class FileWatcher
{
public:
    FileWatcher(const AppConfig& app_config, const UserConfig& user_config, UserDb& user_db);

    void run();

private:
    const AppConfig& m_app_config;
    const UserConfig& m_user_config;
    UserDb& m_user_db;
};

}
