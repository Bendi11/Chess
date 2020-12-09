/*
File to render what the game logic is saying
*/
#include "include/render.hpp"

using namespace renderer;

void Drawer::init(unsigned int w, unsigned int h, Chess::board_t& Board)
{
    SCREEN_WIDTH = w;
    SCREEN_HEIGHT = h;
    scale = SCREEN_HEIGHT / (8 * 77);

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    win = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    textures.resize(8);
    pos.resize(8);
    size.resize(8);
    BGTextures.resize(8);
    unsigned int shift = 0;

    for(unsigned x = 0; x < 8; ++x)
    {
        textures[x].resize(8);
        pos[x].resize(8);
        size[x].resize(8);
        BGTextures[x].resize(8);

        for(unsigned y = 0; y < 8; ++y)
        {
            if(x == y)
            {
                BGTextures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/wSquare.png"));
            }
            else
            {
                BGTextures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/bSquare.png"));
            }
            
            if(x == 0 && y == 7)
            {
                textures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/wQueen.png"));
            }
            else if(Board.container[x][y].type != EMPTY)
            {
                textures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/bRook.png"));
            }
            pos[x][y].x = x * (SCREEN_WIDTH / 8);
            pos[x][y].y = y * (SCREEN_HEIGHT / 8);
            pos[x][y].w = 77 * scale;
            pos[x][y].h = 77 * scale;

            size[x][y].x = 0;
            size[x][y].y = 0;
            size[x][y].w = 77 * scale;
            size[x][y].h = 77 * scale;
            if(shift == 0) shift = 1;
            else shift = 0;
            
        }
    }

}

//Function to draw a chess board onscreen
void Drawer::drawBoard(Chess::board_t& Board)
{
    for(unsigned x = 0; x < 8; ++x)
    {
        for(unsigned y = 0; y < 8; ++y)
        {
            SDL_RenderCopy(render, BGTextures[x][y], &size[x][y], &pos[x][y]);
            if(Board.container[x][y].type != EMPTY)
            {
                SDL_RenderCopy(render, textures[x][y], &size[x][y], &pos[x][y]);
            }
            
        }
    }
    SDL_RenderPresent(render);
}