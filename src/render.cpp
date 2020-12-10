/*
File to render what the game logic is saying
*/
#include "include/render.hpp"

using namespace renderer;

//Function to assign right textures to each piece on a board
void Drawer::assignTextures(Chess::board_t& Board, unsigned int x, unsigned int y)
{
    SDL_DestroyTexture(textures[x][y]); //Delete this texture because it will be replaced
    SDL_Surface* temp = NULL; //Temp surface for loading the texture
    /*Assign a texture*/
    switch(Board.container[x][y].type)
    {
        case wPAWN:
            temp = IMG_Load("assets/wPawn.png");
        break;

        case bPAWN:
            temp =  IMG_Load("assets/bPawn.png");
        break;

        case wROOK:
            temp = IMG_Load("assets/wRook.png");
        break;

        case bROOK:
            temp =  IMG_Load("assets/bRook.png");
        break;

        case wBISHOP:
            temp =  IMG_Load("assets/wBishop.png");
        break;

        case bBISHOP:
            temp = IMG_Load("assets/bBishop.png");
        break;

        case wKNIGHT:
            temp = IMG_Load("assets/wKnight.png");
        break;

        case bKNIGHT:
            temp = IMG_Load("assets/bKnight.png");
        break;

        case wKING:
            temp = IMG_Load("assets/wKing.png");
        break;

        case bKING:
            temp = IMG_Load("assets/bKing.png");
        break;

        case wQUEEN:
            temp = IMG_Load("assets/wQueen.png");
        break;

        case bQUEEN:
            temp = IMG_Load("assets/bQueen.png");
        break;

        case EMPTY:

        break;
    }

    textures[x][y] = SDL_CreateTextureFromSurface(render, temp); //Make a texture from the surface
    SDL_FreeSurface(temp); //Clear the surface to avoid ANOTHER FR*GGING MEMORY LEAK
}

void Drawer::init(unsigned int w, unsigned int h, Chess::board_t& Board)
{
    /*Set screen dimensions according to inputted values*/
    SCREEN_WIDTH = w;
    SCREEN_HEIGHT = h;

    scale = (float)SCREEN_WIDTH / (8.0f * 77.0f); //Get how much to scale sprites in order to fit them to the screen

    /*Make sure that SDL init goes well*/
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        exit(EXIT_FAILURE); //Exit the program if SDL2 couldn't init
    }
    IMG_Init(IMG_INIT_PNG); //Start SDL2_image
    
    //Create an SDL2 window with the given parameters
    win = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(!win) //If the window doesn't exist
    {
        exit(EXIT_FAILURE); //Quit the program
    }

    render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED); //Make a renderer for SDL2 associated with the window

    SDL_LoadWAV("assets/sounds/move.wav", &wavSpec, &wavBuffer, &wavLength); //Load a sound effect to populate the specification for WAV data
    USING_SOUND = true; //Sound is on by default
    if( (audioDevice = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0) ) < 0) //Open the desired audio device
    {
        USING_SOUND = false; //Just don't play sound effects if there was an error with opening an audio device
    }
    SDL_PauseAudioDevice(audioDevice, 0); //Enable sound playback

    /*SDL2 init done!*/

    //Set the size of the mouse position rectangle
    mouseRect.w = (PIECE_X + 1) * scale;
    mouseRect.h = (PIECE_Y + 1) * scale; 

    //Set the size rectangle once, everything is the same size
    size.x = 0;
    size.y = 0;
    size.w = (PIECE_X + 1) * scale; //The texture size is 77x77, but I increased by one pixel to avoid single pixel gaps between textures
    size.h = (PIECE_Y + 1) * scale;
    unsigned int realY = 8; //Realy counts down as y counts up, it is stupid but works

    /*Resize all matrices to be 8x8, matching the chess board*/
    textures.resize(8);
    pos.resize(8);
    BGTextures.resize(8);

    for(unsigned x = 0; x < 8; ++x)
    {
        /*Resize each matrix to actually be 8x8*/
        textures[x].resize(8);
        pos[x].resize(8);
        BGTextures[x].resize(8);
        realY = 7; 

        for(unsigned y = 0; y < 8; ++y)
        {
            /*Making the checkerboard pattern for the background*/
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
            
            /*Assigning the right values to the position rectangle, mapping the chess coordinate to the screen coordinate*/
            pos[x][y].x = x * (SCREEN_WIDTH / 8);
            pos[x][y].y = realY * (SCREEN_HEIGHT / 8);
            pos[x][y].w = (PIECE_X + 1) * scale;
            pos[x][y].h = (PIECE_Y + 1) * scale;
            realY--; //Subtract from realY so that it decreases while y increases
            
        }
    }

}

