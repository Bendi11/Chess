#pragma once
#include "Chess.hpp"
#include "render.hpp"
#include <math.h>
#include <random>
#include <time.h>

namespace Bot //Namespace containing all bot objects
{
    typedef std::pair<double, std::pair<std::pair<unsigned int, unsigned int>, std::pair<unsigned int, unsigned int> > > moveScore; //Type for a move and score

    class computerEnemy //Class containing the enemy functions and values
    {
    public:

        const double Pwt = 200;
        const double Nwt = 320;
        const double Bwt = 330;
        const double Rwt = 500;
        const double Qwt = 900;
        const double Kwt = 200000;

        //Where pawns are encouraged to go
        double wPawnTable[8][8] = 
        {
            {0,  0,  0,  0,  0,  0,  0,  0},
            {50, 50, 50, 50, 50, 50, 50, 50},
            {10, 10, 20, 30, 30, 20, 10, 10},
            {5,  5, 10, 25, 25, 10,  5,  5},
            {0,  0,  0, 20, 20,  0,  0,  0},
            {5, -5,-10,  0,  0,-10, -5,  5},
            {5, 10, 10,-20,-20, 10, 10,  5},
            {0,  0,  0,  0,  0,  0,  0,  0}
        };
        double bPawnTable[8][8] = 
        {
            {0,  0,  0,  0,  0,  0,  0,  0},
            {5, 10, 10,-20,-20, 10, 10,  5},
            {5, -5,-10,  0,  0,-10, -5,  5},
            {0,  0,  0, 20, 20,  0,  0,  0},
            {5,  5, 10, 25, 25, 10,  5,  5},
            {10, 10, 20, 30, 30, 20, 10, 10},
            {50, 50, 50, 50, 50, 50, 50, 50},
            {0,  0,  0,  0,  0,  0,  0,  0}
        };

        double wKnightTable[8][8] = 
        {
            {-50,-40,-30,-30,-30,-30,-40,-50,},
            {-40,-20,  0,  0,  0,  0,-20,-40,},
            {-30,  0, 10, 15, 15, 10,  0,-30,},
            {-30,  5, 15, 20, 20, 15,  5,-30,},
            {-30,  0, 15, 20, 20, 15,  0,-30,},
            {-30,  5, 10, 15, 15, 10,  5,-30,},
            {-40,-20,  0,  5,  5,  0,-20,-40,},
            {-50,-40,-30,-30,-30,-30,-40,-50,}
        };
        double bKnightTable[8][8] = 
        {
            {-50,-40,-30,-30,-30,-30,-40,-50,},
            {-40,-20,  0,  5,  5,  0,-20,-40,},
            {-30,  5, 10, 15, 15, 10,  5,-30,},
            {-30,  0, 15, 20, 20, 15,  0,-30,},
            {-30,  5, 15, 20, 20, 15,  5,-30,},
            {-30,  0, 10, 15, 15, 10,  0,-30,},
            {-40,-20,  0,  0,  0,  0,-20,-40,},
            {-50,-40,-30,-30,-30,-30,-40,-50,}
        };

        double wBishopTable[8][8] = 
        {
            {-20,-10,-10,-10,-10,-10,-10,-20,},
            {-10,  0,  0,  0,  0,  0,  0,-10,},
            {-10,  0,  5, 10, 10,  5,  0,-10,},
            {-10,  5,  5, 10, 10,  5,  5,-10,},
            {-10,  0, 10, 10, 10, 10,  0,-10,},
            {-10, 10, 10, 10, 10, 10, 10,-10,},
            {-10,  5,  0,  0,  0,  0,  5,-10,},
            {-20,-10,-10,-10,-10,-10,-10,-20,},
        };
        double bBishopTable[8][8] = 
        {
            {-20,-10,-10,-10,-10,-10,-10,-20,},
            {-10,  5,  0,  0,  0,  0,  5,-10,},
            {-10, 10, 10, 10, 10, 10, 10,-10,},
            {-10,  0, 10, 10, 10, 10,  0,-10,},
            {-10,  5,  5, 10, 10,  5,  5,-10,},
            {-10,  0,  5, 10, 10,  5,  0,-10,},
            {-10,  0,  0,  0,  0,  0,  0,-10,},
            {-20,-10,-10,-10,-10,-10,-10,-20,}
            
        };

