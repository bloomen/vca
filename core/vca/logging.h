#pragma once

#include <sstream>

#include "filesystem.h"
#include "utils.h"

namespace vca
{

class Logger
{
public:
    enum class Level
    {
        Debug,
        Info,
        Warn,
        Error,
    };

    explicit Logger(const Level level, const char* file, const int line);

    VCA_DELETE_COPY(Logger)
    VCA_DEFAULT_MOVE(Logger)

    ~Logger();

    template <typename T>
    Logger&
    operator<<(T&& data) &
    {
        m_os << std::forward<T>(data);
        return *this;
    }

    template <typename T>
    Logger&&
    operator<<(T&& data) &&
    {
        return std::move(operator<<(std::forward<T>(data)));
    }

private:
    Level m_level;
    std::ostringstream m_os;
};

void
init_logging(const fs::path& filename);

void
set_log_level(Logger::Level level);

bool
is_log_level_active(Logger::Level level);

} // namespace vca

#define VCA_DEBUG                                                              \
    if (!vca::is_log_level_active(vca::Logger::Level::Debug))                  \
        ;                                                                      \
    else                                                                       \
        vca::Logger                                                            \
        {                                                                      \
            vca::Logger::Level::Debug, __FILE__, __LINE__                      \
        }

#define VCA_INFO                                                               \
    if (!vca::is_log_level_active(vca::Logger::Level::Info))                   \
        ;                                                                      \
    else                                                                       \
        vca::Logger                                                            \
        {                                                                      \
            vca::Logger::Level::Info, __FILE__, __LINE__                       \
        }

#define VCA_WARN                                                               \
    if (!vca::is_log_level_active(vca::Logger::Level::Warn))                   \
        ;                                                                      \
    else                                                                       \
        vca::Logger                                                            \
        {                                                                      \
            vca::Logger::Level::Warn, __FILE__, __LINE__                       \
        }

#define VCA_ERROR                                                              \
    if (!vca::is_log_level_active(vca::Logger::Level::Error))                  \
        ;                                                                      \
    else                                                                       \
        vca::Logger                                                            \
        {                                                                      \
            vca::Logger::Level::Error, __FILE__, __LINE__                      \
        }
