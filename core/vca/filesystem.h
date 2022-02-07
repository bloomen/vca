#pragma once

#include <filesystem>
#include <fstream>
#include <vector>

#include "string.h"

namespace vca
{

class Path
{
public:
    Path() = default;

    explicit Path(const std::filesystem::path& p)
        : m_path{p}
    {
    }

    explicit Path(std::filesystem::path&& p)
        : m_path{std::move(p)}
    {
    }

    explicit Path(const String& p)
        : m_path{std::filesystem::u8path(wide_to_narrow(p))}
    {
    }

    explicit Path(const std::string& p)
        : m_path{std::filesystem::u8path(p)}
    {
    }

    explicit Path(const char* p)
        : m_path{std::filesystem::u8path(p)}
    {
    }

    Path
    absolute() const
    {
        return Path{std::filesystem::absolute(m_path)};
    }

    Path
    canonical() const
    {
        return Path{std::filesystem::canonical(m_path)};
    }

    bool
    operator<(const Path& p) const
    {
        return m_path < p.m_path;
    }

    Path&
    operator/=(const Path& p)
    {
        m_path /= p.m_path;
        return *this;
    }

    std::string
    to_narrow() const
    {
        return m_path.u8string();
    }

    String
    to_wide() const
    {
        return narrow_to_wide(m_path.u8string());
    }

    bool
    exists() const
    {
        return std::filesystem::exists(m_path);
    }

    bool
    is_absolute() const
    {
        return m_path.is_absolute();
    }

    bool
    is_relative() const
    {
        return m_path.is_relative();
    }

    bool
    is_file() const
    {
        return std::filesystem::is_regular_file(m_path);
    }

    bool
    is_parent_of(const Path& child) const;

    Path
    parent() const
    {
        return Path{m_path.parent_path()};
    }

    Path
    filename() const
    {
        return Path{m_path.filename()};
    }

    Path
    stem() const
    {
        return Path{m_path.stem()};
    }

    Path
    extension() const
    {
        return Path{m_path.extension()};
    }

private:
    friend inline bool
    operator==(const Path& l, const Path& r);

    friend inline std::ostream&
    operator<<(std::ostream& os, const Path& p);

    friend inline void
    create_directories(const Path& p);

    friend inline Path
    relative(const Path& p1, const Path& p2);

    friend inline void
    remove(const Path& p);

    friend inline auto
    make_rec_dir_iterator(const Path& p);

    friend inline std::ifstream
    make_ifstream(const Path& p, std::ios_base::openmode mode);

    friend inline std::ofstream
    make_ofstream(const Path& p, std::ios_base::openmode mode);

    std::filesystem::path m_path;
};

inline Path
operator/(const Path& l, const Path& r)
{
    Path res{l};
    res /= r;
    return res;
}

inline bool
operator==(const Path& l, const Path& r)
{
    return l.m_path == r.m_path;
}

inline bool
operator!=(const Path& l, const Path& r)
{
    return !(l == r);
}

inline std::ostream&
operator<<(std::ostream& os, const Path& p)
{
    os << p.m_path;
    return os;
}

inline void
create_directories(const Path& p)
{
    std::filesystem::create_directories(p.m_path);
}

inline Path
relative(const Path& p1, const Path& p2)
{
    return Path{std::filesystem::relative(p1.m_path, p2.m_path)};
}

inline void
remove(const Path& p)
{
    std::filesystem::remove(p.m_path);
}

inline auto
make_rec_dir_iterator(const Path& p)
{
    return std::filesystem::recursive_directory_iterator{p.m_path};
}

inline std::ifstream
make_ifstream(const Path& p, std::ios_base::openmode mode = std::ios_base::in)
{
    return std::ifstream{p.m_path, mode};
}

inline std::ofstream
make_ofstream(const Path& p, std::ios_base::openmode mode = std::ios_base::out)
{
    return std::ofstream{p.m_path, mode};
}

Path
user_config_dir();

Path
user_documents_dir();

std::string
read_text(std::istream& f, size_t max_byte_count);

} // namespace vca
