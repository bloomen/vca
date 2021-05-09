#include "async.h"

namespace vca
{

Async::Async(const size_t n_threads)
{
    for (size_t i = 0; i < n_threads; ++i)
    {
        std::thread thread;
        try
        {
            thread = std::thread{&Async::worker, this, i};
        }
        catch (...)
        {
            shutdown();
            throw;
        }
        try
        {
            _threads.emplace_back(std::move(thread));
        }
        catch (...)
        {
            shutdown();
            thread.join();
            throw;
        }
    }
}

Async::~Async()
{
    shutdown();
}

size_t
Async::threadCount() const
{
    return _threads.size();
}

void
Async::worker(const size_t)
{
    for (;;)
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock{_mutex};
            _cond_var.wait(lock, [this] { return _done || !_tasks.empty(); });
            if (_done && _tasks.empty())
            {
                break;
            }
            task = std::move(_tasks.front());
            _tasks.pop_front();
        }
        task();
    }
}

void
Async::shutdown()
{
    {
        std::lock_guard<std::mutex> lock{_mutex};
        _done = true;
    }
    _cond_var.notify_all();
    for (std::thread& thread : _threads)
    {
        thread.join();
    }
    _threads.clear();
}

} // namespace vca
