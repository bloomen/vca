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

// Represents an error. Can be constructed from error code and/or string.
class Error
{
public:
    enum
    {
        CODE_UNSET = std::numeric_limits<int>::lowest()
    };

    explicit Error() = default;
    explicit Error(const int code)
        : _code{code}
    {
    }
    explicit Error(const char* message)
        : _message{message}
    {
    }
    explicit Error(const std::string& message)
        : _message{message}
    {
    }
    explicit Error(std::string&& message)
        : _message{std::move(message)}
    {
    }
    explicit Error(const int code, const char* message)
        : _code{code}
        , _message{message}
    {
    }
    explicit Error(const int code, const std::string& message)
        : _code{code}
        , _message{message}
    {
    }
    explicit Error(const int code, std::string&& message)
        : _code{code}
        , _message{std::move(message)}
    {
    }

    // Returns the error code (may be defaulted to `CODE_UNSET`)
    int
    code() const
    {
        return _code;
    }

    // Returns the error message (may be empty)
    const std::string&
    message() const
    {
        return _message;
    }

    // Returns a textual representation of the error
    std::string
    repr() const
    {
        std::string str = "Error";
        if (_code != CODE_UNSET)
        {
            str += ": [" + std::to_string(_code) + "]";
        }
        if (!_message.empty())
        {
            str += _code == CODE_UNSET ? ": " : " ";
            str += _message;
        }
        return str;
    }

    // Appends to the error message
    template <typename T>
    Error&
    operator<<(T&& data) &
    {
        std::ostringstream os;
        os << std::forward<T>(data);
        _message += os.str();
        return *this;
    }

    // Appends to the error message
    template <typename T>
    Error&&
    operator<<(T&& data) &&
    {
        return std::move(operator<<(std::forward<T>(data)));
    }

private:
    int _code = CODE_UNSET;
    std::string _message;
};

std::ostream& operator<<(std::ostream& os, const Error& e);

// Represents a custom error that can carry any type, e.g. a user-defined enum
template <typename ObjectType>
struct CustomErr
{
    using object_type = ObjectType;

    CustomErr(const object_type& obj)
        : obj{obj}
    {
    }
    CustomErr(object_type&& obj)
        : obj{std::move(obj)}
    {
    }

    object_type obj;
};

class ResultException : public std::runtime_error
{
public:
    explicit ResultException(const std::string& message)
        : std::runtime_error{message}
    {
    }
};

template <typename ValueType, typename ErrorType = Error>
class VCA_NODISCARD Result;

// Allows you to store a ValueType in the success case and an ErrorType in the
// failure case. `Result` can be constructed from either ValueType or ErrorType.
template <typename ValueType, typename ErrorType>
class VCA_NODISCARD Result
{
public:
    using value_type = ValueType;
    using error_type = ErrorType;

    static_assert(!std::is_same_v<value_type, CustomErr<error_type>>,
                  "ValueType cannot be ErrorType");

    Result(const value_type& value)
        : m_result{value}
    {
    }
    Result(value_type&& value)
        : m_result{std::move(value)}
    {
    }
    Result(const CustomErr<error_type>& error)
        : m_result{error.obj}
    {
    }
    Result(CustomErr<error_type>&& error)
        : m_result{std::move(error.obj)}
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

    template <typename ValueFunctor, typename ErrorFunctor>
    void
    match_view(ValueFunctor&& value_functor,
               ErrorFunctor&& error_functor) const&
    {
        if (ok())
        {
            std::forward<ValueFunctor>(value_functor)(value_view());
        }
        else
        {
            std::forward<ErrorFunctor>(error_functor)(error_view());
        }
    }

    template <typename ValueFunctor, typename ErrorFunctor>
    void
    match(ValueFunctor&& value_functor, ErrorFunctor&& error_functor)
    {
        if (ok())
        {
            std::forward<ValueFunctor>(value_functor)(value());
        }
        else
        {
            std::forward<ErrorFunctor>(error_functor)(error());
        }
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
            throw ResultException{"ResultException: CustomErr"};
        }
    }

    std::variant<value_type, error_type> m_result;
};

