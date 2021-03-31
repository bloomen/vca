#pragma once

#include <memory>

#include "filesystem.h"
#include "utils.h"

namespace vca
{

class FileLock
{
public:
    explicit FileLock(fs::path file);

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

} // namespace vca
