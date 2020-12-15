#include "include/bot.hpp"

using namespace Bot;

//Function to evaluate a chess position and return a score
double computerEnemy::evaluate(Chess::board_t& Board)
{
    unsigned int numWhite = 0;
    unsigned int numBlack = 0;

    for(unsigned x = 0; x < 8; ++x)
    {
        for(unsigned y = 0; y < 8; ++y)
        {
            if(Board.container[x][y].type <= WHITE_END && Board.container[x][y].type != EMPTY)
            {
                numWhite++;
            }
            else if(Board.container[x][y].type > WHITE_END)
            {
                numBlack++;
            }
        }
    }
    return (numBlack - numWhite);
}

//Function to return the best move and score of that move
moveScore computerEnemy::miniMax(Chess::board_t& Board, int depth)
{
    moveScore bestTestMove;
    Chess::board_t testBoard = Board; //Chess board for testing

    if(depth == 0)
    {
        bestTestMove.first = evaluate(Board);
        return bestTestMove;
    }
    
    if(Board.counter % 2) //Black's move
    {
        bestTestMove.first = -99999999999;
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
                        double score = miniMax(testBoard, depth - 1).first;
                        if(score > bestTestMove.first)
                        {
                            bestTestMove.first = score;
                            bestTestMove.second.first.first = x;
                            bestTestMove.second.first.second = y;
                            bestTestMove.second.second.first = Board.container[x][y].moveable[i].first;
                            bestTestMove.second.second.second = Board.container[x][y].moveable[i].second;
                        }
                    }
                    for(unsigned i = 0; i < Board.container[x][y].attackable.size(); ++i)
                    {
                        testBoard = Board;
                        testBoard.playerMove(x, y, Board.container[x][y].attackable[i].first, Board.container[x][y].attackable[i].second, false);
                        double score = miniMax(testBoard, depth - 1).first;
                        if(score > bestTestMove.first)
                        {
                            bestTestMove.first = score;
                            bestTestMove.second.first.first = x;
                            bestTestMove.second.first.second = y;
                            bestTestMove.second.second.first = Board.container[x][y].attackable[i].first;
                            bestTestMove.second.second.second = Board.container[x][y].attackable[i].second;
                        }
                    }
                }
                
            }
        }
        return bestTestMove;
    }
    else //White's move
    {
        bestTestMove.first = -99999999999;
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
                        double score = miniMax(testBoard, depth - 1).first;
                        if(score < bestTestMove.first)
                        {
                            bestTestMove.first = score;
                            bestTestMove.second.first.first = x;
                            bestTestMove.second.first.second = y;
                            bestTestMove.second.second.first = Board.container[x][y].moveable[i].first;
                            bestTestMove.second.second.second = Board.container[x][y].moveable[i].second;
                        }
                    }
                    for(unsigned i = 0; i < Board.container[x][y].attackable.size(); ++i)
                    {
                        testBoard = Board;
                        testBoard.playerMove(x, y, Board.container[x][y].attackable[i].first, Board.container[x][y].attackable[i].second, true);
                        double score = miniMax(testBoard, depth - 1).first;
                        if(score < bestTestMove.first)
                        {
                            bestTestMove.first = score;
                            bestTestMove.second.first.first = x;
                            bestTestMove.second.first.second = y;
                            bestTestMove.second.second.first = Board.container[x][y].attackable[i].first;
                            bestTestMove.second.second.second = Board.container[x][y].attackable[i].second;
                        }
                    }
                }
                
            }
        }
        return bestTestMove;
    
    }
}

//Function to make the best evaluated move 
void computerEnemy::makeMove(Chess::board_t& Board, bool graphic, renderer::Drawer& d)
{
    moveScore bestMove;
    if(!graphic) bestMove = miniMax(Board, 2);
    else bestMove = graphicMiniMax(Board, 3, d, false, -9999999, 9999999);
    std::cout<<bestMove.second.first.first<<", "<<bestMove.second.first.second<<"\t"<<bestMove.second.second.first<<","<<bestMove.second.second.second<<std::endl;
    Board.playerMove(bestMove.second.first.first, bestMove.second.first.second, bestMove.second.second.first, bestMove.second.second.second, false);
}

