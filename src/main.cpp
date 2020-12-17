/*
Chess by Benjamin L (Bendi11)
Chess game made with C++
Compile commands: 

Windows: 
g++  -o bin/Chess.exe src/main.cpp src/Chess.cpp src/render.cpp src/bot.cpp -Isrc/include  -Lbin/ -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -static-libgcc -static-libstdc++ -lws2_32 -O3  icon.o -IE:\asio-1.18.0\include

Linux: 
g++ -o bin/Chess src/main.cpp src/Chess.cpp src/render.cpp -Isrc/include -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -static-libgcc -static-libstdc++ icon.o
*/

#include "include/Chess.hpp"
#include "include/render.hpp"
#include "include/bot.hpp"

#include <asio.hpp>
#include <thread>
#include <thread>

std::string wMoveString = " ";
std::string bMoveString = " ";
std::string recordString; //Recorded match
unsigned int botDifficulty; //Stockfish difficulty
unsigned int botTime; //How much time the bot gets to decide a move
unsigned int botContempt; //How aggressive the bot will be 
bool limitStrength; //If the engine should limit its strength to meet elo requirement


/*--------------------NETWORKING OBJECTS--------------------*/
using asio::ip::tcp;

asio::io_context ioContext;

std::string storedBMove = " ";
std::string storedWMove = " ";
tcp::acceptor servAccept(ioContext, tcp::endpoint(tcp::v4(), 8080)); //Server request acceptor   

bool server = true; //If the user is hosting
bool isOnline = true; //If the player is playing on LAN

