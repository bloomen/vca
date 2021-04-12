#include "txt_tokenizer.h"

#include <boost/algorithm/string.hpp>

#include <vca/string.h>

namespace vca
{

// TODO:
// - include some numbers?
// - ignore common words like I, you, we etc?
std::vector<String>
TxtTokenizer::extract(const FileData& data) const
{
    if (data.filename_ext != U".txt")
    {
        return {};
    }
    if (!data.initial_contents)
    {
        return {};
    }

    String one_line;
    for (const auto& line : *data.initial_contents)
    {
        if (line.back() == U'-')
        {
            one_line.insert(one_line.end(), line.begin(), line.end() - 1);
        }
        else
        {
            one_line += line + U" ";
        }
    }

    to_lower_case(one_line);
    replace_all(one_line, spec_chars(), U' ');

    std::vector<String> words;
    std::list<String> tokens;
    split(tokens, one_line);
    for (auto& t : tokens)
    {
        trim(t);
        if (t.size() <= 1)
        {
            continue;
        }
        if (is_numeric(t))
        {
            continue;
        }
        words.emplace_back(std::move(t));
    }
    return words;
}

} // namespace vca
