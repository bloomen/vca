#pragma once

#include <served/served.hpp>

#include <vca/command_queue.h>
#include <vca/config.h>
#include <vca/time.h>
#include <vca/userdb.h>

namespace vca
{

class HttpServer
{
public:
    HttpServer(vca::CommandQueue& commands,
               UserConfig& user_config,
               const vca::UserDb& user_db,
               std::string host,
               int port);

    const std::string&
    host() const;
    int
    port() const;

    VCA_DELETE_COPY(HttpServer)
    VCA_DELETE_MOVE(HttpServer)

    ~HttpServer();

private:
    void
    get_config(served::response& res, const served::request& req);
    void
    set_config(served::response& res, const served::request& req);
    void
    search(served::response& res, const served::request& req);

    vca::CommandQueue& m_commands;
    UserConfig& m_user_config;
    const vca::UserDb& m_user_db;
    std::string m_host;
    int m_port;
    served::multiplexer m_mux;
    std::unique_ptr<served::net::server> m_server;
};

} // namespace vca