moveScore computerEnemy::graphicMiniMax(Chess::board_t& Board, int depth, renderer::Drawer& d, bool WHITE, double alpha, double beta)
{
    moveScore bestTestMove;
    Chess::board_t testBoard = Board; //Chess board for testing

    if(depth == 0)
    {
        bestTestMove.first = evaluate(Board);
        return bestTestMove;
    }
    
    if(!WHITE) //Black's move
    {
        bestTestMove.first = -99999999999;
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
                        d.fastDrawBoard(testBoard);
                        double score = graphicMiniMax(testBoard, depth - 1, d, true, alpha, beta).first;
                        if(score >= beta) //If the branch fails...
                        {
                            bestTestMove.first = beta;
                            return bestTestMove;
                        }
                        if(score > bestTestMove.first) //If the branch looks good...
                        {
                            bestTestMove.first = score;
                            bestTestMove.second.first.first = x;
                            bestTestMove.second.first.second = y;
                            bestTestMove.second.second.first = Board.container[x][y].moveable[i].first;
                            bestTestMove.second.second.second = Board.container[x][y].moveable[i].second;
                        }
                    }
                    for(unsigned i = 0; i < Board.container[x][y].attackable.size(); ++i)
                    {
                        testBoard = Board;
                        testBoard.playerMove(x, y, Board.container[x][y].attackable[i].first, Board.container[x][y].attackable[i].second, false);
                        d.fastDrawBoard(testBoard);
                        double score = graphicMiniMax(testBoard, depth - 1, d, true, alpha, beta).first;
                        if(score >= beta) //If the branch is failing...
                        {
                            bestTestMove.first = beta;
                            return bestTestMove;
                        }
                        if(score > alpha) //If the branch looks good...
                        {
                            alpha = score;
                            bestTestMove.first = score;
                            bestTestMove.second.first.first = x;
                            bestTestMove.second.first.second = y;
                            bestTestMove.second.second.first = Board.container[x][y].attackable[i].first;
                            bestTestMove.second.second.second = Board.container[x][y].attackable[i].second;
                        }
                    }
                }
                
            }
        }
        return bestTestMove;
    }
    else //White's move
    {
        bestTestMove.first = 99999999999;
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
                        d.fastDrawBoard(testBoard);
                        double score = graphicMiniMax(testBoard, depth - 1, d, false, alpha, beta).first;
                        if(score <= alpha) //If branch is failing...
                        {
                            alpha = score;
                            bestTestMove.first = alpha;
                            return bestTestMove;
                        }
                        if(score < beta) //If branch looks good
                        {
                            beta = score;
                            bestTestMove.first = score;
                            bestTestMove.second.first.first = x;
                            bestTestMove.second.first.second = y;
                            bestTestMove.second.second.first = Board.container[x][y].moveable[i].first;
                            bestTestMove.second.second.second = Board.container[x][y].moveable[i].second;
                        }
                    }
                    for(unsigned i = 0; i < Board.container[x][y].attackable.size(); ++i)
                    {
                        testBoard = Board;
                        testBoard.playerMove(x, y, Board.container[x][y].attackable[i].first, Board.container[x][y].attackable[i].second, true);
                        d.fastDrawBoard(testBoard);
                        double score = graphicMiniMax(testBoard, depth - 1, d, false, aplha, beta).first;
                        if(score <= alpha) //If branch is failing...
                        {
                            alpha = score;
                            bestTestMove.first = alpha;
                            return bestTestMove;
                        }
                        if(score < beta) //If branch looks good
                        {
                            beta = score;
                            bestTestMove.first = score;
                            bestTestMove.second.first.first = x;
                            bestTestMove.second.first.second = y;
                            bestTestMove.second.second.first = Board.container[x][y].moveable[i].first;
                            bestTestMove.second.second.second = Board.container[x][y].moveable[i].second;
                        }
                        if(score < bestTestMove.first)
                        {
                            bestTestMove.first = score;
                            bestTestMove.second.first.first = x;
                            bestTestMove.second.first.second = y;
                            bestTestMove.second.second.first = Board.container[x][y].attackable[i].first;
                            bestTestMove.second.second.second = Board.container[x][y].attackable[i].second;
                        }
                    }
                }
                
            }
        }
        return bestTestMove;
    
    }
    
}

