/*
Chess by Benjamin L (Bendi11)
Chess game made in SDL2
Compile commands: 
Windows: 
g++ -o bin/Chess.exe src/main.cpp src/init.cpp src/input.cpp src/fileLoader.cpp src/board.cpp src/render.cpp -Isrc/include -lSDL2main -lSDL2 -lSDL2_image -static-libgcc

Linux: 
g++ -o bin/Chess src/main.cpp src/init.cpp src/input.cpp src/fileLoader.cpp src/board.cpp src/render.cpp -Isrc/include -lSDL2main -lSDL2 -lSDL2_image -static-libgcc
*/
#include "include/main.hpp"

int main(int argc, char** argv)
{
    chessGame g;
    g.init();
    g.Board.wKing.loadIcon("assets/wPawn.png", g.render);
    SDL_Rect pos;
    pos.x = 0;
    pos.y = 0;
    pos.w = 77;
    pos.h = 77;
    SDL_Rect Pos;
    Pos.x = 0;
    Pos.y = 0;
    Pos.w = pos.w * g.SCALING_X;
    Pos.h = pos.h * g.SCALING_Y;
    SDL_RenderCopy(g.render, g.Board.wKing.icon, &pos, &Pos);
    while(g.IS_RUNNING)
    {
        g.renderUpdate();
        g.getInput();

    }
}