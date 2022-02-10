#pragma once

#include "tokenizer.h"

namespace vca
{

class TexTokenizer : public Tokenizer
{
public:
    TexTokenizer() = default;

    std::vector<String>
    extract(const Path& file) const override;
};

} // namespace vca
