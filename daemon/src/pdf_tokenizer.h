#pragma once

#include "tokenizer.h"

namespace vca
{

class PdfTokenizer : public Tokenizer
{
public:
    PdfTokenizer() = default;

    std::vector<String>
    extract(const Path& file) const override;
};

} // namespace vca
