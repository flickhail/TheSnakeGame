#pragma once

#include <string>

#ifdef SNK_DEBUG
    #define SNK_LOG_INFO(msg) Log::Info(msg)
#else 
    #define SNK_LOG_INFO(msg)
#endif // SNK_DEBUG

class Log
{
public:
    static void Init();
    static void Info(const std::string& msg);
    static void Warn(const std::string& msg);
    static void Error(const std::string& msg);
};