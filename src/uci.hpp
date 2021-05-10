#pragma once

/**
 * @file uci.hpp
 * @brief This header contains classes for spawning a child process via either the WIN32 API or linux system calls
 * 
 */

#ifdef _WIN32 

#include <Windows.h>
#include <stdio.h>

namespace uci 
{

/**
 * @brief The Process class is used to actually get a cross platform way of spawning / piping child processes
 * across OSes
 * 
 */
class Process 
{
public:

    /**
     * @brief Spawn a process using the full path to an executable and an array of string arguments
     * 
     * @param p_name The full path to the executable to spawn
     * @param p_args An array of string arguments for the process
     * @return Process The spawned process 
     */
    static Process spawn(char* p_name, char* p_args[]);

    /**
     * @brief Write a string of text to the stdin of the child process
     * 
     * @param data The buffer to write to stdin
     * @param len The number of bytes to write from the data parameter
     * @return true If the operation succeeded
     * @return false If the operation failed
     */
    bool write(char* data, size_t len);

    /**
     * @brief Read bytes from the child processes stdout until EOF and return the resulting
     * byte array
     * 
     * @return char* The read data from the child pipe
     */
    char* read();

private:
    /// The writable handle to our child processes stdin
    HANDLE m_stdin = NULL;

    /// The readable handle to our child processes stdout
    HANDLE m_stdout = NULL;
};
}

#endif //_WIN32