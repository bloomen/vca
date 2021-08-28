#pragma once

#include <served/served.hpp>

#include <vca/command_queue.h>
#include <vca/time.h>
#include <vca/userdb.h>

namespace vca
{

class Server
{
public:
    Server(vca::CommandQueue& commands,
           const vca::UserDb& user_db,
           const std::string& host,
           const std::string& port);

    VCA_DEFAULT_COPY(Server)
    VCA_DEFAULT_MOVE(Server)

    ~Server();

private:
    void
    search(served::response& res, const served::request& req);

    vca::CommandQueue& m_commands;
    const vca::UserDb& m_user_db;
    served::multiplexer m_mux;
    std::unique_ptr<served::net::server> m_server;
};

} // namespace vca
