#pragma once

#include <qtc/Handler.h>

namespace vca
{

class DaemonHandler : public qtc::Handler
{
public:
    DaemonHandler(const std::string& host,
                  int port,
                  qtc::Model& model,
                  qtc::ThreadPool& threadPool,
                  qtc::CommandQueue& mainQueue);

    qtc::Node<int>& m_daemon_count =
        addNode("daemon_count", int{}, false, false);
    qtc::Node<QList<QString>>& m_daemon_hosts =
        addNode("daemon_hosts", QList<QString>{}, false, false);
    qtc::Node<QList<QString>>& m_daemon_ports =
        addNode("daemon_ports", QList<QString>{}, false, false);

    void
    connect() override;
};

} // namespace vca
