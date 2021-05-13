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
                m_result.assignDefault();
                return;
            }
            auto wide_input = vca::narrow_to_wide(value.toUtf8().data());
            vca::trim(wide_input);

            std::list<vca::String> values;
            vca::split(values, wide_input, vca::space_char());

            if (!values.empty())
            {
                VCA_INFO << "Empty search string";
            }

            const auto file_contents = vca::FileContents::fromSearch(values);

            VCA_INFO << "Searching ...";
            vca::Timer timer;
            const auto result = m_user_db->search(file_contents);
            VCA_INFO << "Search took: " << timer.us() << " us";

            QString output;
            for (const auto& path : result)
            {
                output += QString::fromUtf8((path.u8string() + "\n").data());
            }

            m_result.setValue(std::move(output));
        });
}

} // namespace vca
