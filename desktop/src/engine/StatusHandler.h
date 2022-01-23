#pragma once

#include <atomic>
#include <mutex>
#include <thread>

#include <qtc/Handler.h>

namespace vca
{

class StatusHandler : public qtc::Handler
{
public:
    StatusHandler(qtc::Model& model,
                  qtc::ThreadPool& threadPool,
                  qtc::CommandQueue& mainQueue);

    ~StatusHandler();

    qtc::Node<bool>& m_indexing =
        addNode("indexing", true, false, false);

    void connect() override;

private:
    void worker();

    std::mutex m_indexing_mutex;
    std::atomic<bool> m_done{false};
    std::thread m_thread;
};

} // namespace vca
