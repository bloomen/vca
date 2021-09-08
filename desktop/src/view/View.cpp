#include "View.h"

#include <QtCore/QDir>
#include <QtCore/QUrl>

#include <restclient-cpp/restclient.h>

#include <vca/json.h>
#include <vca/logging.h>
#include <vca/time.h>

using json = nlohmann::json;

namespace vca
{

namespace
{

QString
pathFromUrl(const QString& url)
{
    auto path = QDir::toNativeSeparators(QUrl{url}.path());
    if (path[0] == '\\')
    {
        path.remove(0, 1);
    }
    return path;
}

json
fetchConfig(const QString& host, const QString& port)
{
    const auto query =
        "http://" + host.toStdString() + ":" + port.toStdString() + "/c";
    VCA_INFO << "GET: " << query;
    const auto response = RestClient::get(query);
    if (response.code != 200)
    {
        VCA_ERROR << "Received error code: " << response.code;
        return {};
    }
    return json::parse(response.body);
}

} // namespace

QList<QString>
View::getHostDirectories(const QString& host, const QString& port) const
{
    auto j = fetchConfig(host, port);
    QList<QString> result_dirs;
    for (const auto dir : j["root_dirs"])
    {
        result_dirs.append(QString::fromUtf8(dir.get<std::string>().c_str()));
    }
    return result_dirs;
}

void
View::addHostDirectory(const QString& host,
                       const QString& port,
                       const QString& dir) const
{
    auto j = fetchConfig(host, port);
    const auto query =
        "http://" + host.toStdString() + ":" + port.toStdString() + "/c";
    VCA_INFO << "POST: " << query;
    j["root_dirs"].push_back(pathFromUrl(dir).toStdString());
    std::ostringstream os;
    os << j;
    const auto response = RestClient::post(query, "application/json", os.str());
    if (response.code != 200)
    {
        VCA_ERROR << "Received error code: " << response.code;
    }
}

void
View::removeHostDirectory(const QString& host,
                          const QString& port,
                          const QString& dir) const
{
    const auto dir_std = pathFromUrl(dir).toStdString();
    auto j = fetchConfig(host, port);
    const auto query =
        "http://" + host.toStdString() + ":" + port.toStdString() + "/c";
    VCA_INFO << "POST: " << query;
    size_t index = 0;
    for (const auto d : j["root_dirs"])
    {
        if (d == dir_std)
        {
            j["root_dirs"].erase(index);
            break;
        }
        ++index;
    }
    std::ostringstream os;
    os << j;
    const auto response = RestClient::post(query, "application/json", os.str());
    if (response.code != 200)
    {
        VCA_ERROR << "Received error code: " << response.code;
    }
}

} // namespace vca
