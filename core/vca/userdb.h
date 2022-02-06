#pragma once

#include <chrono>
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

    bool
    operator<(const FileContents& o) const
    {
        if (words.size() < o.words.size())
        {
            return true;
        }
        if (words.size() > o.words.size())
        {
            return false;
        }
        for (size_t i = 0; i < words.size(); ++i)
        {
            if (words[i] < o.words[i])
            {
                return true;
            }
        }
        return false;
    }
};

struct SearchResult
{
    Path dir;
    Path file;
    std::string ext;

    bool
    operator<(const SearchResult& o) const
    {
        return std::tie(dir, file, ext) < std::tie(o.dir, o.file, o.ext);
    }
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

    virtual const Path&
    path() const = 0;

    virtual void
    create(const std::set<Path>& root_dirs) = 0;

    virtual void
    add_root_dir(const Path& root_dir) = 0;

    virtual void
    remove_root_dir(const Path& root_dir) = 0;

    virtual void
    update_file(const Path& path, const FileContents& contents) = 0;

    virtual void
    remove_file(const Path& path) = 0;

    virtual void
    move_file(const Path& old_path, const Path& path) = 0;

    virtual std::vector<SearchResult>
    search(const FileContents& contents) const = 0;

    virtual std::chrono::time_point<std::chrono::system_clock>
    last_file_update() const = 0;
};

} // namespace vca
