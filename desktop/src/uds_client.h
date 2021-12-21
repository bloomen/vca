#pragma once

#include <atomic>
#include <mutex>
#include <thread>

//#include <sockpp/unix_dgram_socket.h>

#include <vca/filesystem.h>
#include <vca/utils.h>

namespace vca
{

class UdsClient
{
public:
    UdsClient(fs::path client, fs::path server);

    VCA_DELETE_COPY(UdsClient)
    VCA_DELETE_MOVE(UdsClient)

    ~UdsClient();

    const std::string&
    host() const;
    const std::string&
    port() const;

private:
    fs::path m_client;
    //    sockpp::socket_initializer sock_init;
    std::string m_host;
    std::string m_port;
};

} // namespace vca
