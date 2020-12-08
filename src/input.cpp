/*
Function to handle player input and output events that the player wants to happen
Ex: 
ke4
*/

#include "include/main.hpp"

//Function to get user input 
void chessGame::getInput()
{
    while(SDL_WaitEvent(&userE)) //WaitEvent waits until input is entered, to avoid the game updating when it doesn't need to
    {
        if(userE.type == SDL_QUIT) //If the user wants to quit the game
        {
            IS_RUNNING = false; //Make the game not running anymore
            return; //Exit input loop
        }
    }
}