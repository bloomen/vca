#pragma once

#include "tokenizer.h"

namespace vca
{

class DocxTokenizer : public Tokenizer
{
public:
    std::vector<String>
    extract(const fs::path& file) const override;
};

} // namespace vca
