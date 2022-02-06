#pragma once

#include <filesystem>
#include <vector>

#include "platform.h"
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

std::string
read_text(std::istream& f, size_t max_byte_count);

inline fs::path
native_path(const fs::path& path)
{
#ifdef VCA_PLATFORM_WINDOWS
    constexpr auto prefix = R"(\\?\)";
    constexpr auto prefix_size = 4;
    const auto str = path.u8string();
    if (str.size() >= prefix_size &&
        std::equal(str.begin(),
                   str.begin() + prefix_size,
                   prefix,
                   prefix + prefix_size))
    {
        return path;
    }
    else
    {
        return prefix + str;
    }
#else
    return path;
#endif
}

inline fs::path
display_path(const fs::path& path)
{
#ifdef VCA_PLATFORM_WINDOWS
    constexpr auto prefix = R"(\\?\)";
    constexpr auto prefix_size = 4;
    const auto str = path.u8string();
    if (str.size() >= prefix_size &&
        std::equal(str.begin(),
                   str.begin() + prefix_size,
                   prefix,
                   prefix + prefix_size))
    {
        return str.substr(prefix_size);
    }
    else
    {
        return path;
    }
#else
    return path;
#endif
}

} // namespace vca
