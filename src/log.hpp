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
        /// The log_file should NOT be written to manually, use the info, warn, and error functions to write correctly formatted strings
        inline FILE* log_file = fopen("log.txt", "w");

        /**
         * @brief Format a string using the same syntax as printf() and return the formatted string
         * 
         * @tparam Args The variadic list of argument types that we will pass to the function
         * @param fmt The format string to insert formatted values into
         * @param args The variadic list of arguments to be formatted
         * @return std::string The formatted result string
         */
        template<typename ... Args>
        inline std::string format(const char* const fmt, Args ... args)
        {
            //Get the required length of the formatted string
            int sprint_len = std::snprintf(nullptr, 0, fmt, args ...) + 1;
            //Allocate a buffer with enough space for the formatted string
            char* formatted = new char[sprint_len];
            std::snprintf(formatted, static_cast<std::size_t>(sprint_len), fmt, args ...); //Format the string and write it to the output buffer
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

    /**
     * @brief Print a formatted output message to the log file with the INFO tag. This function has the same formatting
     * syntax as printf(const char* restrict, ...) from Cstdlib
     * 
     * @tparam Args Variadic argument type list; these are the types of format arguments for the string
     * @param fmt The format string to write to the output file
     * @param args The arguments that will be inserted into the format string
     */
    template<typename ... Args>
    inline void info(const char* const fmt, Args ... args)
    {
        std::stringstream ss;

        ss << "[ " << detail::current_date_time() << "]-INFO: ";
        ss << detail::format(fmt, args ...) << std::endl;
        fwrite(ss.str().c_str(), sizeof(char), ss.str().size(), detail::log_file);
        fflush(detail::log_file);
    }

    /**
     * @brief Print a formatted warning message to the log file, using the same syntax as printf() from C
     * 
     * @tparam Args The variadic list of types of format arguments 
     * @param fmt The format string that args... will insert into
     * @param args The arguments that will be inserted into the format string
     */
    template<typename ... Args>
    inline void warn(const char* const fmt, Args ... args)
    {
        std::stringstream ss;
        ss << "[ " << detail::current_date_time() << "]-WARNING: ";
        ss << detail::format(fmt, args ...) << std::endl;
        fwrite(ss.str().c_str(), sizeof(char), ss.str().size(), detail::log_file);
        fflush(detail::log_file);
    }

    /**
     * @brief Print a formatted error message to the log file using the same format syntax as C's printf()
     * 
     * @tparam Args variadic list of argument types 
     * @param fmt The format string to insert values into
     * @param args The variadic list of arguments to insert into the format string
     */
    template<typename ... Args>
    inline void error(const char* const fmt, Args ... args)
    {
        std::stringstream ss;
        ss << "[ " << detail::current_date_time() << "]-ERROR: ";
        ss << detail::format(fmt, args ...) << std::endl;
        fwrite(ss.str().c_str(), sizeof(char), ss.str().size(), detail::log_file);
        fflush(detail::log_file);
    }
}