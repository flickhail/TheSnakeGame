#include "Log.hpp"

#ifdef SNK_DEBUG

Log::LoggerPtr Log::m_mainLogger;

void Log::Init(const std::string& logName, const std::string& logFile)
{
    m_mainLogger = spdlog::basic_logger_mt(logName, logFile);
}

Log::LoggerPtr Log::GetLogger()
{
    return Log::m_mainLogger;
}

#endif // SNK_DEBUG ------------