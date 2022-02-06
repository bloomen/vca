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

    return tokenize(std::move(one_line));
}

} // namespace vca
