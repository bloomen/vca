#pragma once

#include <string>
#include <vector>

#include "filesystem.h"

namespace vca
{

struct FileContents
{
    std::vector<std::string> words;
};

class UserDb
{
public:
    virtual ~UserDb() = default;

    virtual const fs::path&
    path() const = 0;

    virtual void
    truncate() = 0;

    virtual void
    update_file(const fs::path& path, const FileContents& contents) = 0;

    virtual void
    remove_file(const fs::path& path) = 0;

    virtual void
    move_file(const fs::path& old_path, const fs::path& path) = 0;

    virtual std::vector<fs::path>
    search(const FileContents& contents) = 0;
};

} // namespace vca
