#include "zipxml_tokenizer.h"

#include <vca/logging.h>
#include <vca/string.h>
#include <vca/zip_inflater.h>

namespace vca
{

namespace
{

constexpr size_t g_max_byte_count = 81920;

}

ZipxmlTokenizer::ZipxmlTokenizer(std::string entry)
    : m_entry{std::move(entry)}
{
}

std::vector<String>
ZipxmlTokenizer::extract(const fs::path& file) const
{
    std::string content;
    try
    {
        content = ZipInflater{file, g_max_byte_count, m_entry}.get();
    }
    catch (...)
    {
        VCA_DEBUG << "Zip inflate failed for: " << file;
        return {};
    }

    auto tag_content = xml_tag_content(content, g_max_byte_count);
    return tokenize(std::move(tag_content));
}

} // namespace vca
