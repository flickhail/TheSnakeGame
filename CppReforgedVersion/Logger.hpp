#pragma once

#include <string>
#include <iostream>
#include <fstream>

class ILogPrintable
{
public:
    virtual std::string ToString() const = 0;
};

class Logger
{
public:
    virtual ~Logger() {};
    virtual void Log(const std::string& str) = 0;
    virtual void Log(const ILogPrintable& obj) = 0;
};

class FileLogger : public Logger
{
public:
    FileLogger(const std::string& filePath)
    {
        fileStream = new std::ofstream{ filePath, std::ios_base::app };
    }

    ~FileLogger() override
    {
        delete fileStream;
    }

    void Log(const std::string& str) override
    {
        *fileStream << str << std::endl;
    }

    void Log(const ILogPrintable& obj) override
    {
        *fileStream << obj.ToString() << std::endl;
    }

private:
    std::ofstream* fileStream;
};