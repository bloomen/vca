#pragma once

#include "tokenizer.h"

namespace vca
{

class FilenameTokenizer : public Tokenizer
{
public:
    std::vector<String>
    extract(const FileData& data) const override;
};

} // namespace vca
