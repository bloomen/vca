#pragma once

#include "tokenizer.h"

namespace vca
{

class XmlTokenizer : public Tokenizer
{
public:
    std::vector<String>
    extract(const Path& file) const override;
};

} // namespace vca
