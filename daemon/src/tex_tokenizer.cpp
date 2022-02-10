#include "tex_tokenizer.h"

#include <fstream>

#include <vca/filesystem.h>
#include <vca/logging.h>

namespace vca
{

namespace
{

constexpr size_t g_max_byte_count = 81920;

}

std::vector<String>
TexTokenizer::extract(const fs::path& file) const
{
    std::string data;
    {
        std::ifstream f{file};
        try
        {
            data = read_text(f, g_max_byte_count);
        }
        catch (...)
        {
            VCA_DEBUG << "Read text failed for: " << file;
            return {};
        }
    }

    std::string text;
    bool is_text = true;
    for (auto c = data.begin(); c != data.end(); ++c)
    {
        if (is_text)
        {
            if (*c == '\\')
            {
                is_text = false;
            }
        }
        else
        {
            if (*c == ' ' || *c == '{' || *c == ']')
            {
                is_text = true;
                continue;
            }
        }

        if (is_text)
        {
            if (*c == '{' || *c == '}' || *c == '[' || *c == ']')
            {
                text += ' ';
            }
            else if (*c != '$')
            {
                text += *c;
            }
        }
    }

    return tokenize(narrow_to_wide(text));
}

} // namespace vca
