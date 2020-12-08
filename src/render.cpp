/*
File to render what the game logic is saying
*/
#include "include/main.hpp"

//Function to render game logic onscreen
void chessGame::renderUpdate()
{
    SDL_RenderPresent(render);
}