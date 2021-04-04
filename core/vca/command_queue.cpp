#include "command_queue.h"

#include "logging.h"

namespace vca
{

CommandQueue::~CommandQueue()
{
    std::lock_guard<std::mutex> lock{m_mutex};
    m_done = true;
    m_cv.notify_one();
}

void
CommandQueue::push(std::function<void()> cmd)
{
    std::lock_guard<std::mutex> lock{m_mutex};
    m_cmds.emplace_back(std::move(cmd));
    m_cv.notify_one();
}

void
CommandQueue::run()
{
    for (;;)
    {
        std::list<std::function<void()>> cmds;
        {
            std::unique_lock<std::mutex> lock{m_mutex};
            m_cv.wait(lock, [this] { return m_done || !m_cmds.empty(); });
            if (m_done && m_cmds.empty())
            {
                break;
            }
            std::swap(m_cmds, cmds);
        }
        for (const auto& cmd : cmds)
        {
            cmd();
        }
    }
}

} // namespace vca
