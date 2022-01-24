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

    qtc::Node<bool>& m_connected = addNode("connected", false, false, false);
    qtc::Node<bool>& m_indexing = addNode("indexing", false, false, false);

    void
    connect() override;

private:
    void
    worker();

    std::mutex m_model_mutex;
    std::atomic<bool> m_done{false};
    std::thread m_thread;
};

} // namespace vca
