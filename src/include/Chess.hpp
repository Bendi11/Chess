#pragma once
#include <string>
#include <vector>

#include <iostream> //Take this out


#include "defs.hpp"



//Chess namespace containing everything chess
namespace Chess
{
    
    class piece_t //Piece containing information like location on board 
    {
    public:
        uint8_t type; //ID of the type of piece

        std::vector<std::pair<unsigned int, unsigned int > > moveable; //List of tiles the piece can move to
        std::vector<std::pair<unsigned int, unsigned int> > attackable; //List of tiles the piece can attack

        piece_t(uint8_t _type); //Constructor for initializing a new piece
        piece_t(void) { type = EMPTY; }; //Default constructor is empty
    };


    typedef std::vector< std::vector< piece_t > > pMat; //2D matrix of pieces for the board

    class board_t //Board holding all pieces and functions for moving them
    {
    public:
        pMat container; //The object holding all pieces on the board
        unsigned int counter; //How many turns the game has gone for

        unsigned int sizeX = 7;
        unsigned int sizeY = 7;

        /*Functions*/

        board_t(void); //One constructor for making a default chess board
        board_t(unsigned int w, unsigned int h); //Function for making a variable height and width grid

        bool move(unsigned int x, unsigned int y, unsigned int moveX, unsigned int moveY); //Function to move a piece from one spot to another
        void findMoves(unsigned int x, unsigned int y); //Function to find which tiles a certain piece can move to

        //Functions for finding moves for each type of piece
        void findPawn(unsigned int x, unsigned int y, bool WHITE); //Function to find black or white pawn's moves

        void findRook(unsigned int x, unsigned int y, bool WHITE); //Function to find black or white rook's moves

        void findKnight(unsigned int x, unsigned int y, bool WHITE); //Function to find black or white knight's moves

        void findBishop(unsigned int x, unsigned int y, bool WHITE); //Function to find black or white bishop's moves

        void findKing(unsigned int x, unsigned int y, bool WHITE); //Function to find black or white king's moves

        void findQueen(unsigned int x, unsigned int y, bool WHITE); //Function to find black or white queen's moves

    };
}
