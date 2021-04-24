#pragma once

#include <vector>

#include <vca/filesystem.h>
#include <vca/string.h>

namespace vca
{

class Tokenizer
{
public:
    virtual ~Tokenizer() = default;

    virtual std::vector<String>
    extract(const fs::path& file) const = 0;
};

} // namespace vca
