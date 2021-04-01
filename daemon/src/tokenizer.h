#pragma once

#include <string>
#include <vector>

namespace vca
{

struct FileData
{
    const std::string& filename_stem;
    const std::string& filename_ext;
    const std::vector<std::string>& initial_contents;
};

class Tokenizer
{
public:
    virtual ~Tokenizer() = default;

    virtual std::vector<std::string>
    extract(const FileData& data) const = 0;
};

} // namespace vca
