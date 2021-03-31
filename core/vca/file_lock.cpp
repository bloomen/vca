#include "file_lock.h"

#include <boost/interprocess/sync/file_lock.hpp>

namespace vca
{

struct FileLock::Impl
{
    explicit Impl(fs::path file)
        : lock{file.u8string().c_str()}
    {
    }

    boost::interprocess::file_lock lock;
};

FileLock::~FileLock() = default;

void
FileLock::lock()
{
    m_impl->lock.lock();
}

void
FileLock::unlock()
{
    m_impl->lock.unlock();
}

} // namespace vca
