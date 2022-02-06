#include "logging.h"

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

namespace vca
{

namespace
{

std::atomic<Logger::Level> g_log_level{Logger::Level::Info};

void
log_message(const char* const logger_name,
            const Logger::Level level,
            const std::string& message)
{
    if (auto logger = spdlog::get(logger_name))
    {
        switch (level)
        {
        case Logger::Level::Debug:
            logger->debug(message);
            break;
        case Logger::Level::Info:
            logger->info(message);
            break;
        case Logger::Level::Warn:
            logger->warn(message);
            break;
        case Logger::Level::Error:
            logger->error(message);
            break;
        }
    }
}

} // namespace

Logger::Logger(const Level level, const char* const file, const int line)
    : m_level{level}
{
    m_os << "[" << relative(Path{file}, Path{VCA_SOURCE_DIR}).to_narrow() << ":"
         << line << "] ";
}

Logger::~Logger()
{
    const auto message = m_os.str();
    log_message("vca_logging_console_logger", m_level, message);
    log_message("vca_logging_file_logger", m_level, message);
}

void
init_logging(const std::optional<Path>& filename)
{
    const auto level = spdlog::level::debug;
    const auto pattern = "%l [%Y:%m:%dT%H:%M:%S.%f] [%t] %v";
    auto console_logger =
        spdlog::stdout_logger_mt("vca_logging_console_logger");
    console_logger->set_level(level);
    console_logger->set_pattern(pattern);
    if (filename)
    {
        create_directories(filename->parent());
        auto file_logger = spdlog::rotating_logger_mt(
            "vca_logging_file_logger", filename->to_narrow(), 1048576 * 5, 3);
        file_logger->set_level(level);
        file_logger->set_pattern(pattern);
    }
}

void
set_log_level(const Logger::Level level)
{
    g_log_level = level;
}

bool
is_log_level_active(const Logger::Level level)
{
    return static_cast<int>(g_log_level.load()) <= static_cast<int>(level);
}

} // namespace vca
