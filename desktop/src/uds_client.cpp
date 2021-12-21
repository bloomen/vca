#include "uds_client.h"

#include <vca/json.h>
#include <vca/logging.h>

using json = nlohmann::json;

namespace vca
{

UdsClient::UdsClient(fs::path client, fs::path server)
    : m_client{std::move(client)}
{
    //    fs::remove_all(m_client);

    //    sockpp::unix_dgram_socket socket;
    //    VCA_CHECK(socket);
    //    VCA_CHECK(socket.bind(sockpp::unix_address{m_client.u8string()}));
    //    socket.send_to(std::string{"i"},
    //    sockpp::unix_address{server.u8string()});

    //    char buf[512];
    //    const auto n = socket.recv_from(buf, sizeof(buf));
    //    VCA_CHECK(n >= 0) << "Error receiving: " << n;
    //    buf[n] = '\n';
    //    VCA_INFO << "Received: " << buf;

    //    auto j = json::parse(buf);
    //    m_host = j["host"];
    //    m_port = j["port"];
    m_host = "localhost";
    m_port = "7777";
}

const std::string&
UdsClient::host() const
{
    return m_host;
}

const std::string&
UdsClient::port() const
{
    return m_port;
}

UdsClient::~UdsClient()
{
    //    fs::remove_all(m_client);
}

} // namespace vca
