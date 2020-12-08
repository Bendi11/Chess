#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include <fstream>

#include "defs.hpp"

typedef std::vector<std::vector <double > > mat; //Defining the matrix type so that I don't have to type this over and over
typedef std::vector<std::vector < piece_t > > pMat; //Defining matrix of pieces as a board type


//Structure that will hold all data pertaining to a piece on the board
typedef struct piece_t
{
    unsigned int type; //What type of piece the piece is (defined in defs.hpp)
    unsigned int x; //Piece x coordinate
    unsigned int y; //Piece y coordinate

    SDL_Texture* icon; //Icon of the piece
    piece_t(unsigned int iType); //Constructor for settting up type of piece and loading icon
    
} piece_t;

//Structure holding all pieces on the board
typedef struct board_t
{
    std::vector<double> bitBoard; //Bit board as input to the NN
    pMat board; //8x8 matrix for the actual board
    std::string PGN; //PGN file for later

    void init(); //Function to init the chess board with all pieces
    bool movePiece(); //Function to try to move a piece and return if it can move there

} board_t;

//Class that will hold the chess game logic, input, and display functions
class chessGame
{
public:

    /*SDL2 things*/
    SDL_Window* win = NULL; //Window object of the (duh) window
    SDL_Renderer* render = NULL; //The object that will draw everything onscreen for us


    //--------------------Functions--------------------//

    void init(); //Function to init SDL2 and other values

    /*File loading functions*/
    void loadCfg(); //Function to load config values from a CFG file


    //--------------------Values to be set by cfg file and never changed again--------------------//
    unsigned int SCREEN_WIDTH; //Screen width in pixels
    unsigned int SCREEN_HEIGHT; //Screen height in pixels
    bool FULLSCREEN; //Fullscreen or not


};