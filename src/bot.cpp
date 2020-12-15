#include "include/bot.hpp"

using namespace Bot;

//Function to evaluate a chess position and return a score
double computerEnemy::evaluate(Chess::board_t& Board)
{
    /*If a side is in check, it is very bad, so just return that the board configuration is bad*/
    uint8_t check = Board.isCheck();
    if(check == WHITE_CHECK)
    {
        return 100000;
    }
    else if(check == BLACK_CHECK)
    {
        return -100000;
    }

    unsigned int numWhite = 0;
    unsigned int numBlack = 0;
    double whiteScore = 0;
    double blackScore = 0;

    uint8_t wPawns = 0;
    uint8_t wKnights = 0;
    uint8_t wBishops = 0;
    uint8_t wRooks = 0;
    uint8_t wKings = 0;
    uint8_t wQueens = 0;

    uint8_t bPawns = 0;
    uint8_t bKnights = 0;
    uint8_t bBishops = 0;
    uint8_t bRooks = 0;
    uint8_t bKings = 0;
    uint8_t bQueens = 0;

    for(unsigned x = 0; x < 8; ++x)
    {
        for(unsigned y = 0; y < 8; ++y)
        {
            switch(Board.container[x][y].type)
            {
                case wPAWN: 
                    wPawns++; //Increase number of pawns
                    whiteScore += wPawnTable[x][y] + Pwt; //Check position and add or subtract to score
                break;

                case wKNIGHT: 
                    wKnights++; 
                    whiteScore += wKnightTable[x][y] + Nwt;
                break;

                case wROOK: 
                    wRooks++; 
                    whiteScore += wRookTable[x][y] + Rwt;
                break;

                case wBISHOP: 
                    wBishops++; 
                    whiteScore += wBishopTable[x][y] + Bwt;
                break;

                case wKING: 
                    wKings++; 
                    whiteScore += wKingTable[x][y] + Kwt;
                break;

                case wQUEEN: 
                    wQueens++; 
                    whiteScore += wQueenTable[x][y] + Qwt;
                break;

                case bPAWN: 
                    bPawns++; 
                    blackScore += bPawnTable[x][y] + Pwt;
                break;

                case bKNIGHT: 
                    bKnights++; 
                    blackScore += bKnightTable[x][y] + Nwt;
                break;

                case bROOK: 
                    bRooks++; 
                    blackScore += bRookTable[x][y] + Rwt;
                break;

                case bBISHOP: 
                    bBishops++; 
                    blackScore += bBishopTable[x][y] + Bwt;
                break;

                case bKING: 
                    bKings++; 
                    blackScore += bKingTable[x][y] + Kwt;
                break;

                case bQUEEN: 
                    bQueens++; 
                    blackScore += bQueenTable[x][y] + Qwt;
                break;
            }
        }
    }
    whiteScore += (wPawns * Pwt) + (wKnights * Nwt) + (wRooks * Rwt) + (wBishops * Bwt) + (wKings * Kwt) + (wQueens * Qwt);
    blackScore += (bPawns * Pwt) + (bKnights * Nwt) + (bRooks * Rwt) + (bBishops * Bwt) + (bKings * Kwt) + (bQueens * Qwt);
    return blackScore - whiteScore;
}

//Function to make the best evaluated move 
void computerEnemy::makeMove(Chess::board_t& Board, bool graphic, renderer::Drawer& d)
{
    moveScore bestMove;
    bestMove = ABMax(-99999999, 99999999, Board, 4, d, graphic);
    std::cout<<bestMove.second.first.first<<", "<<bestMove.second.first.second<<"\t"<<bestMove.second.second.first<<","<<bestMove.second.second.second<<std::endl;
    Board.playerMove(bestMove.second.first.first, bestMove.second.first.second, bestMove.second.second.first, bestMove.second.second.second, false);
}

