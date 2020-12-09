#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Chess.hpp"

namespace renderer
{
    class Drawer
    {
    public: 
        /*SDL2 things*/
        SDL_Window* win = NULL;
        SDL_Renderer* render = NULL;
        SDL_Event e;
        unsigned int SCREEN_WIDTH;
        unsigned int SCREEN_HEIGHT;

        std::vector<std::vector<SDL_Texture* > > textures; 
        std::vector<std::vector<SDL_Texture* > > BGTextures;
        std::vector<std::vector<SDL_Rect > > pos;
        std::vector<std::vector<SDL_Rect > > size;

        float scale; //How much to scale pieces for a cerain screen coords

        //Textures for white and black squares of the chess board
        SDL_Texture* whiteTexture = NULL;
        SDL_Texture* blacktexture = NULL;

        void init(unsigned int w, unsigned int h, Chess::board_t& Board);

        void drawBoard(Chess::board_t& Board); //Function to draw all pieces on a chessboard
    };
    
}