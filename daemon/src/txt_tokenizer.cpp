#include "txt_tokenizer.h"

#include <fstream>

#include <vca/filesystem.h>
#include <vca/logging.h>

namespace vca
{

std::vector<String>
TxtTokenizer::extract(const fs::path& file) const
{
    String one_line;
    {
        std::ifstream f{file, std::ios_base::binary};
        constexpr size_t max_byte_count = 8192;
        try
        {
            one_line = read_text(f, max_byte_count);
        }
        catch (...)
        {
            VCA_DEBUG << "Read text failed for: " << file;
            return {};
        }
    }

    replace_all(one_line, {U'\r', U'\025', U'\036'}, U'\n');
    std::list<String> lines;
    split(lines, one_line, U'\n');

    one_line.clear();
    for (const auto& line : lines)
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

    replace_all(one_line, special_chars(), U' ');

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
