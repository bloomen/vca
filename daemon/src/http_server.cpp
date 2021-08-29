#include "http_server.h"

#include <vca/json.h>
#include <vca/logging.h>

using json = nlohmann::json;

namespace vca
{

HttpServer::HttpServer(vca::CommandQueue& commands,
                       UserConfig& user_config,
                       const vca::UserDb& user_db,
                       std::string host,
                       int port)
    : m_commands{commands}
    , m_user_config{user_config}
    , m_user_db{user_db}
    , m_host{std::move(host)}
    , m_port{port}
{
    m_mux.handle("/c")
        .get([this](auto&... p) { get_config(p...); })
        .post([this](auto&... p) { set_config(p...); });
    m_mux.handle("/s").get([this](auto&... p) { search(p...); });
    int retry = 0;
    while (!m_server)
    {
        try
        {
            m_server = std::make_unique<served::net::server>(
                m_host, std::to_string(m_port), m_mux, false);
        }
        catch (const boost::system::system_error&)
        {
            ++retry;
            ++m_port;
            if (retry >= 100)
            {
                throw;
            }
        }
    }
    m_server->run(std::thread::hardware_concurrency(), false);
    VCA_INFO << "Serving on: " << m_host << ":" << m_port;
}

const std::string&
HttpServer::host() const
{
    return m_host;
}

int
HttpServer::port() const
{
    return m_port;
}

HttpServer::~HttpServer()
{
    VCA_INFO << "Exiting HttpServer";
    m_server->stop();
}

void
HttpServer::get_config(served::response& res, const served::request&)
{
    res << m_user_config.as_json();
    res.set_status(200);
}

void
HttpServer::set_config(served::response& res, const served::request& req)
{
    const auto json = req.body();
    m_user_config.set_json(json);
    res.set_status(200);
}

void
HttpServer::search(served::response& res, const served::request& req)
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
