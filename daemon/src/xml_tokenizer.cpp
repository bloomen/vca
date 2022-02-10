#include "xml_tokenizer.h"

#include <fstream>

#include <vca/filesystem.h>
#include <vca/logging.h>

namespace vca
{

namespace
{

constexpr size_t g_max_byte_count = 8192;

}

std::vector<String>
XmlTokenizer::extract(const Path& file) const
{
    std::string content;
    {
        auto f = make_ifstream(file, std::ios_base::binary);
        try
        {
            content = read_text(f, g_max_byte_count * 2);
        }
        catch (...)
        {
            VCA_DEBUG << "Read text failed for: " << file;
            return {};
        }
    }

    auto tag_content = xml_tag_content(content, g_max_byte_count);
    return tokenize(std::move(tag_content));
}

} // namespace vca
