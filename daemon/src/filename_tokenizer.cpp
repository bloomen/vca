#include "filename_tokenizer.h"

#include <boost/algorithm/string.hpp>

namespace vca
{

std::vector<std::string>
FilenameTokenizer::extract(const FileData& data) const
{
    return {data.filename_stem + data.filename_ext};
}

} // namespace vca
