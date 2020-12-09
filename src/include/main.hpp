#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include <fstream>

#include "defs.hpp"

typedef uint64_t bitBoard; //Bit board of 8x8 board is a 64 bit integer

//Structure that will hold all data pertaining to a piece on the board
typedef struct piece_t
{
    uint8_t type; //What type of piece the piece is (defined in defs.hpp)
    SDL_Texture* icon = NULL; //What icon this piece has

    bitBoard pos; //Position bitboard, 1 on where the piece is
    bitBoard canMove; //Where the piece can move
    bitBoard canAttack; //What pieces the piece can attack

    piece_t(bitBoard _pos, uint8_t _type); //Function used to initialize a piece
    piece_t() {} //Default constructor
    void loadIcon(std::string path, SDL_Renderer* render); //Function to load a texture from a PNG file
    
} piece_t;

//Structure holding all pieces on the board
class board_t
{
public:

    //Bitboard collections
    bitBoard allPieces; //All pieces on the board
    bitBoard wPieces; //All white pieces
    bitBoard bPieces; //All black pieces

    std::vector<piece_t> wPawns; //List of all white pawns
    std::vector<piece_t> bPawns; //List of all black pawns

    std::vector<piece_t> wRooks; //List of both white rooks
    std::vector<piece_t> bRooks; //List of all black rooks

    std::vector<piece_t> wBishops; //List of all white bishops
    std::vector<piece_t> bBishops; //List of all black bishops

    std::vector<piece_t> wKnights; //List of all white knights
    std::vector<piece_t> bKnights; //List of all black knights

    piece_t wKing; //White king
    piece_t wQueen; //White queen

    piece_t bKing; //Black king
    piece_t bQueen; //Black queen

    //----------Functions----------//

    void init(); //Function used to set all pieces with right values and in right spots
    board_t() {} //Default constructor
};

//Class that will hold the chess game logic, input, and display functions
class chessGame
{
public:

    /*SDL2 things*/
    SDL_Window* win = NULL; //Window object of the (duh) window
    SDL_Renderer* render = NULL; //The object that will draw everything onscreen for us
    SDL_Event userE; //SDL user event queue
    bool IS_RUNNING = true;
    double SCALING_X; //How much the icons should be scaled, assuming a resolution of 77x77 for sprites
    double SCALING_Y; //How much the icons should be scaled in height, assuming 77x77 resolution

    std::ofstream logger; //Logger file object for logging errors / messages

    board_t Board; //The board object that contains all pieces
    chessGame() {} //Default constructor


    //--------------------Functions--------------------//

    void init(); //Function to init SDL2 and other values

    /*File loading functions*/
    void loadCfg(); //Function to load config values from a CFG file

    void getInput(); //Function to get user input

    void renderUpdate(); //Function to display what game logic updates
    void renderBG(); //Function to render the board graphic

    void logicUpdate(); //Function to take in player moves like ke5 and actually change values


    //--------------------Values to be set by cfg file and never changed again--------------------//
    unsigned int SCREEN_WIDTH; //Screen width in pixels
    unsigned int SCREEN_HEIGHT; //Screen height in pixels
    bool FULLSCREEN; //Fullscreen or not


};

