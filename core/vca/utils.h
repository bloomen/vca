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

#define VCA_SYMBOL_JOIN(symbol1, symbol2) VCA_SYMBOL_DO_JOIN(symbol1, symbol2)
#define VCA_SYMBOL_DO_JOIN(symbol1, symbol2) symbol1##symbol2

#define VCA_SOURCE_LOCATION                                                    \
    vca::SourceLocation                                                        \
    {                                                                          \
        __FILE__, __LINE__, __func__                                           \
    }

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

struct SourceLocation
{
    std::string file;
    int line;
    std::string function;

    std::string
    repr() const
    {
        return file + ":" + std::to_string(line) + ":" + function;
    }
};

} // namespace vca
