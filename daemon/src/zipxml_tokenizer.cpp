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
{}

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

    XMLParser parser{content};

    String one_line;
    size_t byte_count = 0;
    while (!parser.end() && byte_count < g_max_byte_count)
    {
        const auto line = parser.next();
        if (line.empty())
        {
            continue;
        }
        byte_count += line.size() * 4;
        if (line.back() == U'-')
        {
            one_line.insert(one_line.end(), line.begin(), line.end() - 1);
        }
        else
        {
            one_line += line + U" ";
        }
    }

    replace_all(one_line, special_chars(), space_char());

    std::vector<String> words;
    std::list<String> tokens;
    split(tokens, one_line, space_char());
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
