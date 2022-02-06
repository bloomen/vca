#include "filesystem.h"

#include "utils.h"

#include <sago/platform_folders.h>

namespace vca
{

bool
Path::is_parent_of(const Path& a_child) const
{
    if (*this == a_child)
    {
        return false;
    }
    auto parent = *this;
    if (!parent.is_absolute())
    {
        parent = parent.absolute();
    }
    auto child = a_child;
    if (!child.is_absolute())
    {
        child = child.absolute();
    }
    auto parent_it = parent.m_path.begin();
    auto child_it = child.m_path.begin();
    while (parent_it != parent.m_path.end() && child_it != child.m_path.end())
    {
        if (*parent_it != *child_it)
        {
            return false;
        }
        ++parent_it;
        ++child_it;
    }
    return child_it != child.m_path.end();
}

Path
user_config_dir()
{
    return Path{sago::getConfigHome()};
}

Path
user_documents_dir()
{
    return Path{sago::getDocumentsFolder()};
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
