#include "utils.h"

#include <memory>

#include "platform.h"

#ifdef VCA_PLATFORM_UNIX
#include <cxxabi.h>
#endif

namespace vca
{

std::string
demangle_type(const char* const type_name)
{
#ifdef VCA_PLATFORM_UNIX
    int status = -1;
    std::unique_ptr<char, void (*)(void*)> result{
        abi::__cxa_demangle(type_name, NULL, NULL, &status), std::free};
    return status == 0 ? result.get() : type_name;
#else
    return type_name;
#endif
}

VcaError::VcaError(const std::string& message)
    : std::runtime_error{"VcaError: " + message}
{
}

} // namespace vca
