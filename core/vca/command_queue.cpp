#include "command_queue.h"

#include <concurrentqueue.h>

#include "logging.h"

namespace vca
{

struct CommandQueue::Impl
{
    moodycamel::ConcurrentQueue<std::function<void()>> queue;
};

CommandQueue::CommandQueue()
    : m_impl{std::make_unique<Impl>()}
{
}

CommandQueue::~CommandQueue() = default;

void
CommandQueue::sync(const std::atomic<int>& signal_status)
{
    std::function<void()> cmd;
    while (signal_status == 0 && m_impl->queue.try_dequeue(cmd))
    {
        cmd();
    }
}

void
CommandQueue::push_impl(std::function<void()>&& cmd)
{
    m_impl->queue.enqueue(std::move(cmd));
}

} // namespace vca
