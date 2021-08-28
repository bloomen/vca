#pragma once

#include <served/served.hpp>

#include <vca/command_queue.h>
#include <vca/config.h>
#include <vca/time.h>
#include <vca/userdb.h>

namespace vca
{

class Server
{
public:
    Server(vca::CommandQueue& commands,
           UserConfig& user_config,
           const vca::UserDb& user_db,
           const std::string& host,
           const std::string& port);

    VCA_DELETE_COPY(Server)
    VCA_DELETE_MOVE(Server)

    ~Server();

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
    served::multiplexer m_mux;
    std::unique_ptr<served::net::server> m_server;
};

} // namespace vca
