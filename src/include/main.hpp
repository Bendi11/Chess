#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>

typedef std::vector<std::vector <double > > mat; //Defining the matrix type so that I don't have to type this over and over


//Structure that will hold all data pertaining to a piece on the board
typedef struct piece_t
{
    std::string name; //Name of the piece
    unsigned int x; //Piece x coordinate
    unsigned int y; //Piece y coordinate

    SDL_Texture* icon; //Icon of the piece
    bool loadIcon(std::string& path); //Function to load the icon from hard drive
    
    bool movePiece(); //Function to try to move the piece and return if it can move there
} piece_t;

//Class that will hold the chess game logic, input, and display functions
class chessGame
{
public:

    /*SDL2 things*/
    SDL_Window* win = NULL; //Window object of the (duh) window
    SDL_Renderer* render = NULL; //The object that will draw everything onscreen for us
};