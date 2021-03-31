#pragma once

#include <memory>

#include <vca/filesystem.h>

#include "tokenizer.h"

namespace vca
{

class FileProcessor
{
public:
    void
    add_tokenizer(std::unique_ptr<Tokenizer> tokenizer);

    std::vector<std::string>
    process(const fs::path& file) const;

private:
    std::vector<std::unique_ptr<Tokenizer>> m_tokenizers;
};

} // namespace vca
