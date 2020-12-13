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
#include <asio.hpp>
#include <iostream>
#include <fstream>
#include <thread>

using asio::ip::tcp;

asio::io_context ioContext;
asio::io_service ioService;

std::string storedBMove = " ";
std::string storedWMove = " ";
std::string received;
tcp::acceptor servAccept(ioContext, tcp::endpoint(tcp::v4(), 23)); //Server request acceptor   
tcp::resolver resolver(ioContext); //Used for resolving host names for client
tcp::endpoint ep(asio::ip::address::from_string("192.168.1.73"), 8080);


bool server = true;
bool receivedMessage = false;

//Function to be run in parallel with the game logic to check for messages being sent
static void netThread(Chess::board_t& Board, bool server, tcp::socket& socket)
{
    char buf[128] = {0}; 
    std::vector<char> buffer(128); 
    
    if(server)
    {
        for(;;)
        {
            /*Read the sent message*/
            size_t n = socket.read_some(asio::buffer(buffer));
            if(buffer[0] != 'E')
            {
                if(!server) //Make a move for white if the player is black
                {
                    Board.playerMove(buffer[2] - '0', buffer[4] - '0', buffer[7] - '0', buffer[9] - '0', true);
                }
                else //make a move for black if the player is white
                {
                    Board.playerMove(buffer[2] - '0', buffer[4] - '0', buffer[7] - '0', buffer[9] - '0', false);
                }
            }
        }
    }
    else
    {
        

        for(;;)
        {
            /*Read the sent message*/
            size_t n = socket.read_some(asio::buffer(buffer));
            std::cout<<std::string(buffer.begin(), buffer.end())<<std::endl;
            if(buffer[0] != 'E')
            {
                if(!server) //Make a move for white if the player is black
                {
                    Board.playerMove(buffer[2] - '0', buffer[4] - '0', buffer[7] - '0', buffer[9] - '0', true);
                }
                else //make a move for black if the player is white
                {
                    Board.playerMove(buffer[2] - '0', buffer[4] - '0', buffer[7] - '0', buffer[9] - '0', false);
                }
            }
            
        }
    }
    
    
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
    //Init all objects
    Chess::board_t b;
    renderer::Drawer d;
    d.init(624, 624, b);
    bool gameover = false;

    showStartupBox();

    tcp::socket socket(ioContext); //Make a new socket
    if(server) servAccept.accept(socket); //Await a connection request
    else socket.connect(ep);

    d.WHITEORBLACK = server; //Set the player's color based on server and host
    std::thread netThread1(netThread, std::ref(b), server, std::ref(socket)); //Start the receiving network thread

    while(d.running)
    {
        d.drawBoard(b);
        d.input(b);
        //Check if we need to send that we just made a move and are black
        if(storedBMove != b.bMoveString && !server)
        {
            storedBMove = b.bMoveString;
            asio::write(socket, asio::buffer(b.bMoveString));
        }
        else if(storedWMove != b.wMoveString && server) //Check if we need to send a move and we are white
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
    

    //d.WHITEORBLACK = isWhite; //Set if the player is white or black
    d.WHITEORBLACK = false;
    
    
    asio::write(socket, asio::buffer("END"));
    socket.close();

    SDL_DestroyRenderer(d.render);
    SDL_DestroyWindow(d.win);
    SDL_CloseAudio();
    SDL_FreeWAV(d.wavBuffer);
    IMG_Quit();
    SDL_Quit();
    
    return 0;
}