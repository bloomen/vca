#include "SearchInput.h"

#include <vca/logging.h>
#include <vca/time.h>

namespace app
{
namespace handler
{

SearchInput::SearchInput()
    : Handler(QString{})
{
}

QString
SearchInput::endpoint() const
{
    return "/search/input";
}

void
SearchInput::set_user_db(const vca::UserDb& user_db)
{
    m_user_db = &user_db;
}

void
SearchInput::valueChanged(const QVariant& value, const bool input)
{
    VCA_ASSERT(m_user_db);
    if (!input)
    {
        return;
    }
    auto wide_input = vca::narrow_to_wide(value.toByteArray().data());
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

    set("/search/output", output);
}

} // namespace handler
} // namespace app
