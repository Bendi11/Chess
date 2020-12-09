/*
File to render what the game logic is saying
*/
#include "include/render.hpp"

using namespace renderer;

void Drawer::init(unsigned int w, unsigned int h, Chess::board_t& Board)
{
    SCREEN_WIDTH = w;
    SCREEN_HEIGHT = h;
    scale = (float)SCREEN_WIDTH / (8.0f * 77.0f);
    std::cout<<scale<<std::endl;

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    win = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    size.x = 0;
    size.y = 0;
    size.w = 78 * scale;
    size.h = 78 * scale;
    unsigned int realY = 8;

    textures.resize(8);
    pos.resize(8);
    BGTextures.resize(8);

    for(unsigned x = 0; x < 8; ++x)
    {
        textures[x].resize(8);
        pos[x].resize(8);
        BGTextures[x].resize(8);
        realY = 7;

        for(unsigned y = 0; y < 8; ++y)
        {
            if(y % 2 == 0)
            {
                if(x % 2)
                    BGTextures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/bSquare.png"));
                else
                {
                    BGTextures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/wSquare.png"));
                }
                
            }
            else
            {
                 if(x % 2)
                    BGTextures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/wSquare.png"));
                else
                {
                    BGTextures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/bSquare.png"));
                }
            }
            
            switch(Board.container[x][y].type)
            {
                case wPAWN:
                    textures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/wPawn.png"));
                break;

                case bPAWN:
                    textures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/bPawn.png"));
                break;

                case wROOK:
                    textures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/wRook.png"));
                break;

                case bROOK:
                    textures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/bRook.png"));
                break;

                case wBISHOP:
                    textures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/wBishop.png"));
                break;

                case bBISHOP:
                    textures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/bBishop.png"));
                break;

                case wKNIGHT:
                    textures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/wKnight.png"));
                break;

                case bKNIGHT:
                    textures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/bKnight.png"));
                break;

                case wKING:
                    textures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/wKing.png"));
                break;

                case bKING:
                    textures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/bKing.png"));
                break;

                case wQUEEN:
                    textures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/wQueen.png"));
                break;

                case bQUEEN:
                    textures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/bQueen.png"));
                break;
            }
            
            
            pos[x][y].x = x * (SCREEN_WIDTH / 8);
            pos[x][y].y = realY * (SCREEN_HEIGHT / 8);
            pos[x][y].w = 78 * scale;
            pos[x][y].h = 78 * scale;
            realY--;
            
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
            SDL_RenderCopy(render, BGTextures[x][y], &size, &pos[x][y]);
            if(Board.container[x][y].type != EMPTY)
            {
                SDL_RenderCopy(render, textures[x][y], &size, &pos[x][y]);
            }
            
        }
    }
    SDL_RenderPresent(render);
}

void Drawer::input(Chess::board_t& Board)
{
    while(SDL_PollEvent(&e))
    {
        if(e.type == SDL_QUIT)
        {
            running = false;
        }
        else if(e.type == SDL_MOUSEBUTTONDOWN)
        {
            unsigned int mX = (unsigned int)(e.motion.x / (SCREEN_WIDTH / 8) );
            unsigned int mY = (unsigned int)(e.motion.y / (SCREEN_HEIGHT / 8));
            mY = ( (7 - 0) / (0 - 7)) * (mY - 7);
            if(e.button.button == SDL_BUTTON_RIGHT)
            {
                storedY = mY;
                storedX = mX;
            }
            else
            {
            std::cout<<"Mouse X: "<<mX<<" Mouse Y: "<<mY<<std::endl;
                
                if(Board.move(storedX, storedY, mX, mY))
                {   
        std::cout<<"Can move!"<<std::endl;
        SDL_DestroyTexture(textures[storedX][storedY]);
        SDL_DestroyTexture(textures[mX][mY]);
        unsigned int x = mX;
        unsigned int y = mY;

        switch(Board.container[x][y].type)
            {
                case wPAWN:
                    textures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/wPawn.png"));
                break;

                case bPAWN:
                    textures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/bPawn.png"));
                break;

                case wROOK:
                    textures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/wRook.png"));
                break;

                case bROOK:
                    textures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/bRook.png"));
                break;

                case wBISHOP:
                    textures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/wBishop.png"));
                break;

                case bBISHOP:
                    textures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/bBishop.png"));
                break;

                case wKNIGHT:
                    textures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/wKnight.png"));
                break;

                case bKNIGHT:
                    textures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/bKnight.png"));
                break;

                case wKING:
                    textures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/wKing.png"));
                break;

                case bKING:
                    textures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/bKing.png"));
                break;

                case wQUEEN:
                    textures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/wQueen.png"));
                break;

                case bQUEEN:
                    textures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/bQueen.png"));
                break;
            }
            } 
        }
    }
    }
    
}