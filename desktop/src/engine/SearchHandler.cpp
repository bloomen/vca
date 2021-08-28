#include "SearchHandler.h"

#include <restclient-cpp/restclient.h>
#include <served/served.hpp>

#include <vca/json.h>
#include <vca/logging.h>
#include <vca/time.h>

using json = nlohmann::json;

namespace vca
{

SearchHandler::SearchHandler(qtc::Model& model,
                             qtc::ThreadPool& threadPool,
                             qtc::CommandQueue& mainQueue)
    : Handler{model, threadPool, mainQueue, "/search"}
{
}

void
SearchHandler::connect()
{
    m_input.addListener(
        [this](const qtc::Node<QString>& input, qtc::UpdateType) {
            const auto value = input.getValue();
            if (value.isEmpty())
            {
                m_result_count.assignDefault();
                m_result_dirs.assignDefault();
                m_result_files.assignDefault();
                m_result_exts.assignDefault();
                return;
            }

            VCA_INFO << "Searching ...";
            vca::Timer timer;
            const auto query = "http://127.0.0.1:7777/s?v=" +
                served::query_escape(value.toStdString());
            VCA_INFO << "Query: " << query;
            const auto response = RestClient::get(query);
            if (response.code != 200)
            {
                VCA_ERROR << "Received error code: " << response.code;
                return;
            }
            VCA_INFO << "Search took: " << timer.us() << " us";
            json j = json::parse(response.body);

            QList<QString> result_dirs;
            QList<QString> result_files;
            QList<QString> result_exts;

            for (const auto& result : j["results"])
            {
                result_dirs.append(
                    QString::fromUtf8(result["d"].get<std::string>().c_str()));
                result_files.append(
                    QString::fromUtf8(result["f"].get<std::string>().c_str()));
                result_exts.append(
                    QString::fromUtf8(result["e"].get<std::string>().c_str()));
            }

            const auto result_count = result_dirs.size();
            m_result_dirs.setValue(std::move(result_dirs));
            m_result_files.setValue(std::move(result_files));
            m_result_exts.setValue(std::move(result_exts));
            m_result_count.setValue(result_count);
        });
}

} // namespace vca
