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
class CustomErr
{
public:
    using object_type = ObjectType;

    static_assert(!std::is_convertible<object_type, Error>::value,
                  "ObjectType cannot be convertible to Error");

    explicit CustomErr(const object_type& obj)
        : _obj{obj}
    {
    }
    explicit CustomErr(object_type&& obj)
        : _obj{std::move(obj)}
    {
    }

    const object_type&
    get() const
    {
        return _obj;
    }

    object_type&
    get()
    {
        return _obj;
    }

    std::string
    repr() const
    {
        return "CustomErr";
    }

private:
    object_type _obj;
};

class ResultError : public std::runtime_error
{
public:
    explicit ResultError(const std::string& message)
        : std::runtime_error{message}
    {
    }
};

// Allows you to store a ValueType in the success case and an ErrorType in the
// failure case. `Result` can be constructed from either ValueType or ErrorType.
template <typename ValueType, typename ErrorType = Error>
class VCA_NODISCARD Result
{
public:
    using value_type = ValueType;
    using error_type = ErrorType;

    static_assert(!std::is_same_v<value_type, Error>, "Value cannot be Error");

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

    const error_type&
    error_view() const
    {
        return std::get<error_type>(m_result);
    }

    error_type&&
    error()
    {
        return std::get<error_type>(std::move(m_result));
    }

    const value_type&
    value_view() const
    {
        return std::get<value_type>(m_result);
    }

    value_type&&
    value()
    {
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
            throw ResultError{"Result: " + error_view().repr()};
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

    const error_type&
    error_view() const
    {
        return std::get<error_type>(m_result);
    }

    error_type&&
    error()
    {
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
            throw ResultError{"Result: " + error_view().repr()};
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
    owner = FS_PRIVATE_RESULT_JOIN(tmp__result__, __LINE__).value()

// Convenience macro to make error handling more ergonomic.
// Returns an error object if `expr` returns a failed result, otherwise
// continues execution as normal.
// Use this macro when an error should be forwarded up the call stack.
#define CHECK_RESULT(expr)                                                     \
    auto&& FS_PRIVATE_RESULT_JOIN(tmp__result__, __LINE__) = expr;             \
    if (FS_PRIVATE_RESULT_JOIN(tmp__result__, __LINE__).bad())                 \
    return FS_PRIVATE_RESULT_JOIN(tmp__result__, __LINE__).error()

// Tests if `condition` is true and returns an `Error` object if not.
// A custom message can be appended using the << operator following the macro
// call.
#define TEST_CONDITION(condition)                                              \
    if (!(condition))                                                          \
        return vca::Error                                                      \
        {                                                                      \
            FS_PRIVATE_RESULT_LOCATION + ": Test failed: '" + #condition +     \
                "' "                                                           \
        }

// Private macro - not to be used directly by a client. Joins two symbols to a
// single symbol
#define FS_PRIVATE_RESULT_JOIN(symbol1, symbol2)                               \
    FS_PRIVATE_RESULT_DO_JOIN(symbol1, symbol2)
#define FS_PRIVATE_RESULT_DO_JOIN(symbol1, symbol2) symbol1##symbol2

// Private macro - not to be used directly by a client. Provides the location of
// the macro as a string
#define FS_PRIVATE_RESULT_LOCATION                                             \
    std::string{__FILE__} + ":" + std::to_string(__LINE__) + ":" + std::string \
    {                                                                          \
        __func__                                                               \
    }
