#include "uds_server.h"

#include <vca/json.h>
#include <vca/logging.h>

using json = nlohmann::json;

namespace vca
{

UdsServer::UdsServer(const HttpServer& http_server, fs::path file)
    : m_http_server{http_server}
    , m_file{std::move(file)}
    , m_running{true}
{
    //    fs::remove_all(m_file);
    //    VCA_CHECK(m_socket);
    //    VCA_CHECK(m_socket.bind(sockpp::unix_address{m_file.u8string()}));
    //    m_thread = std::thread{[this] { run(); }};
}

UdsServer::~UdsServer()
{
    //    m_running = false;
    //    m_socket.send_to(std::string{"q"}, m_socket.address());
    //    m_thread.join();
    //    fs::remove_all(m_file);
}

void
UdsServer::run()
{
    //    VCA_INFO << "Awaiting packets on: " << m_socket.address();
    //    while (m_running)
    //    {
    //        sockpp::unix_address src_addr;
    //        char value;
    //        const auto n = m_socket.recv_from(&value, 1, &src_addr);
    //        if (n != 1)
    //        {
    //            VCA_ERROR << "Error receiving: " << n;
    //            continue;
    //        }
    //        switch (value)
    //        {
    //        case 'q':
    //        {
    //            VCA_INFO << "Exiting UdsServer";
    //            return;
    //        }
    //        case 'i':
    //        {
    //            json j;
    //            j["host"] = m_http_server.host();
    //            j["port"] = std::to_string(m_http_server.port());
    //            std::ostringstream os;
    //            os << j;
    //            const auto data = os.str();
    //            m_socket.send_to(data, src_addr);
    //            break;
    //        }
    //        default:
    //        {
    //            VCA_ERROR << "Received unknown value: " << value;
    //            continue;
    //        }
    //        }
    //    }
}

} // namespace vca
