#pragma once

#include "tokenizer.h"

namespace vca
{

class TxtTokenizer : public Tokenizer
{
public:
    std::vector<std::string>
    extract(const FileData& data) const override;
};

} // namespace vca
