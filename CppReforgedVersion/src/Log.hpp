#pragma once

#ifdef SNK_DEBUG

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <string>
#include <memory>

class Log
{
public:
    using LoggerPtr = std::shared_ptr<spdlog::logger>;

    static void Init(const std::string& logName, const std::string& logFile);
    static LoggerPtr GetLogger();

private:
    static LoggerPtr m_mainLogger;
};

// --------------------------------------------------------
// Debug and release usage of logging system through macros
// for optimization purposes
// --------------------------------------------------------

#define SNK_LOG_INIT(loggerName, logFilePath)   ::Log::Init(loggerName, logFilePath)
#define SNK_LOG_INFO(...)                       ::Log::GetLogger()->info(__VA_ARGS__)
#define SNK_LOG_ERROR(...)                      ::Log::GetLogger()->error(__VA_ARGS__)

#else // SNK_DEBUG -----------------

#define SNK_LOG_INIT(loggerName, logFilePath)
#define SNK_LOG_INFO(...)
#define SNK_LOG_ERROR(...)

#endif // SNK_DEBUG ----------------