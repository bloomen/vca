#include "StatusHandler.h"

#include <restclient-cpp/restclient.h>
#include <served/served.hpp>

#include <vca/json.h>
#include <vca/logging.h>

#include "DaemonHandler.h"

using json = nlohmann::json;

namespace vca
{

StatusHandler::StatusHandler(qtc::Model& model,
                             qtc::ThreadPool& threadPool,
                             qtc::CommandQueue& mainQueue)
    : Handler{model, threadPool, mainQueue, "/status"}
{
}

StatusHandler::~StatusHandler()
{
    m_done = true;
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

void
StatusHandler::connect()
{
    m_thread = std::thread{[this] { worker(); }};
}

void
StatusHandler::worker()
{
    const auto& daemon_handler =
        static_cast<DaemonHandler&>(getHandler("/daemon"));

    while (!m_done)
    {
        const auto hosts = daemon_handler.m_daemon_hosts.getValue();
        const auto ports = daemon_handler.m_daemon_ports.getValue();
        bool connected = false;
        bool indexing = false;
        for (int i = 0; i < daemon_handler.m_daemon_count.getValue(); ++i)
        {
            if (indexing)
            {
                // no need to check other daemons
                break;
            }
            const auto query = "http://" + hosts[i].toStdString() + ":" +
                ports[i].toStdString() + "/t";
            const auto response = RestClient::get(query);
            connected = response.code == 200;
            if (!connected)
            {
                indexing = false;
                VCA_ERROR << "Received error code: " << response.code
                          << " from: " << hosts[i].toStdString();
                break;
            }
            json j = json::parse(response.body);
            indexing = j["indexing"];
        }

        {
            std::lock_guard lock{m_model_mutex};
            m_connected.setValue(connected);
            m_indexing.setValue(indexing);
        }

        std::this_thread::sleep_for(std::chrono::seconds{1});
    }
}

} // namespace vca