//Function to draw a chess board onscreen
void Drawer::drawBoard(Chess::board_t& Board)
{
    SDL_RenderClear(render); //Clear the renderer of all loaded textures

    for(unsigned x = 0; x < 8; ++x)
    {
        for(unsigned y = 0; y < 8; ++y)
        {
            assignTextures(Board, x, y); //Assign the correct texture to this coordinate
            SDL_RenderCopy(render, BGTextures[x][y], &size, &pos[x][y]); //Add the background texture 
            SDL_RenderCopy(render, textures[x][y], &size, &pos[x][y]); //Draw the piece texture over it 
        }
    }
    if(isDragging) SDL_RenderCopy(render, textures[storedX][storedY], &size, &mouseRect);
    SDL_RenderPresent(render);
}

void Drawer::input(Chess::board_t& Board)
{
    while(SDL_PollEvent(&userIn)) //Poll through the queue of inputted events
    {
        //Exit the program if the inputted event was to quit
        if(userIn.type == SDL_QUIT) 
        {
            running = false;
        }
        else if(userIn.type == SDL_MOUSEMOTION) //Check if the user just moved their mouse
        {
            if(isDragging) //If the user is currently drag and dropping a piece
            {
                /*Set mouse dragged sprite position*/
                mouseRect.x = userIn.motion.x - (PIECE_X / 2);
                mouseRect.y = userIn.motion.y - (PIECE_Y / 2);
            }
        }
        else if(userIn.type == SDL_MOUSEBUTTONDOWN && userIn.button.button == SDL_BUTTON_LEFT) //Start drag and dropping a piece
        {
            if(!isDragging) //Make sure we aren't already dragging
            {
                /*Changing mouse screen coordinates to chess board coordinates*/
                storedX = (unsigned int)(userIn.motion.x / (SCREEN_WIDTH / 8) );
                storedY = (unsigned int)(userIn.motion.y / (SCREEN_HEIGHT / 8));
                storedY =   -(storedY - 7); //Y coords need to be reversed because I am not a smart person

                /*Set the mouse dragged sprite position*/
                mouseRect.x = userIn.motion.x - (PIECE_X / 2);
                mouseRect.y = userIn.motion.y - (PIECE_Y / 2);

                isDragging = true; //The user is dragging a piece
            }
            

        }   
        //Checking if the event was a mouse release
        else if(userIn.type == SDL_MOUSEBUTTONUP && userIn.button.button == SDL_BUTTON_LEFT && isDragging)
        {
            isDragging = false; //We aren't trying to drag and drop anymore

            /*Changing mouse screen coordinates to chess board coordinates*/
            unsigned int mX = (unsigned int)(userIn.motion.x / (SCREEN_WIDTH / 8) );
            unsigned int mY = (unsigned int)(userIn.motion.y / (SCREEN_HEIGHT / 8));

            mY =   -(mY - 7); //Y coords need to be reversed because I am not a smart person

            uint8_t success; //Store the move's outcome in success variable
            success = Board.playerMove(storedX, storedY, mX, mY, true); //Get wether the move failed, completed, or captured a piece

            //playerMove returns 2 if the move captured something, or 1 if it was successful, so play a sound effect based on this
            if(success == MOVE_GOOD) //If the move completed
            {
                if(USING_SOUND)
                {
                    SDL_LoadWAV("assets/sounds/move.wav", &wavSpec, &wavBuffer, &wavLength); //Load the move sound effect
                    SDL_QueueAudio(audioDevice, wavBuffer, wavLength); //Play the sound effect for moving
                    SDL_PauseAudioDevice(audioDevice, 0); //Un - pause audio
                } 
            }
            else if(success == MOVE_CAPTURED)
            {
                if(USING_SOUND)
                {
                    SDL_LoadWAV("assets/sounds/capture.wav", &wavSpec, &wavBuffer, &wavLength); //Load the capture sound effect
                    SDL_QueueAudio(audioDevice, wavBuffer, wavLength); //Play the sound effect for capturing a piece
                    SDL_PauseAudioDevice(audioDevice, 0); //Un - pause audio
                }
                
            } 
            
        }
    } 
}

