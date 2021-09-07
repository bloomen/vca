#include "View.h"

#include <restclient-cpp/restclient.h>

#include <vca/json.h>
#include <vca/logging.h>
#include <vca/time.h>

using json = nlohmann::json;

namespace vca
{

QList<QString>
View::getHostDirectories(const QString& host, const QString& port) const
{
    const auto query =
        "http://" + host.toStdString() + ":" + port.toStdString() + "/c";
    VCA_INFO << "Query: " << query;
    const auto response = RestClient::get(query);
    if (response.code != 200)
    {
        VCA_ERROR << "Received error code: " << response.code;
        return {};
    }
    json j = json::parse(response.body);
    QList<QString> result_dirs;
    for (const auto dir : j["root_dirs"])
    {
        result_dirs.append(QString::fromUtf8(dir.get<std::string>().c_str()));
    }
    return result_dirs;
}

} // namespace vca
