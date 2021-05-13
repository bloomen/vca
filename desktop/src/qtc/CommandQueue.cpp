#include "CommandQueue.h"

namespace qtc
{

void
CommandQueue::push(std::function<void()> functor)
{
    std::lock_guard<std::mutex> lock{mMutex};
    mCommands.push(std::move(functor));
}

void
CommandQueue::sync()
{
    decltype(mCommands) commands;
    {
        std::lock_guard<std::mutex> lock{mMutex};
        if (mCommands.empty())
        {
            return;
        }
        std::swap(commands, mCommands);
    }
    while (!commands.empty())
    {
        commands.front()();
        commands.pop();
    }
}

} // namespace qtc
