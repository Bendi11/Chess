/*
Function to handle the game init, so setting up SDL2 objects, checking the list twice, etc.
*/
#include "include/main.hpp"

void chessGame::init()
{
    loadCfg(); //Open the log file and read the cfg file

    //Start init SDL2
    if(SDL_Init(SDL_INIT_VIDEO))
    {   
        logger<<"SDL_Init bad! Error: "<<SDL_GetError()<<std::endl; //Log the error
        exit(EXIT_FAILURE); //Exit the program
    }

    logger<<"SDL_Init good!"<<std::endl; //Log the init went good

    IMG_Init(IMG_INIT_PNG); //Start sdl image loading

    //Create the SDL2 window
    win = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(!win) //Make sure the window was created
    {
        logger<<"SDL_CreateWindow bad! Error: "<<SDL_GetError()<<std::endl; //Log the error
        exit(EXIT_FAILURE); //Exit the program
    }

    logger<<"Created SDL2 window with height "<<SCREEN_HEIGHT<<" and width "<<SCREEN_WIDTH<<std::endl;

    render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED); //Make the SDL2 renderer
    if(!render) //Make sure SDL2 renderer object was created
    {
        logger<<"SDL_CreateRenderer bad! Error: "<<SDL_GetError()<<std::endl; //Log the error
        exit(EXIT_FAILURE); //Exit the program
    }

    logger<<"Created SDL2 renderer!"<<std::endl;




}