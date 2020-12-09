/*
Chess by Benjamin L (Bendi11)
Chess game made in SDL2
Compile commands: 
Windows: 
g++ -o bin/Chess.exe src/main.cpp src/Chess.cpp src/render.cpp -Isrc/include -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -static-libgcc

Linux: 
g++ -o bin/Chess src/main.cpp src/Chess.cpp src/render.cpp -Isrc/include -lSDL2main -lSDL2 -lSDL2_image -static-libgcc
*/
#include "include/Chess.hpp"
#include "include/render.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int main(int argc, char** argv)
{
    renderer::Drawer d;
    Chess::board_t b;
    d.init(720, 720, b);
    bool running = true;
    while(running)
    {
        d.input(b);
        d.drawBoard(b);
        while(SDL_PollEvent(&d.e))
        {
            if(d.e.type == SDL_QUIT)
            {
                running = false;
            }
        }
        d.input(b);
    }
    
    SDL_Delay(2000);
    return 0;
}