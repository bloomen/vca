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
    replace_all(stem, special_chars(), U' ');
    std::list<String> tokens;
    split(tokens, stem);
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
    for (const auto w : words)
    {
        result.emplace_back(wide_to_narrow(w));
    }
    return result;

    //    // check if we want to read binary or text or nothing
    //    bool binary_supported = false;
    //    bool text_supported = false;
    //    for (const auto& tokenizer : m_tokenizers)
    //    {
    //        if (tokenizer->binary_supported(ext))
    //        {
    //            binary_supported = true;
    //            break;
    //        }
    //        if (tokenizer->text_supported(ext))
    //        {
    //            text_supported = true;
    //            break;
    //        }
    //    }

    //    // read contents
    //    constexpr size_t max_byte_count = 8192; // 2^13
    //    std::optional<std::vector<String>> initial_contents;
    //    if (binary_supported)
    //    {
    //        String data(max_byte_count / 4, 0);
    //        std::ifstream f{file, std::ios_base::binary};
    //        f.read(reinterpret_cast<char*>(data.data()), data.size() * 4);
    //        initial_contents = std::vector<String>{std::move(data)};
    //    }
    //    else if (text_supported)
    //    {
    //        initial_contents = std::vector<String>{};
    //        std::ifstream f{file};
    //        std::string line;
    //        size_t bytes = 0;
    //        while (std::getline(f, line))
    //        {
    //            String wide_line;
    //            try
    //            {
    //                wide_line = narrow_to_wide(line);
    //            }
    //            catch (const std::exception& e)
    //            {
    //                VCA_EXCEPTION(e) << e.what() << ": " << file;
    //                initial_contents = {};
    //                break;
    //            }
    //            trim(wide_line);
    //            if (!wide_line.empty())
    //            {
    //                bytes += wide_line.size() * 4;
    //                initial_contents->emplace_back(std::move(wide_line));
    //                if (bytes >= max_byte_count)
    //                {
    //                    break;
    //                }
    //            }
    //        }
    //    }

    //    // extract words
    //    std::set<std::string> words;
    //    const FileData data{stem, ext, initial_contents};
    //    for (const auto& tokenizer : m_tokenizers)
    //    {
    //        auto tokens = tokenizer->extract(data);
    //        for (auto& t : tokens)
    //        {
    //            trim(t);
    //            if (t.size() <= 1)
    //            {
    //                continue;
    //            }
    //            words.emplace(wide_to_narrow(t));
    //        }
    //    }
    //    return {words.begin(), words.end()};
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
