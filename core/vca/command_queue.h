#pragma once

#include <condition_variable>
#include <functional>
#include <list>
#include <mutex>

namespace vca
{

class CommandQueue
{
public:
    CommandQueue() = default;

    ~CommandQueue();

    void
    push(std::function<void()> cmd);

    void
    run();

private:
    std::mutex m_mutex;
    bool m_done = false;
    std::condition_variable m_cv;
    std::list<std::function<void()>> m_cmds;
};

} // namespace vca
