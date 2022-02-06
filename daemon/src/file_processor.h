#pragma once

#include <map>
#include <memory>

#include <vca/config.h>
#include <vca/filesystem.h>

#include "tokenizer.h"

namespace vca
{

class FileProcessor
{
public:
    explicit FileProcessor(const AppConfig& app_config);

    void
    set_default_tokenizer(std::unique_ptr<Tokenizer> tokenizer);

    void
    add_tokenizer(String ext, std::unique_ptr<Tokenizer> tokenizer);

    std::vector<std::string>
    process(const Path& file) const;

private:
    const Tokenizer*
    find_tokenizer(const String& ext) const;

    const AppConfig& m_app_config;
    std::unique_ptr<Tokenizer> m_default_tokenizer;
    std::map<String, std::unique_ptr<Tokenizer>> m_tokenizers;
};

} // namespace vca
