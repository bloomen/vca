#pragma once

#include <iosfwd>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>

#include "utils.h"

namespace vca
{

class ErrInfo
{
public:
    enum
    {
        CODE_UNSET = std::numeric_limits<int>::lowest()
    };

    explicit ErrInfo() = default;

    int
    code() const
    {
        return m_code;
    }

    int&
    code()
    {
        return m_code;
    }

    const std::string&
    message() const
    {
        return m_message;
    }

    std::string&
    message()
    {
        return m_message;
    }

    std::string
    repr() const
    {
        std::string str = "ErrInfo";
        if (m_code != CODE_UNSET)
        {
            str += ": [" + std::to_string(m_code) + "]";
        }
        if (!m_message.empty())
        {
            str += m_code == CODE_UNSET ? ": " : " ";
            str += m_message;
        }
        return str;
    }

private:
    int m_code = CODE_UNSET;
    std::string m_message;
};

template <typename ErrorType>
class Error
{
public:
    explicit Error(const ErrorType& err)
        : m_err{err}
    {
    }
    explicit Error(ErrorType&& err)
        : m_err{std::move(err)}
    {
    }

    const ErrorType&
    get() const
    {
        return m_err;
    }

    ErrorType&
    get()
    {
        return m_err;
    }

private:
    ErrorType m_err;
};

template <>
class Error<ErrInfo>
{
public:
    explicit Error() = default;

    const ErrInfo&
    get() const
    {
        return m_info;
    }

    ErrInfo&
    get()
    {
        return m_info;
    }

    Error&
    code(const int c) &
    {
        m_info.code() = c;
        return *this;
    }

    Error&&
    code(const int c) &&
    {
        return std::move(code(c));
    }

    template <typename T>
    Error&
    operator<<(T&& data) &
    {
        std::ostringstream os;
        os << std::forward<T>(data);
        m_info.message() += os.str();
        return *this;
    }

    template <typename T>
    Error&&
    operator<<(T&& data) &&
    {
        return std::move(operator<<(std::forward<T>(data)));
    }

private:
    ErrInfo m_info;
};

std::ostream& operator<<(std::ostream& os, const Error<ErrInfo>& e);

template <typename Arg>
auto
error(Arg&& arg)
{
    return Error<std::remove_reference_t<Arg>>{std::forward<Arg>(arg)};
}

inline auto
error()
{
    return Error<ErrInfo>{};
}

class BadResultException : public std::runtime_error
{
public:
    explicit BadResultException(const std::string& message)
        : std::runtime_error{message}
    {
    }
};

template <typename ValueType, typename ErrorType = ErrInfo>
class VCA_NODISCARD Result
{
public:
    using value_type = ValueType;
    using error_type = ErrorType;

    static_assert(!std::is_same_v<value_type, Error<error_type>>,
                  "ValueType cannot be Error<ErrorType>");

    Result(const value_type& value)
        : m_result{value}
    {
    }
    Result(value_type&& value)
        : m_result{std::move(value)}
    {
    }
    Result(const Error<error_type>& error)
        : m_result{error.get()}
    {
    }
    Result(Error<error_type>&& error)
        : m_result{std::move(error.get())}
    {
    }

    bool
    ok() const
    {
        return std::holds_alternative<value_type>(m_result);
    }

    bool
    bad() const
    {
        return !ok();
    }

    const error_type&
    error_view() const
    {
        VCA_ASSERT(bad());
        return std::get<error_type>(m_result);
    }

    error_type&&
    error()
    {
        VCA_ASSERT(bad());
        return std::get<error_type>(std::move(m_result));
    }

    const value_type&
    value_view() const
    {
        VCA_ASSERT(ok());
        return std::get<value_type>(m_result);
    }

    value_type&&
    value()
    {
        VCA_ASSERT(ok());
        return std::get<value_type>(std::move(m_result));
    }

    const value_type&
    unwrap_view() const
    {
        check_ok();
        return value_view();
    }

    value_type&&
    unwrap()
    {
        check_ok();
        return value();
    }

private:
    void
    check_ok() const
    {
        if (!ok())
        {
            if constexpr (std::is_same_v<error_type, ErrInfo>)
            {
                throw BadResultException{"BadResultException: " +
                                         error_view().get().repr()};
            }
            else
            {
                throw BadResultException{"BadResultException"};
            }
        }
    }

    std::variant<value_type, error_type> m_result;
};

template <typename ErrorType>
class VCA_NODISCARD Result<void, ErrorType>
{
public:
    using error_type = ErrorType;

    Result()
        : m_result{std::monostate{}}
    {
    }
    Result(const Error<error_type>& error)
        : m_result{error.get()}
    {
    }
    Result(Error<error_type>&& error)
        : m_result{std::move(error.get())}
    {
    }

    bool
    ok() const
    {
        return m_result.ok();
    }

    bool
    bad() const
    {
        return m_result.bad();
    }

    const error_type&
    error_view() const
    {
        return m_result.error_view();
    }

    error_type&&
    error()
    {
        return m_result.error();
    }

    void
    unwrap()
    {
        m_result.unwrap();
    }

private:
    Result<std::monostate, error_type> m_result;
};

} // namespace vca

// Convenience macro to make error handling more ergonomic.
// Assigns a value to `owner` if `expr` returns a successful result,
// otherwise returns an error object.
// Use this macro when an error should be forwarded up the call stack.
#define TAKE_VALUE(owner, expr)                                                \
    CHECK_RESULT(expr);                                                        \
    owner = VCA_SYMBOL_JOIN(tmp__result__, __LINE__).value()

// Convenience macro to make error handling more ergonomic.
// Returns an error object if `expr` returns a failed result, otherwise
// continues execution as normal.
// Use this macro when an error should be forwarded up the call stack.
#define CHECK_RESULT(expr)                                                     \
    auto&& VCA_SYMBOL_JOIN(tmp__result__, __LINE__) = expr;                    \
    if (VCA_SYMBOL_JOIN(tmp__result__, __LINE__).bad())                        \
    return VCA_SYMBOL_JOIN(tmp__result__, __LINE__).error()

// Tests if `condition` is true and returns an `Error` object if not.
// A custom message can be appended using the << operator following the macro
// call.
#define TEST_CONDITION(condition)                                              \
    if (!(condition))                                                          \
    return vca::error() << VCA_SOURCE_LOCATION.repr() << ": Test failed: '"    \
                        << #condition << "' "
