/*
File used for defining functions for pieces like move,
update attackable squares, etc.
*/
#include "include/Chess.hpp"
using namespace Chess;

//Function to init a piece with position and type
piece_t::piece_t(uint8_t _type)
{
    type = _type;
}

//Function to start a chess game with the default chess positions and 8x8 grid
board_t::board_t()
{
    unsigned int i; //Iterator variable

    container.resize(8); //Resize container x to 8
    for(i = 0; i < 8; ++i)
    {
        container[i].resize(8); //Resize each y to 8
    }

    //Start placing white pieces in their default locations
    container[board_a][board_1].type = wROOK; //Place rook at A1
    container[board_b][board_1].type = wKNIGHT; //Place knight at B1
    container[board_c][board_1].type = wBISHOP; //Place bishop at C1
    container[board_d][board_1].type = wQUEEN; //Place queen at D1
    container[board_e][board_1].type = wKING; //Place king at E1
    container[board_f][board_1].type = wBISHOP; //Place bishop at F1
    container[board_g][board_1].type = wKNIGHT; //Place knight at G1
    container[board_h][board_1].type = wROOK; //Place rook at H1
    for(i = 0; i < 8; ++i) //Place eight pawns
    {
        container[i][board_2].type = wPAWN;
    }

    //Start placing black pieces in their default locations
    container[board_a][board_8].type = wROOK; //Place rook at A8
    container[board_b][board_8].type = wKNIGHT; //Place knight at B8
    container[board_c][board_8].type = wBISHOP; //Place bishop at C8
    container[board_d][board_8].type = wQUEEN; //Place queen at D8
    container[board_e][board_8].type = wKING; //Place king at E8
    container[board_f][board_8].type = wBISHOP; //Place bishop at F8
    container[board_g][board_8].type = wKNIGHT; //Place knight at G8
    container[board_h][board_8].type = wROOK; //Place rook at H8
    for(i = 0; i < 8; ++i) //Place eight pawns
    {
        container[i][board_7].type = wPAWN;
    }
}

//Function to find where a piece can move on the board
