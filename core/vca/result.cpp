#include "result.h"

namespace vca
{

std::ostream&
operator<<(std::ostream& os, const Error<ErrInfo>& e)
{
    os << e.get().repr();
    return os;
}

} // namespace vca
