#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Chess.hpp"

namespace renderer
{
    class Drawer
    {
    public: 
        //TAKE THIS OUT, DEBUGGING
        bool WHITEORBLACK;
        bool turn = BLACK_TURN;

        /*SDL2 things*/
        SDL_Window* win = NULL; //SDL2 window object
        SDL_Renderer* render = NULL; //SDL2 renderer used to draw textures onscreen
        SDL_Event userIn; //Event object holding user inputted event queue
        SDL_AudioDeviceID audioDevice; //The audio device we'll be playing sound effects to

        bool USING_SOUND = true; //If we should use sound effects
        SDL_AudioSpec wavSpec; //The audio specification for sound effects, .WAV in this case
        Uint32 wavLength; //How long the sound effect is
        uint8_t *wavBuffer; //Buffer for the sound effect

        /*What dimensions the screen will be*/
        unsigned int SCREEN_WIDTH;
        unsigned int SCREEN_HEIGHT;
        bool running = true; //If the game is still running

        /*The square that the user right clicked on*/
        unsigned int storedX = 0;
        unsigned int storedY = 0;
        bool isDragging = false; //If the user is clicking and holding to drag and drop
        SDL_Rect mouseRect; //Mouse position rectangle for drawing a sprite on where the mouse is
        SDL_Texture* draggedTexture; //The texture the mouse is dragging

        /*A 2D matrix of textures, one for each grid square on the board*/
        std::vector<SDL_Texture* > textureList; //List of all loaded textures on the board
        std::vector<std::vector<SDL_Texture* > > textures; 
        std::vector<std::vector<SDL_Texture* > > BGTextures; //Another 8x8 array of textures, these are for the background
        std::vector<std::vector<SDL_Rect > > pos; //Position rectangles for each texture
        SDL_Rect size; //We only need one size rectangle, all sprites are the same size

        float scale; //How much to scale pieces for a certain screen shape

        void init(unsigned int w, unsigned int h, Chess::board_t& Board); //Function to start SDL2 and init other values like BG textures
        void input(Chess::board_t& Board); //Function to handle input

        void drawBoard(Chess::board_t& Board); //Function to draw all pieces on a chessboard
        void assignTextures(Chess::board_t& Board, unsigned int x, unsigned int y); //Function to iterate through and assign the right texture to each piece on a board
    };
    
}