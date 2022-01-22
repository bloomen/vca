#include "DaemonHandler.h"

#include <vca/json.h>
#include <vca/logging.h>
#include <vca/time.h>

using json = nlohmann::json;

namespace vca
{

DaemonHandler::DaemonHandler(const std::string& host,
                             const int port,
                             qtc::Model& model,
                             qtc::ThreadPool& threadPool,
                             qtc::CommandQueue& mainQueue)
    : Handler{model, threadPool, mainQueue, "/daemon"}
{
    m_daemon_count.setValue(1);
    QList<QString> hosts;
    hosts.append(QString::fromStdString(host));
    m_daemon_hosts.setValue(std::move(hosts));
    QList<QString> ports;
    ports.append(QString::fromStdString(std::to_string(port)));
    m_daemon_ports.setValue(std::move(ports));
}

void
DaemonHandler::connect()
{
}

} // namespace vca
