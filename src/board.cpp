/*
File used for defining functions for pieces like move,
update attackable squares, etc.
*/
#include "include/main.hpp"

//Function to init a piece with position and type
piece_t::piece_t(bitBoard _pos, uint8_t _type)
{
    pos = _pos;
    type = _type;
}

void board_t::init()
{
    uint8_t i = 0; //Iterator variable for pawns (I'm to lazy to write all that out)
    bitBoard pos = H2; //Where the piece is on board
    
    /*Add all white pawn locations*/
    for(; i < 8; ++i)
    {
        pos *= 2; //Multiplying pos by 2 is equivalent to moving pos on bitboard left one column
        wPawns.push_back(piece_t(pos, wPAWN));
    }

    pos = H7; //Set first pawn location to H7
    //Add all black pawn locations
    for(i = 0; i < 8; ++i)
    {
        pos *= 2; //Multiplying pos by 2 is equivalent to moving pos on bitboard left one column
        wPawns.push_back(piece_t(pos, bPAWN));
    }

    /*
    CHESS BOARD LAYOUT
    bR  bK  bB  bQ  bKi  bB  bK  bR
    bP  bP  bP  bP  bP  bP  bP  bP
    .   .   .   .   .   .   .   .
    .   .   .   .   .   .   .   .
    .   .   .   .   .   .   .   .
    .   .   .   .   .   .   .   .
    wP  wP  wP  wP  wP  wP  wP  wP
    wR  wK  wB  wQ  wKi wB  wK  wR
    */

    bRooks.push_back(piece_t(A8, bROOK)); //Place black rook
    bKnights.push_back(piece_t(B8, bKNIGHT)); //Place black knight
    bBishops.push_back(piece_t(C8, bBISHOP)); //Place black bishop
    bQueen = piece_t(D8, bQUEEN); //Place black queen
    bKing = piece_t(E8, bKING); //Place black king
    bBishops.push_back(piece_t(F8, bBISHOP)); //Place black bishop
    bKnights.push_back(piece_t(G8, bKNIGHT)); //Place black knight
    bRooks.push_back(piece_t(H8, bROOK)); //Place black rook

    wRooks.push_back(piece_t(A1, bROOK)); //Place white rook
    wKnights.push_back(piece_t(B1, bKNIGHT)); //Place white knight
    wBishops.push_back(piece_t(C1, bBISHOP)); //Place white bishop
    wQueen = piece_t(D1, bQUEEN); //Place white queen
    wKing = piece_t(E1, bKING); //Place white king
    wBishops.push_back(piece_t(F1, bBISHOP)); //Place white bishop
    wKnights.push_back(piece_t(G1, bKNIGHT)); //Place white knight
    wRooks.push_back(piece_t(H1, bROOK)); //Place white rook

}

