#pragma once

#include <vca/config.h>
#include <vca/userdb.h>

#include "file_processor.h"

namespace vca
{

void
scan(const AppConfig& app_config,
     const UserConfig& user_config,
     UserDb& user_db,
     const FileProcessor& file_processor);

} // namespace vca
