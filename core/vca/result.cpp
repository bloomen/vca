#include "result.h"

namespace vca
{

std::ostream&
operator<<(std::ostream& os, const Error& e)
{
    os << e.repr();
    return os;
}

} // namespace vca
