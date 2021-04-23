#pragma once

#include <filesystem>

namespace fs = std::filesystem;

namespace vca
{

fs::path
user_config_dir();

fs::path
user_documents_dir();

bool
is_parent_of(const fs::path& parent, const fs::path& child);

} // namespace vca
