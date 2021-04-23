#include "filename_tokenizer.h"

#include <vca/string.h>

namespace vca
{

bool
FilenameTokenizer::contents_supported(const String&) const
{
    return false;
}

std::vector<String>
FilenameTokenizer::extract(const FileData& data) const
{
    auto line = data.filename_stem;
    replace_all(line, special_chars(), U' ');
    std::list<String> tokens;
    split(tokens, line);
    tokens.emplace_back(data.filename_ext);
    return {tokens.begin(), tokens.end()};
}

} // namespace vca
