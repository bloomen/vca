#pragma once

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
    add_tokenizer(std::unique_ptr<Tokenizer> tokenizer);

    std::vector<std::string>
    process(const fs::path& file) const;

private:
    const AppConfig& m_app_config;
    std::vector<std::unique_ptr<Tokenizer>> m_tokenizers;
};

} // namespace vca
