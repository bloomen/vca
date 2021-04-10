#include "txt_tokenizer.h"

#include <boost/algorithm/string.hpp>

#include <vca/string.h>

namespace vca
{

// TODO:
// - include some numbers?
// - ignore common words like I, you, we etc?
std::vector<std::string>
TxtTokenizer::extract(const FileData& data) const
{
    if (data.filename_ext != ".txt")
    {
        return {};
    }
    if (!data.initial_contents)
    {
        return {};
    }

    std::string one_line;
    for (const auto& line : *data.initial_contents)
    {
        auto trimmed = line;
        boost::trim(trimmed);
        if (trimmed.back() == '-')
        {
            one_line.insert(one_line.end(), trimmed.begin(), trimmed.end() - 1);
        }
        else
        {
            one_line += trimmed + " ";
        }
    }

    replace_all(one_line, special_chars(), ' ');

    std::vector<std::string> words;
    std::vector<std::string> tokens;
    split(tokens, one_line);
    for (auto& t : tokens)
    {
        boost::trim(t);
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
