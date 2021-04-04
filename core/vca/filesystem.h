#pragma once

#include <filesystem>

namespace fs = std::filesystem;

namespace vca
{

fs::path
user_config_dir();

fs::path
user_documents_dir();

} // namespace vca
