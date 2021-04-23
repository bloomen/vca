#pragma once

#include <set>

#include "tokenizer.h"

namespace vca
{

class TxtTokenizer : public Tokenizer
{
public:
    explicit TxtTokenizer(const String& exts);

    bool
    contents_supported(const String& filename_ext) const override;

    std::vector<String>
    extract(const FileData& data) const override;

private:
    std::set<String> m_exts;
};

} // namespace vca
