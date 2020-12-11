/*
Chess by Benjamin L (Bendi11)
Chess game made with C++
Compile commands: 
Windows: 
g++ -o bin/Chess.exe src/main.cpp src/Chess.cpp src/render.cpp -Isrc/include -IE:/asio-1.18.0/include -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -static-libgcc -static-libstdc++ -mwindows icon.o

Linux: 
g++ -o bin/Chess src/main.cpp src/Chess.cpp src/render.cpp -Isrc/include -lSDL2main -lSDL2 -lSDL2_image -static-libgcc -static-libstdc++ icon.o
*/

#define PORT 23
#include "include/Chess.hpp"
#include "include/render.hpp"
#include <asio.hpp>

#include <fstream>

using asio::ip::tcp;
asio::io_context ioContext;

std::string storedBMove;
std::string storedWMove;

bool server = false;

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

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
    }
    else if(buttonID == 1) //User decided to host a game
    {
        server = true;
    }

} 



int main(int argc, char** argv)
{
    renderer::Drawer d;
    Chess::board_t b;
    d.init(624, 624, b);
    showStartupBox();
    bool gameover = false;

    //If user is a server
    if(server)
    {  
        //for(;;) //Loop waiting for a client to connect
        //{
            //tcp::socket socket(ioContext); //Open a new socket
            //serverAcceptor.accept(socket); //Await a connection to the socket
            //asio::write(socket, asio::buffer(test));

        //}
    }
    else //If user is a client
    {
        try
        {
            if (argc != 2)
            {
            std::cerr << "Usage: client <host>" << std::endl;
            //return 1;
            }

            asio::io_context io_context;

            tcp::resolver resolver(io_context);
            tcp::resolver::results_type endpoints =
            resolver.resolve("10.0.0.24", "daytime");

            tcp::socket socket(io_context);
            asio::connect(socket, endpoints);

        }
        catch(std::exception& e)
        {
            std::cerr<<e.what()<<std::endl;
        }   
    }
    
    tcp::acceptor serverAcceptor(ioContext, tcp::endpoint(tcp::v4(), 8080)); //Listen for IPv4 requests on port 8080

    tcp::socket socket(ioContext); //Open a new socket
    serverAcceptor.accept(socket); //Await a connection to the socket

    std::string sent = " ";
    while(d.running)
    {
        d.input(b);
        d.drawBoard(b);
        /*If another move was made, send it*/
        if(storedBMove != b.bMoveString)
        {
            storedBMove = b.bMoveString;
            asio::write(socket, asio::buffer(b.bMoveString));
        }
        else if(storedWMove != b.wMoveString)
        {
            storedWMove = b.wMoveString;
            asio::write(socket, asio::buffer(b.wMoveString));
        }
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