        double wRookTable[8][8] = 
        {
            {0,  0,  0,  0,  0,  0,  0,  0,},
            {5, 10, 10, 10, 10, 10, 10,  5,},
            {-5,  0,  0,  0,  0,  0,  0, -5,},
            {-5,  0,  0,  0,  0,  0,  0, -5,},
            {-5,  0,  0,  0,  0,  0,  0, -5,},
            {-5,  0,  0,  0,  0,  0,  0, -5,},
            {-5,  0,  0,  0,  0,  0,  0, -5,},
            {0,  0,  0,  5,  5,  0,  0,  0}
        };
        double bRookTable[8][8] = 
        {
            {0,  0,  0,  5,  5,  0,  0,  0},
            {-5,  0,  0,  0,  0,  0,  0, -5,},
            {-5,  0,  0,  0,  0,  0,  0, -5,},
            {-5,  0,  0,  0,  0,  0,  0, -5,},
            {-5,  0,  0,  0,  0,  0,  0, -5,},
            {-5,  0,  0,  0,  0,  0,  0, -5,},
            {5, 10, 10, 10, 10, 10, 10,  5,},
            {0,  0,  0,  0,  0,  0,  0,  0,}
        };

        double wQueenTable[8][8] = 
        {
            {-20,-10,-10, -5, -5,-10,-10,-20,},
            {-10,  0,  0,  0,  0,  0,  0,-10,},
            {-10,  0,  5,  5,  5,  5,  0,-10,},
            {-5,  0,  5,  5,  5,  5,  0, -5,},
            {0,  0,  5,  5,  5,  5,  0, -5,},
            {-10,  5,  5,  5,  5,  5,  0,-10,},
            {-10,  0,  5,  0,  0,  0,  0,-10,},
            {-20,-10,-10, -5, -5,-10,-10,-20}
        };
        double bQueenTable[8][8] = 
        {
            {-20,-10,-10, -5, -5,-10,-10,-20},
            {-10,  0,  5,  0,  0,  0,  0,-10,},
            {-10,  5,  5,  5,  5,  5,  0,-10,},
            {0,  0,  5,  5,  5,  5,  0, -5,},
            {-5,  0,  5,  5,  5,  5,  0, -5,},
            {-10,  0,  5,  5,  5,  5,  0,-10,},
            {-10,  0,  0,  0,  0,  0,  0,-10,},
            {-20,-10,-10, -5, -5,-10,-10,-20,}

        };

        double wKingTable[8][8] = 
        {
            {-30,-40,-40,-50,-50,-40,-40,-30,},
            {-30,-40,-40,-50,-50,-40,-40,-30,},
            {-30,-40,-40,-50,-50,-40,-40,-30,},
            {-30,-40,-40,-50,-50,-40,-40,-30,},
            {-20,-30,-30,-40,-40,-30,-30,-20,},
            {-10,-20,-20,-20,-20,-20,-20,-10,},
            {20, 20,  0,  0,  0,  0, 20, 20,},
            {20, 30, 10,  0,  0, 10, 30, 20}
        };
        double bKingTable[8][8] = 
        {
            {20, 30, 10,  0,  0, 10, 30, 20},
            {20, 20,  0,  0,  0,  0, 20, 20,},
            {-10,-20,-20,-20,-20,-20,-20,-10,},
            {-20,-30,-30,-40,-40,-30,-30,-20,},
            {-30,-40,-40,-50,-50,-40,-40,-30,},
            {-30,-40,-40,-50,-50,-40,-40,-30,},
            {-30,-40,-40,-50,-50,-40,-40,-30,},
            {-30,-40,-40,-50,-50,-40,-40,-30,}
        };

        std::pair<std::pair<unsigned int, unsigned int>, std::pair<unsigned int, unsigned int> > bestMove; //The best evauluated move 

        void makeMove(Chess::board_t& Board, bool graphic, renderer::Drawer& d); //Function to make a move on the chess board

        moveScore ABMax(double alpha, double beta, Chess::board_t& Board, int depth, renderer::Drawer& d, bool graphic); 
        moveScore ABMin(double alpha, double beta, Chess::board_t& Board, int depth, renderer::Drawer& d, bool graphic);

        moveScore miniMax(Chess::board_t& Board, int depth); //Function to minimax the position
        moveScore graphicMiniMax(Chess::board_t& Board, int depth, renderer::Drawer& d, bool WHITE); //Function to minimax each possible possition and display them
        double evaluate(Chess::board_t& Board); //Function to evaluate a chess position and return a score
    };
}