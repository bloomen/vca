#pragma once

#include <atomic>
#include <functional>
#include <future>
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

    template <typename Functor>
    auto
    push(Functor&& functor)
    {
        using result_type = decltype(functor());
        auto task = std::make_shared<std::packaged_task<result_type()>>(
            std::forward<Functor>(functor));
        auto future = task->get_future();
        push_impl([t = std::move(task)] { (*t)(); });
        return future;
    }

    void
    sync(const std::atomic<int>& signal_status);

private:
    void
    push_impl(std::function<void()>&& f);

    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace vca
