#include "filename_tokenizer.h"

#include <vca/string.h>

namespace vca
{

std::vector<std::string>
FilenameTokenizer::extract(const FileData& data) const
{
    auto line = data.filename_stem;
    replace_all(line, special_chars(), ' ');
    std::vector<std::string> tokens;
    split(tokens, line);
    tokens.emplace_back(data.filename_ext);
    return tokens;
}

} // namespace vca
