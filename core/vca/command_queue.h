#pragma once

#include <atomic>
#include <functional>
#include <memory>

#include "utils.h"

namespace vca
{

class CommandQueue
{
public:
    CommandQueue();

    VCA_DELETE_COPY(CommandQueue)
    VCA_DEFAULT_MOVE(CommandQueue)

    ~CommandQueue();

    void
    push(std::function<void()> cmd);

    void
    sync(const std::atomic<int>& signal_status);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace vca
