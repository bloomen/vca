#pragma once

#include "tokenizer.h"

namespace vca
{

class XmlTokenizer : public Tokenizer
{
public:
    std::vector<String>
    extract(const fs::path& file) const override;
};

} // namespace vca