template <typename ValueType>
class VCA_NODISCARD Result<ValueType, Error>
{
public:
    using value_type = ValueType;
    using error_type = Error;

    static_assert(!std::is_same_v<value_type, error_type>,
                  "ValueType cannot be ErrorType");

    Result(const value_type& value)
        : m_result{value}
    {
    }
    Result(value_type&& value)
        : m_result{std::move(value)}
    {
    }
    Result(const error_type& error)
        : m_result{error}
    {
    }
    Result(error_type&& error)
        : m_result{std::move(error)}
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

    template <typename ValueFunctor, typename ErrorFunctor>
    void
    match_view(ValueFunctor&& value_functor,
               ErrorFunctor&& error_functor) const&
    {
        if (ok())
        {
            std::forward<ValueFunctor>(value_functor)(value_view());
        }
        else
        {
            std::forward<ErrorFunctor>(error_functor)(error_view());
        }
    }

    template <typename ValueFunctor, typename ErrorFunctor>
    void
    match(ValueFunctor&& value_functor, ErrorFunctor&& error_functor)
    {
        if (ok())
        {
            std::forward<ValueFunctor>(value_functor)(value());
        }
        else
        {
            std::forward<ErrorFunctor>(error_functor)(error());
        }
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
            throw ResultException{"ResultException: " + error_view().repr()};
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
    Result(const CustomErr<error_type>& error)
        : m_result{error.obj}
    {
    }
    Result(CustomErr<error_type>&& error)
        : m_result{std::move(error.obj)}
    {
    }

    bool
    ok() const
    {
        return std::holds_alternative<std::monostate>(m_result);
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

    template <typename ValueFunctor, typename ErrorFunctor>
    void
    match_view(ValueFunctor&& value_functor,
               ErrorFunctor&& error_functor) const&
    {
        if (ok())
        {
            std::forward<ValueFunctor>(value_functor)();
        }
        else
        {
            std::forward<ErrorFunctor>(error_functor)(error_view());
        }
    }

    template <typename ValueFunctor, typename ErrorFunctor>
    void
    match(ValueFunctor&& value_functor, ErrorFunctor&& error_functor)
    {
        if (ok())
        {
            std::forward<ValueFunctor>(value_functor)();
        }
        else
        {
            std::forward<ErrorFunctor>(error_functor)(error());
        }
    }

    void
    unwrap()
    {
        check_ok();
    }

private:
    void
    check_ok() const
    {
        if (!ok())
        {
            throw ResultException{"ResultException: CustomErr"};
        }
    }

    std::variant<std::monostate, error_type> m_result;
};

template <>
class VCA_NODISCARD Result<void, Error>
{
public:
    using error_type = Error;

    Result()
        : m_result{std::monostate{}}
    {
    }
    Result(const error_type& error)
        : m_result{error}
    {
    }
    Result(error_type&& error)
        : m_result{std::move(error)}
    {
    }

    bool
    ok() const
    {
        return std::holds_alternative<std::monostate>(m_result);
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

    template <typename ValueFunctor, typename ErrorFunctor>
    void
    match_view(ValueFunctor&& value_functor,
               ErrorFunctor&& error_functor) const&
    {
        if (ok())
        {
            std::forward<ValueFunctor>(value_functor)();
        }
        else
        {
            std::forward<ErrorFunctor>(error_functor)(error_view());
        }
    }

    template <typename ValueFunctor, typename ErrorFunctor>
    void
    match(ValueFunctor&& value_functor, ErrorFunctor&& error_functor)
    {
        if (ok())
        {
            std::forward<ValueFunctor>(value_functor)();
        }
        else
        {
            std::forward<ErrorFunctor>(error_functor)(error());
        }
    }

    void
    unwrap()
    {
        check_ok();
    }

private:
    void
    check_ok() const
    {
        if (!ok())
        {
            throw ResultException{"ResultException: " + error_view().repr()};
        }
    }

    std::variant<std::monostate, error_type> m_result;
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
        return vca::Error                                                      \
        {                                                                      \
            VCA_SOURCE_LOCATION.repr() + ": Test failed: '" + #condition +     \
                "' "                                                           \
        }
