#pragma once

#include <sockpp/unix_dgram_socket.h>

#include "http_server.h"

namespace vca
{

class UdsServer
{
public:
    UdsServer(const HttpServer& http_server, fs::path file);

    VCA_DELETE_COPY(UdsServer)
    VCA_DELETE_MOVE(UdsServer)

    ~UdsServer();

private:
    void
    run();

    const HttpServer& m_http_server;
    fs::path m_file;
    sockpp::socket_initializer sock_init;
    sockpp::unix_dgram_socket m_socket;
    std::atomic<bool> m_running;
    std::thread m_thread;
    std::mutex m_mutex;
};

} // namespace vca
