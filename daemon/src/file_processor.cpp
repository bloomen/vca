#include "file_processor.h"

#include <fstream>
#include <set>

#include <vca/string.h>

namespace vca
{

FileProcessor::FileProcessor(const AppConfig& app_config)
    : m_app_config{app_config}
{
}

void
FileProcessor::add_tokenizer(std::unique_ptr<Tokenizer> tokenizer)
{
    m_tokenizers.emplace_back(std::move(tokenizer));
}

std::vector<std::string>
FileProcessor::process(const fs::path& file) const
{
    const auto filename_stem = file.filename().stem().u32string();
    auto filename_ext = file.extension().u32string();
    to_lower_case(filename_ext);
    std::optional<std::vector<String>> initial_contents;
    if (m_app_config.extensions().count(filename_ext) > 0)
    {
        initial_contents = std::vector<String>{};
        std::ifstream f{file};
        std::string line;
        constexpr size_t max_byte_count = 10000;
        size_t bytes = 0;
        while (std::getline(f, line))
        {
            auto wide_line = narrow_to_wide(line);
            trim(wide_line);
            if (!wide_line.empty())
            {
                bytes += wide_line.size() * 4;
                initial_contents->emplace_back(std::move(wide_line));
                if (bytes >= max_byte_count)
                {
                    break;
                }
            }
        }
    }

    std::set<std::string> words;
    const FileData data{filename_stem, filename_ext, initial_contents};
    for (const auto& tokenizer : m_tokenizers)
    {
        auto tokens = tokenizer->extract(data);
        for (auto& t : tokens)
        {
            trim(t);
            if (t.size() <= 1)
            {
                continue;
            }
            words.emplace(wide_to_narrow(t));
        }
    }
    return {words.begin(), words.end()};
}

} // namespace vca