moveScore computerEnemy::ABMax(double alpha, double beta, Chess::board_t& Board, int depth, renderer::Drawer& d, bool graphic)
{
    moveScore ret;
    Chess::board_t testBoard = Board;

    if(depth == 0 || Board.isCheck() != NO_CHECK)
    {
        ret.first = evaluate(Board);
        return ret;
    }
    for(unsigned x = 0; x < 8; ++x)
    {
        for(unsigned y = 0; y < 8; ++y)
        {
            if(Board.container[x][y].type > WHITE_END)
            {
                for(unsigned i = 0; i < Board.container[x][y].moveable.size(); ++i)
                {
                    testBoard = Board;
                    testBoard.playerMove(x, y, Board.container[x][y].moveable[i].first, Board.container[x][y].moveable[i].second, false);
                    if(graphic) d.fastDrawBoard(testBoard);
                    double score = ABMin(alpha, beta, testBoard, depth - 1, d, graphic).first;
                    if(score >= beta)
                    {
                        ret.first = beta;
                        return ret;
                    }
                    if(score > alpha)
                    {
                        alpha = score;
                        ret.first = score;
                        ret.second.first.first = x;
                        ret.second.first.second = y;
                        ret.second.second = Board.container[x][y].moveable[i];
                    }
                }

                for(unsigned i = 0; i < Board.container[x][y].attackable.size(); ++i)
                {
                    testBoard = Board;
                    testBoard.playerMove(x, y, Board.container[x][y].attackable[i].first, Board.container[x][y].attackable[i].second, false);
                    if(graphic) d.fastDrawBoard(testBoard);
                    double score = ABMin(alpha, beta, testBoard, depth - 1, d, graphic).first;
                    if(score >= beta)
                    {
                        ret.first = beta;
                        return ret;
                    }
                    if(score > alpha)
                    {
                        alpha = score;
                        ret.first = score;
                        ret.second.first.first = x;
                        ret.second.first.second = y;
                        ret.second.second = Board.container[x][y].attackable[i];
                    }
                }
            }
            
        }
    }
    return ret;
}

moveScore computerEnemy::ABMin(double alpha, double beta, Chess::board_t& Board, int depth, renderer::Drawer& d, bool graphic)
{
    moveScore ret;
    Chess::board_t testBoard = Board;

    if(depth == 0 || Board.isCheck() != NO_CHECK)
    {
        ret.first = evaluate(Board);
        return ret;
    }
    for(unsigned x = 0; x < 8; ++x)
    {
        for(unsigned y = 0; y < 8; ++y)
        {
            if(Board.container[x][y].type <= WHITE_END && Board.container[x][y].type != EMPTY)
            {
                for(unsigned i = 0; i < Board.container[x][y].moveable.size(); ++i)
                {
                    testBoard = Board;
                    testBoard.playerMove(x, y, Board.container[x][y].moveable[i].first, Board.container[x][y].moveable[i].second, true);
                    if(graphic) d.fastDrawBoard(testBoard);
                    double score = ABMax(alpha, beta, testBoard, depth - 1, d, graphic).first;
                    if(score <= alpha)
                    {
                        ret.first = alpha;
                        return ret;
                    }
                    if(score < beta)
                    {
                        beta = score;
                        ret.first = score;
                        ret.second.first.first = x;
                        ret.second.first.second = y;
                        ret.second.second = Board.container[x][y].moveable[i];
                    }
                }

                for(unsigned i = 0; i < Board.container[x][y].attackable.size(); ++i)
                {
                    testBoard = Board;
                    testBoard.playerMove(x, y, Board.container[x][y].attackable[i].first, Board.container[x][y].attackable[i].second, true);
                    if(graphic)d.fastDrawBoard(testBoard);
                    double score = ABMax(alpha, beta, testBoard, depth - 1, d, graphic).first;
                    if(score <= alpha)
                    {
                        ret.first = alpha;
                        return ret;
                    }
                    if(score < beta)
                    {
                        beta = score;
                        ret.first = score;
                        ret.second.first.first = x;
                        ret.second.first.second = y;
                        ret.second.second = Board.container[x][y].attackable[i];
                    }
                }
            }
            
        }
    }
    return ret;
}
