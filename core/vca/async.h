#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <list>
#include <mutex>
#include <thread>
#include <vector>

#include "utils.h"

namespace vca
{

// A simple thread pool used to execute tasks in parallel (if n_threads is
// larger than zero). If n_threads is zero a task pushed is run immediately on
// the current thread.
class Async
{
public:
    explicit Async(size_t n_threads);

    ~Async();

    VCA_DELETE_COPY(Async)
    VCA_DELETE_MOVE(Async)

    size_t
    threadCount() const;

    // Pushes a functor into the thread pool for asynchronous execution.
    // functor is expected to look like: T functor();
    // Returns a future of the form: std::future<T>
    template <typename Functor>
    auto
    push(Functor&& functor)
    {
        using result_type = decltype(functor());
        auto task = std::make_shared<std::packaged_task<result_type()>>(
            std::forward<Functor>(functor));
        auto future = task->get_future();
        if (_threads.empty())
        {
            (*task)();
        }
        else
        {
            std::lock_guard<std::mutex> lock{_mutex};
            _tasks.emplace_back([t = std::move(task)] { (*t)(); });
            _cond_var.notify_one();
        }
        return future;
    }

private:
    void
    worker(size_t index);

    void
    shutdown();

    bool _done = false;
    std::vector<std::thread> _threads;
    std::list<std::function<void()>> _tasks;
    std::condition_variable _cond_var;
    std::mutex _mutex;
};

} // namespace vca
