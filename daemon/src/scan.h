#pragma once

#include <vca/config.h>
#include <vca/userdb.h>

namespace vca
{

bool
matches_ext(const fs::path& path, const std::set<std::string>& extensions);

void
scan(const AppConfig& app_config,
     const UserConfig& user_config,
     UserDb& user_db);

} // namespace vca
