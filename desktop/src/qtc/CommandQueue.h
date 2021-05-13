#pragma once

#include <functional>
#include <mutex>
#include <queue>

#include <QtGlobal>

namespace qtc
{

class CommandQueue
{
public:
    CommandQueue() = default;

    Q_DISABLE_COPY(CommandQueue)

    void
    push(std::function<void()> functor);

    void
    sync();

private:
    std::mutex mMutex;
    std::queue<std::function<void()>> mCommands;
};

} // namespace qtc
