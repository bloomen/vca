#pragma once

#include <type_traits>

#ifndef VCA_ASSERT
#ifdef NDEBUG
#define VCA_ASSERT(x)
#else
#include <cassert>
#define VCA_ASSERT(x) assert(x)
#endif
#endif

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

namespace vca
{

const auto ignore_op = [](auto&&...) {};

template <typename Derived, typename Base>
VCA_NODISCARD Derived*
downcast(Base* const base)
{
    static_assert(std::is_base_of_v<Base, Derived>);
    VCA_ASSERT(dynamic_cast<Derived*>(base) == base);
    return static_cast<Derived*>(base);
}

} // namespace vca
