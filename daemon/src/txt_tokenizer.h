#pragma once

#include "tokenizer.h"

namespace vca
{

class TxtTokenizer : public Tokenizer
{
public:
    explicit TxtTokenizer(bool xml_unescape = false);

    std::vector<String>
    extract(const Path& file) const override;

private:
    bool m_xml_unescape;
};

} // namespace vca
