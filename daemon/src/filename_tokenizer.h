#pragma once

#include "tokenizer.h"

namespace vca
{

class FilenameTokenizer : public Tokenizer
{
public:
    bool
    contents_supported(const String&) const override;

    std::vector<String>
    extract(const FileData& data) const override;
};

} // namespace vca
