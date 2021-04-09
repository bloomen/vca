#include "txt_tokenizer.h"

#include <boost/algorithm/string.hpp>

namespace vca
{

// TODO:
// - ignore most numbers
// - ignore common words like I, you, we etc
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
    std::vector<std::string> words;
    for (const auto& line : *data.initial_contents)
    {
        std::vector<std::string> tokens;
        boost::split(tokens, line, boost::is_any_of(" _-.?!"));
        for (auto& t : tokens)
        {
            words.emplace_back(std::move(t));
        }
    }
    return words;
}

} // namespace vca
