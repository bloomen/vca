#pragma once

#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>

namespace gem
{

class Error
{
public:
    static constexpr int CODE_UNSET = std::numeric_limits<int>::max();

    explicit Error() = default;
    explicit Error(const std::string& message)
        : m_message{message}
    {
    }
    explicit Error(std::string&& message)
        : m_message{std::move(message)}
    {
    }
    explicit Error(const int code)
        : m_code{code}
    {
    }
    explicit Error(const std::string& message, const int code)
        : m_message{message}
        , m_code{code}
    {
    }
    explicit Error(std::string&& message, const int code)
        : m_message{std::move(message)}
        , m_code{code}
    {
    }

    const std::string&
    message() const
    {
        return m_message;
    }

    int
    code() const
    {
        return m_code;
    }

    std::string
    repr() const
    {
        std::string str = "Error: ";
        str += m_message;
        if (m_code != CODE_UNSET)
        {
            str += " (" + std::to_string(m_code) + ")";
        }
        return str;
    }

private:
    std::string m_message;
    int m_code = CODE_UNSET;
};

class ResultError : public std::runtime_error
{
public:
    explicit ResultError(const std::string& message)
        : std::runtime_error{message}
    {
    }
};

template <typename Value>
class Result
{
public:
    static_assert(!std::is_same_v<Value, Error>, "Value cannot be Error");

    Result(const Value& value)
        : m_result{value}
    {
    }
    Result(Value&& value)
        : m_result{std::move(value)}
    {
    }
    Result(const Error& error)
        : m_result{error}
    {
    }
    Result(Error&& error)
        : m_result{std::move(error)}
    {
    }

    bool
    ok() const
    {
        return std::holds_alternative<Value>(m_result);
    }

    const Error&
    error() const
    {
        return std::get<Error>(m_result);
    }

    const Value&
    value() const
    {
        return std::get<Value>(m_result);
    }

    template <typename ValueFunctor, typename ErrorFunctor>
    void
    match(ValueFunctor&& value_functor, ErrorFunctor&& error_functor) const&
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

    template <typename ValueFunctor, typename ErrorFunctor>
    void
    match(ValueFunctor&& value_functor, ErrorFunctor&& error_functor) &&
    {
        if (ok())
        {
            std::forward<ValueFunctor>(value_functor)(
                std::get<Value>(std::move(m_result)));
        }
        else
        {
            std::forward<ErrorFunctor>(error_functor)(error());
        }
    }

    const Value&
    unwrap() const&
    {
        if (!ok())
        {
            throw ResultError{error().repr()};
        }
        return value();
    }

    Value&&
    unwrap() &&
    {
        if (!ok())
        {
            throw ResultError{"Result: " + error().repr()};
        }
        return std::get<Value>(std::move(m_result));
    }

private:
    std::variant<Value, Error> m_result;
};

template <>
class Result<void>
{
public:
    Result()
        : m_result{std::monostate{}}
    {
    }
    Result(const Error& error)
        : m_result{error}
    {
    }
    Result(Error&& error)
        : m_result{std::move(error)}
    {
    }

    bool
    ok() const
    {
        return std::holds_alternative<std::monostate>(m_result);
    }

    const Error&
    error() const
    {
        return std::get<Error>(m_result);
    }

    template <typename ValueFunctor, typename ErrorFunctor>
    void
    match(ValueFunctor&& value_functor, ErrorFunctor&& error_functor) const
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
    unwrap() const
    {
        if (!ok())
        {
            throw ResultError{error().repr()};
        }
    }

private:
    std::variant<std::monostate, Error> m_result;
};

} // namespace gem
