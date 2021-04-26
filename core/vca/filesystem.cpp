#include "filesystem.h"

#include "utils.h"

#include <sago/platform_folders.h>

namespace vca
{

fs::path
user_config_dir()
{
    return fs::u8path(sago::getConfigHome());
}

fs::path
user_documents_dir()
{
    return fs::u8path(sago::getDocumentsFolder());
}

bool
is_parent_of(const fs::path& parent, const fs::path& child)
{
    VCA_CHECK(parent.is_absolute());
    VCA_CHECK(child.is_absolute());
    if (parent == child)
    {
        return false;
    }
    auto parent_it = parent.begin();
    auto child_it = child.begin();
    while (parent_it != parent.end() && child_it != child.end())
    {
        if (*parent_it != *child_it)
        {
            return false;
        }
        ++parent_it;
        ++child_it;
    }
    return child_it != child.end();
}

std::string
read_text(std::istream& f, const size_t max_byte_count)
{
    VCA_CHECK(f.good());
    std::string data(max_byte_count, 0);
    f.read(data.data(), data.size());
    const auto byte_count = f.gcount();
    data.resize(byte_count);
    return data;
}

} // namespace vca
