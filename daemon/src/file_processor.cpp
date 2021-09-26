#include "file_processor.h"

#include <fstream>
#include <set>

#include <vca/logging.h>
#include <vca/string.h>

namespace vca
{

namespace
{

std::set<String>
tokenize_filename(String stem, const String& ext)
{
    replace_all(stem, special_chars(), space_char());
    std::list<String> tokens;
    split(tokens, stem, vca::space_char());
    tokens.emplace_back(ext);
    return {tokens.begin(), tokens.end()};
}

} // namespace

FileProcessor::FileProcessor(const AppConfig& app_config)
    : m_app_config{app_config}
{
}

void
FileProcessor::set_default_tokenizer(std::unique_ptr<Tokenizer> tokenizer)
{
    m_default_tokenizer = std::move(tokenizer);
}

void
FileProcessor::add_tokenizer(String ext, std::unique_ptr<Tokenizer> tokenizer)
{
    m_tokenizers.emplace(std::move(ext), std::move(tokenizer));
}

std::vector<std::string>
FileProcessor::process(const fs::path& file) const
{
    const auto stem = file.filename().stem().u32string();
    auto ext = file.extension().u32string();
    to_lower_case(ext);

    auto words = tokenize_filename(stem, ext);

    const auto tokenizer = find_tokenizer(ext);
    if (tokenizer)
    {
        const auto tokens = tokenizer->extract(file);
        words.insert(tokens.begin(), tokens.end());
    }

    std::vector<std::string> result;
    result.reserve(words.size());
    for (const auto& w : words)
    {
        result.emplace_back(wide_to_narrow(w));
    }
    return result;
}

const Tokenizer*
FileProcessor::find_tokenizer(const String& ext) const
{
    const auto tokenizer = m_tokenizers.find(ext);
    if (tokenizer != m_tokenizers.end())
    {
        return tokenizer->second.get();
    }
    return m_default_tokenizer.get();
}

} // namespace vca
