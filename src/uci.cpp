#include "uci.hpp"
#include <malloc.h>

namespace uci
{

Process Process::spawn(char* p_name, char* p_args[]) 
{
    Process self;
    PROCESS_INFORMATION p_info;
    STARTUPINFO start_info; //The windows API startup information
    start_info.hStdInput = self.m_stdin;
    start_info.hStdOutput = self.m_stdout;
    start_info.cb = sizeof(STARTUPINFO);
    start_info.dwFlags |= STARTF_USESTDHANDLES;

    //Create the child process
    if(!CreateProcess(
        NULL,
        static_cast<LPSTR>(p_name),
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &start_info,
        &p_info
    )) 
    {
        exit(-1);
    }

    CloseHandle(p_info.hProcess);
    CloseHandle(p_info.hThread);
    return self;
}

bool Process::write(char* data, size_t len) 
{
    DWORD written; //The number of bytes written to the pipe
    WriteFile(m_stdin, data, static_cast<DWORD>(len), &written, NULL); //Write the data to the piped child process
    CloseHandle(m_stdin); //Close the handle so that the child process starts processing our sent data
    return true;
}

char* Process::read() 
{
    //Read 128 bytes until reallocating buffer
    constexpr size_t buf_size = 128ULL;

    //The buffer to read bytes from the child process into
    char* buf = static_cast<char*>(calloc(buf_size, 1));

    //The total number of bytes read
    size_t total = 0ULL;

    for(;;)
    {
        DWORD read; //The number of bytes read from the pipe
        //Read the data from the child pipe
        if(!ReadFile(m_stdout, buf + total, buf_size, &read, NULL)) 
            break;
        //If read is 0 then exit because we read all possible dat from the pipe
        if(read == 0) 
            break;
        total += read; //Increase the memory address offset to write to the newly allocated part of the buffer
        buf = static_cast<char*>(realloc(buf, total + buf_size)); //Realloc the buffer to fit another chunk of data read
        
    }
    return buf;
}

}