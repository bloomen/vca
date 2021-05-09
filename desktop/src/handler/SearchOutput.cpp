#include "SearchOutput.h"

namespace app
{
namespace handler
{

SearchOutput::SearchOutput()
    : Handler(QString{})
{
}

QString
SearchOutput::endpoint() const
{
    return "/search/output";
}

} // namespace handler
} // namespace app