//Function to be run in parallel with the game logic to check for messages being sent
static void netThread(Chess::board_t& Board, bool server, tcp::socket& socket)
{
    char buf[128] = {0}; 
    std::vector<char> buffer(128); 
    asio::error_code err; //Error code for ASIO
    
    for(;;)
    {
        /*Read the sent message*/
        size_t n = socket.read_some(asio::buffer(buffer), err);
        /*Quit the application if the other user disconnected*/
        if(err == asio::error::connection_reset)
        {
            exit(EXIT_SUCCESS);
        }
        std::cout<<std::string(buffer.begin(), buffer.end())<<std::endl;
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
        {0, 1, "Host a game"},
        {0, 2, "Computer"}
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
    else if(buttonID == 2)
    {
        isOnline = false;
        server = true;
        //Displaying a message box asking for difficulty
        const SDL_MessageBoxButtonData buttonsDiff[] = 
        {
             {0, 0, "Beginner"},
            {/*Flags,   buttonid, text*/ 0, 1, "Easy"},
            {0, 2, "Fun"},
            {0, 3, "Medium"},
            {0, 4, "Hard"},
            {0, 5, "Grandmaster"}
        };
        const SDL_MessageBoxData messageDataDiff = 
        {
            SDL_MESSAGEBOX_COLOR_TEXT,
            NULL,
            "Chess",
            "Select a difficulty",
            SDL_arraysize(buttonsDiff),
            buttonsDiff,
            &colorScheme
        };
        
        SDL_ShowMessageBox(&messageDataDiff, &buttonID); //Display the message
        //Check what button was pressed
        switch(buttonID)
        {
            case 0: botDifficulty = 500; botTime = 5; botContempt = 2; limitStrength = true; break;
            case 1: botDifficulty = 700; botTime = 2; botContempt = 10; limitStrength = true; break;
            case 2: botDifficulty = 1350; botTime = 4; botContempt = 20; limitStrength = true; break;
            case 3: botDifficulty = 1500; botTime = 5; botContempt = 24; limitStrength = true; break;
            case 4: botDifficulty = 1750; botTime = 10; botContempt = 24; limitStrength = false; break;
            case 5: botDifficulty = 2300; botTime = 30; botContempt = 24; limitStrength = false; break;
        }
    }

} 

int main(int argc, char** argv)
{
    //Init all objects
    Chess::board_t b;
    Bot::computerEnemy e;
    renderer::Drawer d;
    showStartupBox();

    std::ofstream fishFile("move.txt"); //File used for communication between processes
    
    d.init(624, 624, b); 
    bool gameover = false;
    d.WHITEORBLACK = true;

    tcp::socket socket(ioContext); //Make a new socket

    if(isOnline)
    {
        if(server)
        {
            //Get IP of localhost eg. the host's IP
            std::string localIPs = "Type this number on the other computer: ";
            tcp::resolver resolver(ioContext); //Resolver to resolve IP from name
            tcp::resolver::query query(asio::ip::host_name(), ""); //Query for the resolver
            tcp::resolver::iterator iter = resolver.resolve(query); //Resolve the query
            tcp::resolver::iterator end; // End marker.
            while (iter != end) //Iterate through the list of IPs received from query
            {
                tcp::endpoint ep = *iter++; 
                if(ep.address().is_v4()) //If the IP is not an IPv6...
                {
                    localIPs.append(ep.address().to_string()); //Add it to the message box
                    localIPs.append(", ");
                }
            }
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Chess Hosting", localIPs.c_str(), d.win); //Show the user what to paste to the other computer
            servAccept.accept(socket); //Await a connection request
        } 
        else 
        {
            std::string ip = d.getTextInput(); //Prompt the user to enter an IP address
            tcp::endpoint ep(asio::ip::address::from_string(ip), 8080); //Convert to an IP and attempt a connection
            socket.connect(ep);
        }
        std::thread netThread1(netThread, std::ref(b), server, std::ref(socket)); //Start the receiving network thread
    }
    d.WHITEORBLACK = server; //Set the player's color based on server and host
    uint8_t result;

    while(d.running)
    {
        if(b.WINNER != WINNER_NONE)
        {
            if(b.WINNER == WINNER_WHITE)
            {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_COLOR_TEXT, "Chess", "White won by checkmate!", d.win); //Display that white won the game
                b.restart(); //Reset all chess board values and positions
                //Clear the communication file
                fishFile.open("move.txt", std::ofstream::out | std::ofstream::trunc);
                fishFile.close(); 
                recordString.clear(); //Clear the PGN recording, effectively restarting Stockfish
                gameover = false; //Reset gameover
            }
            else if(b.WINNER == WINNER_BLACK)
            {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_COLOR_TEXT, "Chess", "Black won by checkmate!", d.win); //Display that black won the game
                b.restart(); //Reset all chess board values and positions
                //Clear the communication file
                fishFile.open("move.txt", std::ofstream::out | std::ofstream::trunc);
                fishFile.close(); 
                recordString.clear(); //Clear the PGN recording, effectively restarting Stockfish
                gameover = false; //Reset gameover
            }
            else if(b.WINNER == STALEMATE)
            {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_COLOR_TEXT, "Chess", "Draw by stalemate!", d.win); //Display that black won the game
                b.restart(); //Reset all chess board values and positions
                //Clear the communication file
                fishFile.open("move.txt", std::ofstream::out | std::ofstream::trunc);
                fishFile.close(); 
                recordString.clear(); //Clear the PGN recording, effectively restarting Stockfish
                gameover = false; //Reset gameover
            }
        }
    
        d.input(b);
        d.drawBoard(b);
        
        if(wMoveString != b.wMoveString) //If a move was made and we don't know about it...
        {
            wMoveString = b.wMoveString;
            if(!isOnline) //Send the move string to Stockfish
            {
                //Clear the file
                fishFile.open("move.txt", std::ofstream::out | std::ofstream::trunc);
                fishFile.close();
                fishFile.open("move.txt", std::ofstream::out);
                fishFile<<"uci"<<std::endl;

                fishFile<<"position startpos move ";
                recordString.append(b.wPGN); //Add to the recorded string 
                recordString.append(" ");
                fishFile<<recordString<<std::endl;
                fishFile.close();
                b.writePGN("assets/Record/test.txt");
            }    
        }
        
        if(isOnline)
        {
            if(storedBMove != b.bMoveString && !server)
            {
                storedBMove = b.bMoveString;
                asio::write(socket, asio::buffer(b.bMoveString)); //Write our move to the socket
            }
            else if(storedWMove != b.wMoveString && server) //Check if we need to send a move and we are white
            {
                storedWMove = b.wMoveString;
                asio::write(socket, asio::buffer(b.wMoveString));
            }
        }
        else
        {
            if(b.counter % 2) e.stockfishMove(b, recordString, fishFile, botDifficulty, d, botTime, botContempt, limitStrength); //Make a move by Stockfish
        }

        if(b.WINNER != WINNER_NONE) 
        {
            if(b.WINNER == WINNER_WHITE)
            {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_COLOR_TEXT, "Chess", "White won by checkmate!", d.win); //Display that white won the game
                b.restart(); //Reset all chess board values and positions
                //Clear the communication file
                fishFile.open("move.txt", std::ofstream::out | std::ofstream::trunc);
                fishFile.close(); 
                recordString.clear(); //Clear the PGN recording, effectively restarting Stockfish
                gameover = false; //Reset gameover
            }
            else if(b.WINNER == WINNER_BLACK)
            {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_COLOR_TEXT, "Chess", "Black won by checkmate!", d.win); //Display that black won the game
                b.restart(); //Reset all chess board values and positions
                //Clear the communication file
                fishFile.open("move.txt", std::ofstream::out | std::ofstream::trunc);
                fishFile.close(); 
                recordString.clear(); //Clear the PGN recording, effectively restarting Stockfish
                gameover = false; //Reset gameover
            }
            else if(b.WINNER == STALEMATE)
            {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_COLOR_TEXT, "Chess", "Draw by stalemate!", d.win); //Display that black won the game
                b.restart(); //Reset all chess board values and positions
                //Clear the communication file
                fishFile.open("move.txt", std::ofstream::out | std::ofstream::trunc);
                fishFile.close(); 
                recordString.clear(); //Clear the PGN recording, effectively restarting Stockfish
                gameover = false; //Reset gameover
            }
        }
    
    }
    
    SDL_DestroyRenderer(d.render); //Destroy SDL2 renderer
    SDL_DestroyWindow(d.win); //Destroy SDL2 window
    socket.close(); //Close the socket
    SDL_CloseAudio(); //Close SDL2 audio device
    SDL_FreeWAV(d.wavBuffer); //Free the WAV storage
    IMG_Quit(); //Quit SDL2_IMG
    TTF_Quit(); //Quit SDL2_TTF
    SDL_Quit(); //Finally, quit SDL2
    
    return 0;
}
