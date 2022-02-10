#pragma once

#include "tokenizer.h"

namespace vca
{

class TexTokenizer : public Tokenizer
{
public:
    TexTokenizer() = default;

    std::vector<String>
    extract(const fs::path& file) const override;
};

} // namespace vca
