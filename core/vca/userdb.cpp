#include "userdb.h"

namespace vca
{

FileContents
FileContents::fromSearch(std::list<String> values)
{
    for (auto& v : values)
    {
        vca::trim(v);
    }
    values.remove_if([](const auto& v) { return v.empty(); });
    vca::FileContents file_contents;
    file_contents.words.reserve(values.size());
    for (const auto& v : values)
    {
        file_contents.words.emplace_back(vca::wide_to_narrow(v));
    }
    return file_contents;
}

} // namespace vca
