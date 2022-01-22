#pragma once

#include <memory>
#include <mutex>

#include "filesystem.h"
#include "utils.h"

namespace vca
{

class FileLock
{
public:
    explicit FileLock(const fs::path& path);

    VCA_DELETE_COPY(FileLock)
    VCA_DEFAULT_MOVE(FileLock)

    ~FileLock();

    void
    lock();

    void
    unlock();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

class FileGuard
{
public:
    explicit FileGuard(const fs::path& path)
        : m_lock{path}
        , m_guard{m_lock}
    {
    }

private:
    vca::FileLock m_lock;
    std::lock_guard<vca::FileLock> m_guard;
};

} // namespace vca
