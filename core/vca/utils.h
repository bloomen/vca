#pragma once

#include <cassert>
#include <iostream>
#include <sstream>
#include <type_traits>

#define VCA_NODISCARD [[nodiscard]]

#define VCA_DELETE_COPY(class_name)                                            \
    class_name(const class_name&) = delete;                                    \
    class_name& operator=(const class_name&) = delete;

#define VCA_DELETE_MOVE(class_name)                                            \
    class_name(class_name&&) = delete;                                         \
    class_name& operator=(class_name&&) = delete;

#define VCA_DEFAULT_COPY(class_name)                                           \
    class_name(const class_name&) = default;                                   \
    class_name& operator=(const class_name&) = default;

#define VCA_DEFAULT_MOVE(class_name)                                           \
    class_name(class_name&&) = default;                                        \
    class_name& operator=(class_name&&) = default;

#define VCA_CHECK(condition)                                                   \
    if (!(condition))                                                          \
    vca::VcaErrorThrower{} << __FILE__ << ":" << __LINE__ << ":" << __func__   \
                           << ": Check failed: '" << #condition << "' "

#define VCA_ASSERT(x) assert(x)

namespace vca
{

constexpr auto ignore_op = [](auto&&...) {};

template <typename Derived, typename Base>
VCA_NODISCARD Derived*
downcast(Base* const base)
{
    static_assert(std::is_base_of_v<Base, Derived>);
    VCA_ASSERT(dynamic_cast<Derived*>(base) == base);
    return static_cast<Derived*>(base);
}

std::string
demangle_type(const char* type_name);

class VcaError : public std::runtime_error
{
public:
    explicit VcaError(const std::string& message);
};

class VcaErrorThrower
{
public:
    explicit VcaErrorThrower() = default;

    VCA_DEFAULT_COPY(VcaErrorThrower)
    VCA_DEFAULT_MOVE(VcaErrorThrower)

    ~VcaErrorThrower() noexcept(false)
    {
        throw VcaError{m_msg};
    }

    template <typename T>
    VcaErrorThrower&
    operator<<(T&& data) &
    {
        std::ostringstream os;
        os << std::forward<T>(data);
        m_msg += os.str();
        return *this;
    }

    template <typename T>
    VcaErrorThrower&&
    operator<<(T&& data) &&
    {
        return std::move(operator<<(std::forward<T>(data)));
    }

private:
    std::string m_msg;
};

void
to_lower(std::string& str);

} // namespace vca
