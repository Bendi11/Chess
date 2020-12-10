/*
Chess by Benjamin L (Bendi11)
Chess game made with C++
Compile commands: 
Windows: 
g++ -o bin/Chess.exe src/main.cpp src/Chess.cpp src/render.cpp -Isrc/include -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -static-libgcc -mwindows icon.o

Linux: 
g++ -o bin/Chess src/main.cpp src/Chess.cpp src/render.cpp -Isrc/include -lSDL2main -lSDL2 -lSDL2_image -static-libgcc icon.o
*/

#include "include/Chess.hpp"
#include "include/render.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int main(int argc, char** argv)
{
    renderer::Drawer d;
    Chess::board_t b;
    d.init(624, 624, b);

    while(d.running)
    {
        d.input(b);
        d.drawBoard(b);
    }

    SDL_DestroyRenderer(d.render);
    SDL_DestroyWindow(d.win);
    SDL_CloseAudio();
    SDL_FreeWAV(d.wavBuffer);
    IMG_Quit();
    SDL_Quit();
    
    return 0;
}