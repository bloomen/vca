#pragma once

#include <set>
#include <vector>

#include "filesystem.h"
#include "string.h"

namespace vca
{

struct FileContents
{
    std::vector<std::string> words;

    static FileContents
    fromSearch(std::list<String> values);
};

class UserDb
{
public:
    enum class OpenType
    {
        ReadOnly,
        ReadWrite,
    };

    virtual ~UserDb() = default;

    virtual const fs::path&
    path() const = 0;

    virtual void
    create(const std::set<fs::path>& root_dirs) = 0;

    virtual void
    add_root_dir(const fs::path& root_dir) = 0;

    virtual void
    remove_root_dir(const fs::path& root_dir) = 0;

    virtual void
    update_file(const fs::path& path, const FileContents& contents) = 0;

    virtual void
    remove_file(const fs::path& path) = 0;

    virtual void
    move_file(const fs::path& old_path, const fs::path& path) = 0;

    virtual std::vector<fs::path>
    search(const FileContents& contents) const = 0;
};

} // namespace vca
