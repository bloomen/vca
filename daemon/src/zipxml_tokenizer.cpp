#include "zipxml_tokenizer.h"

#include <vca/logging.h>
#include <vca/string.h>

#include <zip.h>

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
    auto zip_file = zip_open(file.u8string().c_str(), 0, 'r');
    if (!zip_file)
    {
        VCA_ERROR << "Could not open zip file: " << file;
        return {};
    }
    if (zip_entry_open(zip_file, m_entry.c_str()))
    {
        VCA_ERROR << "Could not open zip entry in: " << file;
        zip_close(zip_file);
        return {};
    }

    void* buf = nullptr;
    size_t size;
    // TODO: Replace with only reading the first little bit of the file
    zip_entry_read(zip_file, &buf, &size);

    zip_entry_close(zip_file);
    zip_close(zip_file);
    const std::string content(static_cast<char*>(buf),
                              static_cast<char*>(buf) + size);
    free(buf);

    if (!buf || !size)
    {
        VCA_ERROR << "Reading from zip file failed: " << file;
        return {};
    }

    auto tag_content = xml_tag_content(content, g_max_byte_count);
    return tokenize(std::move(tag_content));
}

} // namespace vca
