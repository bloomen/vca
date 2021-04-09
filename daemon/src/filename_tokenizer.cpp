#include "filename_tokenizer.h"

#include <boost/algorithm/string.hpp>

namespace vca
{

std::vector<std::string>
FilenameTokenizer::extract(const FileData& data) const
{
    std::vector<std::string> tokens;
    boost::split(tokens, data.filename_stem, boost::is_any_of(" _-."));
    tokens.emplace_back(data.filename_ext);
    return tokens;
}

} // namespace vca
