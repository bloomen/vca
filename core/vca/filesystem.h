#pragma once

#include <filesystem>
#include <vector>

#include "string.h"

namespace fs = std::filesystem;

namespace vca
{

fs::path
user_config_dir();

fs::path
user_documents_dir();

bool
is_parent_of(const fs::path& parent, const fs::path& child);

String
read_text(std::istream& f, size_t max_byte_count);

} // namespace vca
