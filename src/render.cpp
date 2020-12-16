/*
File to render what the game logic is saying
*/
#include "include/render.hpp"

using namespace renderer;

//Function to assign right textures to each piece on a board
void Drawer::assignTextures(Chess::board_t& Board, unsigned int x, unsigned int y)
{
    //SDL_DestroyTexture(textures[x][y]); //Delete this texture because it will be replaced
    /*Assign a texture*/
    switch(Board.container[x][y].type)
    {
        case wPAWN:
            textures[x][y] = textureList[wPAWN];
        break;

        case bPAWN:
            textures[x][y] = textureList[bPAWN];
        break;

        case wROOK:
            textures[x][y] = textureList[wROOK];
        break;

        case bROOK:
            textures[x][y] = textureList[bROOK];
        break;

        case wBISHOP:
            textures[x][y] = textureList[wBISHOP];
        break;

        case bBISHOP:
            textures[x][y] = textureList[bBISHOP];
        break;

        case wKNIGHT:
            textures[x][y] = textureList[wKNIGHT];
        break;

        case bKNIGHT:
            textures[x][y] = textureList[bKNIGHT];
        break;

        case wKING:
            textures[x][y] = textureList[wKING];
        break;

        case bKING:
            textures[x][y] = textureList[bKING];
        break;

        case wQUEEN:
            textures[x][y] = textureList[wQUEEN];
        break;

        case bQUEEN:
            textures[x][y] = textureList[bQUEEN];
        break;

        case EMPTY:
            textures[x][y] = NULL;
        break;
    }
    
}

