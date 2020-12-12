/*
Chess by Benjamin L (Bendi11)
Chess game made with C++
Compile commands: 
Windows: 
g++ -o bin/Chess.exe src/main.cpp src/Chess.cpp src/render.cpp -Isrc/include -IE:/boost_1_74_0 -IE:/asio-1.18.0/include -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -static-libgcc -static-libstdc++ -mwindows icon.o

Linux: 
g++ -o bin/Chess src/main.cpp src/Chess.cpp src/render.cpp -Isrc/include -lSDL2main -lSDL2 -lSDL2_image -static-libgcc -static-libstdc++ icon.o
*/

#include "include/Chess.hpp"
#include "include/render.hpp"
//#include <thread>
//#include <asio.hpp>
#include <iostream>

#include <fstream>

//using asio::ip::tcp;

//asio::io_context ioContext;

std::string storedBMove;
std::string storedWMove;
unsigned int myCounter = 1;
std::string received;
//tcp::acceptor servAccept(ioContext, tcp::endpoint(tcp::v4(), 8080)); //Server request acceptor   



bool server = true;
bool isWhite = true;
bool receivedMessage = false;

/*/Function to be run in parallel with the game logic to check for messages being sent
void netThread()
{

    for(;;)
    {
        if(server)
        {
            
            asio::error_code error;
            char buf[128];   
            for(;;)
            {
                

                //asio::read(socket, asio::buffer(buf));
                if(buf[0] != '\0') 
                {
                    std::cout<<buf<<std::endl;
                    //W:1,2->1,1
                    if(!isWhite)
                    {
                        b.playerMove(buf[2] - '0', buf[4] - '0', buf[7] - '0', buf[9] - '0', true);
                    }
                    else
                    {
                        b.playerMove(buf[2] - '0', buf[4] - '0', buf[7] - '0', buf[9] - '0', false);
                    }
                    
                }
            }     
            
        }
    }
}*/

//Function to show a message box asking for client / server socket connection
void showStartupBox()
{
    const SDL_MessageBoxColorScheme colorScheme = {
    { /* .colors (.r, .g, .b) */
        /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
        { 255,   255,   255 },
        /* [SDL_MESSAGEBOX_COLOR_TEXT] */
        {   0, 0,   0 },
        /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
        { 0, 0,   0 },
        /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
        {   200,   200, 200 },
        /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
        { 100,   100, 100 }
    } };

    const SDL_MessageBoxButtonData buttons[] = 
    {
        {/*Flags,   buttonid, text*/ 0, 0, "Join a game"},
        {0, 1, "Host a game"}
    };
    const SDL_MessageBoxData messageData = 
    {
        SDL_MESSAGEBOX_COLOR_TEXT,
        NULL,
        "Chess",
        "How do you want to play?",
        SDL_arraysize(buttons),
        buttons,
        &colorScheme
    };
    int buttonID;
    SDL_ShowMessageBox(&messageData, &buttonID); //Show the message box
    if(buttonID == 0) //User decided to connect to a game
    {
        server = false;
        isWhite = false;
    }
    else if(buttonID == 1) //User decided to host a game
    {
        server = true;
        isWhite = true;
    }

} 

int main(int argc, char** argv)
{
    Chess::board_t b;
    renderer::Drawer d;
    
    d.init(624, 624, b);
    bool gameover = false;
    //showStartupBox();


    //tcp::socket socket(ioContext); //Make a new socket
    //servAccept.accept(socket); //Await a connection request

    d.WHITEORBLACK = isWhite;
    //std::thread t1(netThread);

    while(d.running)
    {
        d.drawBoard(b);
        d.input(b);
        /*if(myCounter != b.counter && !isWhite)
        {
            myCounter = b.counter;
            //asio::write(socket, asio::buffer(b.bMoveString));
        }
        else if(myCounter != b.counter && isWhite)
        {
            myCounter = b.counter;
            //asio::write(socket, asio::buffer(b.wMoveString));
        }*/

        if(b.WINNER != WINNER_NONE)
        {
            if(b.WINNER == WINNER_WHITE)
            {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Chess", "You won by checkmate!", d.win);
                gameover = true;
            }
            else
            {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Chess", "You lost by checkmate!", d.win);
                gameover = true;
            }
            
        }
        if(gameover == true)
        {
            b.restart();
            gameover = false;
        }
    }

    SDL_DestroyRenderer(d.render);
    SDL_DestroyWindow(d.win);
    SDL_CloseAudio();
    SDL_FreeWAV(d.wavBuffer);
    IMG_Quit();
    SDL_Quit();
    
    return 0;
}