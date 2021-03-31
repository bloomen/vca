#pragma once

#include "tokenizer.h"

namespace vca
{

class DefaultTokenizer : public Tokenizer
{
public:
    std::vector<std::string>
    extract(const FileData& data) const override
    {
        return {data.filename_stem};
    }
};

} // namespace vca
