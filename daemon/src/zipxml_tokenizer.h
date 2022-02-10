#pragma once

#include "tokenizer.h"

namespace vca
{

class ZipxmlTokenizer : public Tokenizer
{
public:
    explicit ZipxmlTokenizer(std::string entry);

    std::vector<String>
    extract(const Path& file) const override;

private:
    std::string m_entry;
};

} // namespace vca
