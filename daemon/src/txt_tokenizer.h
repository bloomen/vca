#pragma once

#include <set>

#include "tokenizer.h"

namespace vca
{

class TxtTokenizer : public Tokenizer
{
public:
    explicit TxtTokenizer(String ext);

    bool
    contents_supported(const String& filename_ext) const override;

    std::vector<String>
    extract(const FileData& data) const override;

private:
    String m_ext;
};

} // namespace vca
