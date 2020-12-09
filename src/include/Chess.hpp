#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include <fstream>

#include "defs.hpp"



//Chess namespace containing everything chess
namespace Chess
{
    
    class piece_t //Piece containing information like location on board 
    {
    public:
        uint8_t type; //ID of the type of piece

        /*Position on the board*/
        unsigned int x; 
        unsigned int y;

        piece_t(uint8_t _type); //Constructor for initializing a new piece
    };


    typedef std::vector< std::vector< piece_t > > pMat; //2D matrix of pieces for the board

    class board_t //Board holding all pieces and functions for moving them
    {
    public:
        pMat container; //The object holding all pieces on the board

        /*Functions*/

        board_t(void); //One constructor for making a default chess board
        board_t(unsigned int w, unsigned int h); //Function for making a variable height and width grid

        bool move(unsigned int x, unsigned int y, unsigned int moveX, unsigned int moveY); //Function to move a piece from one spot to another
    };
}

