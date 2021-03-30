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
    auto logger = spdlog::get(logger_name);
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

} // namespace

Logger::Logger(const Level level, const char* const file, const int line)
    : m_level{level}
{
    m_os << "[" << file << ":" << line << "] ";
}

Logger::~Logger()
{
    const auto message = m_os.str();
    log_message("console_logger", m_level, message);
    log_message("file_logger", m_level, message);
}

void
init_logging(const fs::path& filename)
{
    fs::create_directories(filename.parent_path());
    spdlog::set_pattern("%l [%Y:%m:%dT%H:%M:%S.%f] [%t] %v");
    spdlog::stdout_logger_mt("console_logger");
    spdlog::rotating_logger_mt(
        "file_logger", filename.u8string(), 1048576 * 5, 3);
    spdlog::set_level(spdlog::level::debug);
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
