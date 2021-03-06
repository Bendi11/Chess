#pragma once
#include <string>
#include <vector>
#include <array>
#include <memory>

namespace Chess
{
    struct Piece; 

    /**
     * @brief Structure containing array of chess pieces on the 
     simulated chess board, with methods to represent pieces as a bitboard,
     record all moves to send to Stockfish, etc.
     * 
     */
    struct Board
    {
        std::array<std::array<std::unique_ptr<Piece>, 8>, 8> board; //Internal 8x8 board containing all pieces, allocated on stack for fast access

    };

    /**
     * @brief Base struct for all chess pieces, used to 
     store chess pieces on a board regardless of type and 
     compute moves differently
     * 
     */
    struct Piece
    {
        bool white; //If this piece is white or black

        //Enumeration enumerating all possible types of piece that this is
        enum Kind
        {
            PAWN,
            KNIGHT,
            BISHOP,
            ROOK,
            QUEEN,
            KING
        };

        Kind type; //What type of piece this is

        /**
         * @brief Method to find all moves for this piece on a board,
         MUST be called before calling move() method
         * 
         * @param board The immutable board reference to get piece location info from
         */
        virtual void find_moves(const Board& board) = 0; 

    };


}

