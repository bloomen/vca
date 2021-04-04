#include "file_processor.h"

#include <fstream>
#include <set>

namespace vca
{

void
FileProcessor::add_tokenizer(std::unique_ptr<Tokenizer> tokenizer)
{
    m_tokenizers.emplace_back(std::move(tokenizer));
}

std::vector<std::string>
FileProcessor::process(const fs::path& file) const
{
    // TODO: add check for binary
    const auto filename_stem = file.filename().stem().u8string();
    const auto filename_ext = file.extension().u8string();
    std::vector<std::string> initial_contents;
    {
        std::ifstream f{file};
        std::string line;
        const size_t max_line_count = 100;
        for (size_t i = 0; i < max_line_count && std::getline(f, line); ++i)
        {
            if (!line.empty())
            {
                initial_contents.emplace_back(line);
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
            words.emplace(std::move(t));
        }
    }
    return {words.begin(), words.end()};
}

} // namespace vca
