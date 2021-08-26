#include "SearchHandler.h"

#include <vca/logging.h>
#include <vca/time.h>

namespace vca
{

SearchHandler::SearchHandler(qtc::Model& model,
                             qtc::ThreadPool& threadPool,
                             qtc::CommandQueue& mainQueue,
                             const vca::UserDb& user_db)
    : Handler{model, threadPool, mainQueue, "/search"}
    , m_user_db{&user_db}
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
            auto wide_input = vca::narrow_to_wide(value.toUtf8().data());
            vca::trim(wide_input);

            std::list<vca::String> values;
            vca::split(values, wide_input, vca::space_char());

            if (values.empty())
            {
                VCA_INFO << "Empty search string";
                return;
            }

            const auto file_contents = vca::FileContents::fromSearch(values);

            VCA_INFO << "Searching ...";
            vca::Timer timer;
            const auto results = m_user_db->search(file_contents);
            VCA_INFO << "Search took: " << timer.us() << " us";

            const int result_count = static_cast<int>(results.size());
            QList<QString> result_dirs;
            QList<QString> result_files;
            QList<QString> result_exts;

            for (const auto& result : results)
            {
                result_dirs.append(
                    QString::fromUtf8(result.dir.u8string().c_str()));
                result_files.append(
                    QString::fromUtf8(result.file.u8string().c_str()));
                result_exts.append(QString::fromUtf8(result.ext.c_str()));
            }

            m_result_dirs.setValue(std::move(result_dirs));
            m_result_files.setValue(std::move(result_files));
            m_result_exts.setValue(std::move(result_exts));
            m_result_count.setValue(result_count);
        });
}

} // namespace vca
