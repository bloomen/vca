#include "server.h"

#include <vca/json.h>
#include <vca/logging.h>

using json = nlohmann::json;

namespace vca
{

Server::Server(vca::CommandQueue& commands,
               UserConfig& user_config,
               const vca::UserDb& user_db,
               const std::string& host,
               const std::string& port)
    : m_commands{commands}
    , m_user_config{user_config}
    , m_user_db{user_db}
{
    m_mux.handle("/c")
        .get([this](auto&... p) { get_config(p...); })
        .post([this](auto&... p) { set_config(p...); });
    m_mux.handle("/s").get([this](auto&... p) { search(p...); });
    m_server = std::make_unique<served::net::server>(host, port, m_mux, false);
    m_server->run(std::thread::hardware_concurrency(), false);
    VCA_INFO << "Serving on: " << host << ":" << port;
}

Server::~Server()
{
    m_server->stop();
}

void
Server::get_config(served::response& res, const served::request&)
{
    res << m_user_config.as_json();
    res.set_status(200);
}

void
Server::set_config(served::response& res, const served::request& req)
{
    const auto json = req.body();
    m_user_config.set_json(json);
    res.set_status(200);
}

void
Server::search(served::response& res, const served::request& req)
{
    const auto value = req.query.get("v");

    auto wide_input = vca::narrow_to_wide(value);
    vca::trim(wide_input);

    std::list<vca::String> values;
    vca::split(values, wide_input, vca::space_char());

    std::vector<SearchResult> results;
    if (!values.empty())
    {
        auto file_contents = vca::FileContents::fromSearch(values);
        auto future =
            m_commands.push([this, file_contents = std::move(file_contents)] {
                VCA_INFO << "Searching ...";
                vca::Timer timer;
                auto results = m_user_db.search(file_contents);
                VCA_INFO << "Search took: " << timer.us() << " us";
                return results;
            });
        results = future.get();
    }

    json j;
    auto j_results = json::array();
    for (const auto r : results)
    {
        auto j_r = json::object();
        j_r["d"] = r.dir.u8string();
        j_r["f"] = r.file.u8string();
        j_r["e"] = r.ext;
        j_results.push_back(std::move(j_r));
    }
    j["results"] = j_results;

    std::ostringstream os;
    os << j;
    res << os.str();
    res.set_status(200);
}

} // namespace vca