//Function to start drawer with all correct SDL2 values etc.
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
    TTF_Init(); //Start text rendering
    sans = TTF_OpenFont("assets/FreeSans.ttf", 14); //Load the font from file

    
    //Create an SDL2 window with the given parameters
    win = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(!win) //If the window doesn't exist
    {
        exit(EXIT_FAILURE); //Quit the program
    }

    render = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE); //Make a renderer for SDL2 associated with the window

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
    textureList.resize(14); //12 pieces + 2 BG tiles
    pos.resize(8);
    BGTextures.resize(8);

    SDL_Surface* temp;
    temp = IMG_Load("assets/wPawn.png");
    textureList[wPAWN] = SDL_CreateTextureFromSurface(render, temp); //Make a texture from the surface

    temp = IMG_Load("assets/wKnight.png");
    textureList[wKNIGHT] = SDL_CreateTextureFromSurface(render, temp); //Make a texture from the surface

    temp = IMG_Load("assets/wBishop.png");
    textureList[wBISHOP] = SDL_CreateTextureFromSurface(render, temp); //Make a texture from the surface

    temp = IMG_Load("assets/wRook.png");
    textureList[wROOK] = SDL_CreateTextureFromSurface(render, temp); //Make a texture from the surface

    temp = IMG_Load("assets/wKing.png");
    textureList[wKING] = SDL_CreateTextureFromSurface(render, temp); //Make a texture from the surface

    temp = IMG_Load("assets/wQueen.png");
    textureList[wQUEEN] = SDL_CreateTextureFromSurface(render, temp); //Make a texture from the surface


    temp = IMG_Load("assets/bPawn.png");
    textureList[bPAWN] = SDL_CreateTextureFromSurface(render, temp); //Make a texture from the surface

    temp = IMG_Load("assets/bKnight.png");
    textureList[bKNIGHT] = SDL_CreateTextureFromSurface(render, temp); //Make a texture from the surface

    temp = IMG_Load("assets/bBishop.png");
    textureList[bBISHOP] = SDL_CreateTextureFromSurface(render, temp); //Make a texture from the surface

    temp = IMG_Load("assets/bRook.png");
    textureList[bROOK] = SDL_CreateTextureFromSurface(render, temp); //Make a texture from the surface

    temp = IMG_Load("assets/bKing.png");
    textureList[bKING] = SDL_CreateTextureFromSurface(render, temp); //Make a texture from the surface

    temp = IMG_Load("assets/bQueen.png");
    textureList[bQUEEN] = SDL_CreateTextureFromSurface(render, temp); //Make a texture from the surface

    temp = IMG_Load("assets/moveable.png"); //Load the moveable texture
    moveableTexture = SDL_CreateTextureFromSurface(render, temp);

    SDL_FreeSurface(temp);


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
                    BGTextures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/wSquare.png"));
                else
                {
                    BGTextures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/bSquare.png"));
                }
                
            }
            else
            {
                 if(x % 2)
                    BGTextures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/bSquare.png"));
                else
                {
                    BGTextures[x][y] = SDL_CreateTextureFromSurface(render, IMG_Load("assets/wSquare.png"));
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
    SDL_Delay(13); //Cap FPS
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
    if(isDragging) //If the user is dragging a piece
    {
        SDL_Surface* temp = IMG_Load("assets/moveable.png"); //Temporary surface loaded with moveable.png
        for(unsigned n = 0; n < moveableTexts.size(); ++n)
        {
            SDL_DestroyTexture(moveableTexts[n]); //Delete all currently stored textures to avoid ANOTHER MEMORY LEAK
        }
        for(unsigned n = 0; n < attackableTexts.size(); ++n)
        {
            SDL_DestroyTexture(attackableTexts[n]); //Delete all currently stored textures to avoid ANOTHER MEMORY LEAK
        }

        moveableTexts.clear(); //Empty the moveable textures object
        attackableTexts.clear(); //Empty the attackable textures list
        attackablePos.clear(); //Empty the attackable textures positions list
        moveablePos.clear(); //Empty the positions of the textures 

        for(unsigned i = 0; i < Board.container[storedX][storedY].moveable.size(); ++i) //Iterate through each moveable spot
        {
            moveableTexts.push_back(SDL_CreateTextureFromSurface(render, temp)); //Add a new moveable texture object
            /*Determine position of the texture*/
            moveablePos.push_back(SDL_Rect());
            moveablePos[i].x = Board.container[storedX][storedY].moveable[i].first * (SCREEN_WIDTH / 8);
            moveablePos[i].y = -((int)Board.container[storedX][storedY].moveable[i].second - 7) * (SCREEN_HEIGHT / 8); //Reverse Y coordinate
            moveablePos[i].w = 77;
            moveablePos[i].h = 77;

            SDL_RenderCopy(render, moveableTexts[i], &size, &moveablePos[i]); //Copy the loaded texture
        }

        for(unsigned j = 0; j < Board.container[storedX][storedY].attackable.size(); ++j) //Iterate through each attackable spot
        {
            attackableTexts.push_back(SDL_CreateTextureFromSurface(render, temp)); //Add a new moveable texture object
            /*Determine the position of the texture*/
            attackablePos.push_back(SDL_Rect());
            attackablePos[j].x = Board.container[storedX][storedY].attackable[j].first * (SCREEN_WIDTH / 8);
            attackablePos[j].y = -((int)Board.container[storedX][storedY].attackable[j].second - 7) * (SCREEN_HEIGHT / 8); //Reverse Y coordinate
            attackablePos[j].w = 77;
            attackablePos[j].h = 77;

            SDL_RenderCopy(render, attackableTexts[j], &size, &attackablePos[j]);
        }
        SDL_FreeSurface(temp); //Free the temporary surface
        SDL_RenderCopy(render, textures[storedX][storedY], &size, &mouseRect); //Show the dragged piece
    } 

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
            success = Board.playerMove(storedX, storedY, mX, mY, WHITEORBLACK); //Get wether the move failed, completed, or captured a piece

            //playerMove returns 2 if the move captured something, or 1 if it was successful, so play a sound effect based on this
            if(success == MOVE_GOOD) //If the move completed
            {
                movedX = mX;
                movedY = mY;

                if(USING_SOUND)
                {
                    SDL_LoadWAV("assets/sounds/move.wav", &wavSpec, &wavBuffer, &wavLength); //Load the move sound effect
                    SDL_QueueAudio(audioDevice, wavBuffer, wavLength); //Play the sound effect for moving
                    SDL_PauseAudioDevice(audioDevice, 0); //Un - pause audio
                } 
            }
            else if(success == MOVE_CAPTURED)
            {
                movedX = mX;
                movedY = mY;
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

std::string Drawer::getTextInput() //Function to get text input and return the string of text, while displaying the inputted text
{
    bool exit = false; //If the user pressed enter
    bool edited = false;
    std::string inputText; //The text the user entered

    SDL_StartTextInput(); //Start SDL2 text input

    SDL_Surface* textSurface; //Surface containing the text window
    SDL_Color txtCol = {0xFF, 0xFF, 0xFF}; //Text color

    SDL_Surface* promptSurface = TTF_RenderText_Shaded(sans, "Enter the host's IPv4 address here: ", txtCol, {0, 0, 0}); //The prompt for text input
    SDL_Rect promptPos;
    promptPos.x = SCREEN_WIDTH / 2;
    promptPos.y = SCREEN_HEIGHT / 2;


    SDL_Texture* promptTexture = SDL_CreateTextureFromSurface(render, promptSurface); //Texture for the text editing prompt
    SDL_Texture* textTexture =  SDL_CreateTextureFromSurface(render, promptSurface); //Texture for the text editing prompt

    SDL_Rect pos; //Position of the text surface
    pos.x = SCREEN_WIDTH / 2;
    pos.y = (SCREEN_HEIGHT / 2 )+ 20;
    pos.h = TTF_FontHeight(sans);


    SDL_Rect sizee;
    sizee.h = promptSurface->h;
    sizee.w = promptSurface->w;
    sizee.y = (SCREEN_HEIGHT / 2) - 20;
    sizee.x = (SCREEN_WIDTH / 2) - sizee.w / 2;

    SDL_RenderClear(render);
    SDL_RenderCopy(render, promptTexture, NULL, &sizee);
    SDL_RenderPresent(render);

    while(!exit) //While the user hasn't entered an IP...
    {
        while(SDL_WaitEventTimeout(&userIn, 30)) //Read through queue of key presses
        {
            if(userIn.type == SDL_QUIT) //If the user is quitting
            {
                exit = true;
            }
            else if(userIn.type == SDL_KEYDOWN) //If the user is pressing backspace / enter
            {
                if(userIn.key.keysym.sym == SDLK_BACKSPACE)
                {
                    if(inputText.length() > 1) inputText.pop_back(); //Take a character off the string
                    else
                    {
                        inputText[0] = ' ';
                    }
                    
                    edited = true;
                } 
                else if(userIn.key.keysym.sym == SDLK_RETURN || userIn.key.keysym.sym == SDLK_RETURN2) exit = true; //Exit the text editing if the user entered
                
            }
            else if(userIn.type == SDL_TEXTINPUT) //If the user is inputting text
            {
                inputText += userIn.text.text; //Append the inputted text to the string
                edited = true;
            }
        }
        if(edited) //Re draw the text box if it was changed
        {
            edited = false;
            SDL_RenderClear(render); //Clear the renderer
            textSurface = TTF_RenderText_Shaded(sans, inputText.c_str(), txtCol, {0, 0, 0}); //Render the new text
            pos.w = textSurface->w;
            pos.x = (SCREEN_WIDTH / 2) - pos.w / 2;

            SDL_DestroyTexture(textTexture);
            textTexture = SDL_CreateTextureFromSurface(render, textSurface); //Make a texture from the surface
            SDL_RenderCopy(render, textTexture, NULL, &pos);
            SDL_RenderCopy(render, promptTexture, NULL, &sizee);
            SDL_FreeSurface(textSurface); //Clear the text surface
            SDL_RenderPresent(render);
        }
        
        
    }
    
    SDL_StopTextInput(); //Stop text input
    return inputText;
}

//Function to draw board without FPS cap
void Drawer::fastDrawBoard(Chess::board_t& Board)
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

    SDL_RenderPresent(render);
}
