#pragma once
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <chrono>
#include <ctime>

namespace msg
{
    namespace detail 
    {
        inline FILE* log_file = fopen("log.txt", "w");
        template<typename ... Args>
        inline std::string format(const char* const fmt, Args ... args)
        {
            int sprint_len = std::snprintf(nullptr, 0, fmt, args ...);
            char* formatted = new char[sprint_len];
            std::snprintf(formatted, static_cast<std::size_t>(sprint_len), fmt, args ...);
            return std::string(std::move(formatted));
        }

        /// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
        inline const std::string current_date_time(void) 
        {
            std::time_t     now = std::time(0);
            std::tm  tstruct;
            char buf[80];
            tstruct = *std::localtime(&now);
            strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
            return buf;
        }
    }

    template<typename ... Args>
    inline void info(const char* const fmt, Args ... args)
    {
        std::stringstream ss;

        ss << "[ " << detail::current_date_time() << "]-INFO: ";
        ss << detail::format(fmt, args ...) << std::endl;
        fwrite(ss.str().c_str(), sizeof(char), ss.str().size(), detail::log_file);
        fflush(detail::log_file);
    }

    template<typename ... Args>
    inline void warn(const char* const fmt, Args ... args)
    {
        std::stringstream ss;
        ss << "[ " << detail::current_date_time() << "]-WARNING: ";
        ss << detail::format(fmt, args ...) << std::endl;
        fwrite(ss.str().c_str(), sizeof(char), ss.str().size(), detail::log_file);
        fflush(detail::log_file);
    }
}