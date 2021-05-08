#include "zipxml_tokenizer.h"

#include <vca/logging.h>
#include <vca/string.h>
#include <vca/zip_inflater.h>

namespace vca
{

namespace
{

constexpr size_t g_max_byte_count = 8192;

}

ZipxmlTokenizer::ZipxmlTokenizer(std::string entry)
    : m_entry{std::move(entry)}
{
}

std::vector<String>
ZipxmlTokenizer::extract(const fs::path& file) const
{
    ZipInflater zip_inflater{file, g_max_byte_count, m_entry};
    auto tag_content = xml_tag_content(zip_inflater.get(), g_max_byte_count);
    return tokenize(std::move(tag_content));
}

} // namespace vca
