#include "utils.h"

namespace vca
{

VcaError::VcaError(const std::string& message)
    : std::runtime_error{"VcaError: " + message}
{
}

} // namespace vca
