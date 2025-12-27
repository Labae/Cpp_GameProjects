#pragma once

#include <string>
#include <iostream>
#include <Windows.h>

namespace GameLibrary
{
    enum class LogLevel
    {
        Info,
        Warning,
        Error
    };

    class Logger
    {
    public:
        static void Init()
        {
            if (s_initialized) return;

            AllocConsole();
            FILE* fp;
            freopen_s(&fp, "CONOUT$", "w", stdout);
            freopen_s(&fp, "CONOUT$", "w", stderr);

            // 콘솔 제목 설정
            SetConsoleTitleW(L"Snake Game - Debug Console");

            s_initialized = true;
        }

        static void Shutdown()
        {
            if (s_initialized)
            {
                FreeConsole();
                s_initialized = false;
            }
        }

        static void Log(LogLevel level, const std::string& message)
        {
            std::string prefix;
            WORD color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

            switch (level)
            {
            case LogLevel::Info:
                prefix = "[INFO] ";
                color = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
                break;
            case LogLevel::Warning:
                prefix = "[WARNING] ";
                color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
                break;
            case LogLevel::Error:
                prefix = "[ERROR] ";
                color = FOREGROUND_RED | FOREGROUND_INTENSITY;
                break;
            }

            std::string output = prefix + message + "\n";

            // 콘솔 출력 (색상 적용)
            if (s_initialized)
            {
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, color);
                std::cout << output;
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }

            // Visual Studio 출력 창
            OutputDebugStringA(output.c_str());
        }

        static void Log(LogLevel level, const std::wstring& message)
        {
            std::wstring prefix;
            WORD color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

            switch (level)
            {
            case LogLevel::Info:
                prefix = L"[INFO] ";
                color = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
                break;
            case LogLevel::Warning:
                prefix = L"[WARNING] ";
                color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
                break;
            case LogLevel::Error:
                prefix = L"[ERROR] ";
                color = FOREGROUND_RED | FOREGROUND_INTENSITY;
                break;
            }

            std::wstring output = prefix + message + L"\n";

            // 콘솔 출력 (색상 적용)
            if (s_initialized)
            {
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, color);
                std::wcout << output;
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }

            // Visual Studio 출력 창
            OutputDebugStringW(output.c_str());
        }

        static void Info(const std::string& message) { Log(LogLevel::Info, message); }
        static void Warning(const std::string& message) { Log(LogLevel::Warning, message); }
        static void Error(const std::string& message) { Log(LogLevel::Error, message); }

        static void Info(const std::wstring& message) { Log(LogLevel::Info, message); }
        static void Warning(const std::wstring& message) { Log(LogLevel::Warning, message); }
        static void Error(const std::wstring& message) { Log(LogLevel::Error, message); }

    private:
        inline static bool s_initialized = false;
    };
}