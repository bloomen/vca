#pragma once

#include <vca/config.h>
#include <vca/userdb.h>

namespace vca
{

void
scan(const AppConfig& app_config,
     const UserConfig& user_config,
     UserDb& user_db);

} // namespace vca
