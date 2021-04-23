#pragma once

#include <optional>
#include <string>
#include <vector>

#include <vca/string.h>

namespace vca
{

struct FileData
{
    const String& filename_stem;
    const String& filename_ext;
    const std::optional<std::vector<String>>& initial_contents;
};

class Tokenizer
{
public:
    virtual ~Tokenizer() = default;

    virtual bool
    contents_supported(const String& filename_ext) const = 0;

    virtual std::vector<String>
    extract(const FileData& data) const = 0;
};

} // namespace vca
