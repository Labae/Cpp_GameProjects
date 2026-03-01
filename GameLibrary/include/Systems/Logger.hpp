#pragma once

#include <iostream>
#include <string>

namespace GameLibrary
{
    enum class ELogLevel
    {
        Normal,
        Info,
        Warning,
        Error
    };

    class Logger
    {
    public:
        Logger() = default;
        ~Logger() = default;

        Logger(const Logger&) = delete;
        Logger(Logger&&) = delete;
        Logger& operator=(const Logger&) = delete;
        Logger& operator=(Logger&&) = delete;

        static void Log(const ELogLevel level, const std::string& message)
        {
            std::string prefix;

            switch (level)
            {
            case ELogLevel::Normal:
                prefix = "\033[37m[Normal]\033[0m ";
                break;
            case ELogLevel::Info:
                prefix = "\033[32m[INFO]\033[0m ";
                break;
            case ELogLevel::Warning:
                prefix = "\033[33m[WARNING]\033[0m ";
                break;
            case ELogLevel::Error:
                prefix = "\033[31m[ERROR]\033[0m ";
                break;
            }

            std::cout << prefix << message << '\n';
        }

        static void Normal(const std::string& message) { Log(ELogLevel::Normal, message); }
        static void Info(const std::string& message) { Log(ELogLevel::Info, message); }
        static void Warning(const std::string& message) { Log(ELogLevel::Warning, message); }
        static void Error(const std::string& message) { Log(ELogLevel::Error, message); }
    };
} // namespace GameLibrary