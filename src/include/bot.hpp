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
        const double Kwt = 20000;


        std::pair<std::pair<unsigned int, unsigned int>, std::pair<unsigned int, unsigned int> > bestMove; //The best evauluated move 

        void makeMove(Chess::board_t& Board, bool graphic, renderer::Drawer& d); //Function to make a move on the chess board

        moveScore miniMax(Chess::board_t& Board, int depth); //Function to minimax the position
        moveScore graphicMiniMax(Chess::board_t& Board, int depth, renderer::Drawer& d, bool WHITE, double alpha, double beta); //Function to minimax each possible possition and display them
        double evaluate(Chess::board_t& Board); //Function to evaluate a chess position and return a score
    };
}