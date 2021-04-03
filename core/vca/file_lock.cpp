#include "file_lock.h"

#include <fstream>

#include <boost/interprocess/sync/file_lock.hpp>

namespace vca
{

struct FileLock::Impl
{
    explicit Impl(const fs::path& file)
        : file_lock{make_file(file).c_str()}
    {
    }

    std::string make_file(const fs::path& file)
    {
        const auto f = file.u8string() + ".lock";
        std::ofstream{f};
        return f;
    }

    boost::interprocess::file_lock file_lock;
};

FileLock::FileLock(const fs::path& file)
    : m_impl{std::make_unique<Impl>(file)}
{
}

FileLock::~FileLock() = default;

void
FileLock::lock()
{
    m_impl->file_lock.lock();
}

void
FileLock::unlock()
{
    m_impl->file_lock.unlock();
}

} // namespace vca
