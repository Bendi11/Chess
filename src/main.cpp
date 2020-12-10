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
        for(unsigned x = 0; x < b.container.size(); ++x)
        {
            for(unsigned y = 0; y < b.container[0].size(); ++y)
            {
                b.findMoves(x, y);
            }
        }
        d.input(b);
        d.drawBoard(b);
    }
    SDL_DestroyRenderer(d.render);
    SDL_DestroyWindow(d.win);
    IMG_Quit();
    SDL_Quit();
    
    return 0;
}