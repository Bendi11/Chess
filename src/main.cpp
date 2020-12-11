/*
Chess by Benjamin L (Bendi11)
Chess game made with C++
Compile commands: 
Windows: 
g++ -o bin/Chess.exe src/main.cpp src/Chess.cpp src/render.cpp -Isrc/include -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -static-libgcc -static-libstdc++ -mwindows icon.o

Linux: 
g++ -o bin/Chess src/main.cpp src/Chess.cpp src/render.cpp -Isrc/include -lSDL2main -lSDL2 -lSDL2_image -static-libgcc -static-libstdc++ icon.o
*/

#define PORT 23
#include "include/Chess.hpp"
#include "include/render.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>

int socketID; //Socket ID
int otherSocket; //Client socket ID
struct sockaddr_in sockAddress; //The address of the socket
int addrlen = sizeof(sockAddress); //How long the address is
char recievedStr[1024]; //Recieved string


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
        {/*Flags,   buttonid, text*/ 0, 0, "Connect to another player's game"},
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
        /*Load the server IP from a file*/
        std::ifstream connectFile("assets/connect.txt");
        std::string servIP;
        std::getline(connectFile, servIP);

        otherSocket = socket(AF_INET, SOCK_STREAM, 0); //Create a new socket
        struct sockaddr_in serverAddress; //Server address structure
        serverAddress.sin_family = AF_INET; //Use IPV4
        serverAddress.sin_port = htons(PORT); //Set the port to 23
        inet_pton(AF_INET, servIP.c_str(), &serverAddress.sin_addr); //Set the IP to the read IP
        connect(otherSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)); //Connect to the host
        
    }
    else if(buttonID == 1) //User decided to host a game
    {
        socketID = socket(AF_INET, SOCK_STREAM, 0); //Create a new socket
        int opt = 1;
        if(setsockopt(socketID, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

        sockAddress.sin_family = AF_INET; //IPv4
        sockAddress.sin_addr.s_addr = INADDR_ANY; //Accept any connection
        sockAddress.sin_port = htons(PORT); //Set the port to 23, commonly used for telenet

        bind(socketID, (struct sockaddr *)&sockAddress, sizeof(sockAddress)); //Bind the socket to the right port
        listen(socketID, 2); //Start listening on the socket
        otherSocket = accept(socketID, (struct sockaddr *)&sockAddress, (socklen_t *)&addrlen); //Await a connection request

    }

} 



int main(int argc, char** argv)
{
    renderer::Drawer d;
    Chess::board_t b;
    d.init(624, 624, b);
    showStartupBox();
    bool gameover = false;

    while(d.running)
    {
        d.input(b);
        d.drawBoard(b);
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