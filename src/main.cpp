#include "board.hpp"
#include "gui.hpp"

#ifdef _WIN32
#include <Windows.h>
#define main(...) WinMain(HINSTANCE h, HINSTANCE h2, PSTR str, int arg)
#endif

int main(int argc, char* argv[])
{
    ChessGui gui;
    gui.init();
    gui.loop();
    return 0;
}
