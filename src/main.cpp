/*
Chess by Benjamin L (Bendi11)
Chess game made in SDL2
Compile commands: 
Windows: g++ -o bin/Chess.exe src/main.cpp src/init.cpp src/input.cpp src/fileLoader.cpp src/board.cpp -Isrc/include -lSDL2main -lSDL2 -lSDL2_image -static-libgcc

Linux: g++ -o bin/Chess src/main.cpp src/init.cpp src/input.cpp src/fileLoader.cpp src/board.cpp src/render.cpp -Isrc/include -lSDL2main -lSDL2 -lSDL2_image -static-libgcc
*/
#include "include/main.hpp"

int main(int argc, char** argv)
{
    chessGame g;
    g.init();
    g.Board.wKing.loadIcon("assets/pawn.png", g.render);
    SDL_Rect pos;
    pos.x = 0;
    pos.y = 0;
    SDL_RenderCopy(g.render, g.Board.wKing.icon, NULL, &pos);
    while(g.IS_RUNNING)
    {
        g.renderUpdate();
        g.getInput();

    }
}