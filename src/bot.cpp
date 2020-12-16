#include "include/bot.hpp"

using namespace Bot;

std::pair<std::pair<unsigned int, unsigned int>, std::pair<unsigned int, unsigned int> > parseStockfish(std::string outStr)
{
    std::string good = "bestmove";
    std::pair<std::pair<unsigned int, unsigned int>, std::pair<unsigned int, unsigned int> > move;
    std::istringstream str(outStr); //Make string into stream
    std::string line; //String holding contents of read line
    unsigned int val = 0;

    while(std::getline(str, line))
    {
        if(line.find(good) != std::string::npos) //Check if best move was outputted
        {
            switch(line[9])
            {
                case 'a': val = 0; break;
                case 'b': val = 1; break;
                case 'c': val = 2; break;
                case 'd': val = 3; break;
                case 'e': val = 4; break;
                case 'f': val = 5; break;
                case 'g': val = 6; break;
                case 'h': val = 7; break;
            }
            move.first.first = val;
            move.first.second = ( (line[10]) - '0') - 1;

            switch(line[11])
            {
                case 'a': val = 0; break;
                case 'b': val = 1; break;
                case 'c': val = 2; break;
                case 'd': val = 3; break;
                case 'e': val = 4; break;
                case 'f': val = 5; break;
                case 'g': val = 6; break;
                case 'h': val = 7; break;
            }
            move.second.first = val;
            move.second.second = ( (line[12]) - '0') - 1;
            std::cout<<line<<std::endl;
            return move;
        }
    }
    return move; //Just to stop warnings, this will never actually be reached
}

std::string exec(const char* cmd, renderer::Drawer& d) 
{
    std::array<char, 128> buffer; //128 bytes at a time read
    std::string result; //Stringified result
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose); //Open a stockfish instance
    SDL_RaiseWindow(d.win); //Set the window above the terminal window

    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) //Get data until EOF (End Of File)
    {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data(); //Fill the buffer with data and add it to the string
    }
    return result; //Return the output
}

void computerEnemy::stockfishMove(Chess::board_t& Board, std::string& record, std::ofstream& write, unsigned int difficulty, renderer::Drawer& d, unsigned int time)
{
    //Add all data to tell stockfish what difficulty to play at and how long a move should take
    write.open("move.txt", std::ofstream::app | std::ofstream::out);
    write<<"setoption name Skill Level value "<<difficulty<<std::endl;
    write<<"go movetime "<<time<<std::endl; //Write command
    write<<"position startpos move "<<record<<std::endl;//<<record<<std::endl;
    write<<"go movetime "<<time<<std::endl;
    write.close();

    std::string res = exec("E:\\Chess\\bin\\stockfish_20090216_x64_bmi2.exe < move.txt", d); //Makea  new instance of Stockfish
    std::pair<std::pair<unsigned int, unsigned int>, std::pair<unsigned int, unsigned int> > move = parseStockfish(res); //Parse received data

    //std::cout<<move.first.first<<", "<<move.first.second<<"\t"<<move.second.first<<", "<<move.second.second<<std::endl; //Print the move

    /*Storing the stringified move by Stockfish*/
    std::string sentStr = "0000";
    switch(move.first.first) //Converting characters sent into letters
    {
        case 0: sentStr[0] = 'a'; break;
        case 1: sentStr[0] = 'b'; break;
        case 2: sentStr[0] = 'c'; break;
        case 3: sentStr[0] = 'd'; break;
        case 4: sentStr[0] = 'e'; break;
        case 5: sentStr[0] = 'f'; break;
        case 6: sentStr[0] = 'g'; break;
        case 7: sentStr[0] = 'h'; break;
    }
    sentStr[1] = move.first.second + 49;
    switch(move.second.first)
    {
        case 0: sentStr[2] = 'a'; break;
        case 1: sentStr[2] = 'b'; break;
        case 2: sentStr[2] = 'c'; break;
        case 3: sentStr[2] = 'd'; break;
        case 4: sentStr[2] = 'e'; break;
        case 5: sentStr[2] = 'f'; break;
        case 6: sentStr[2] = 'g'; break;
        case 7: sentStr[2] = 'h'; break;
    }
    sentStr[3] = move.second.second + 49;

    record.append(sentStr); //Record the move
    record.append(" ");
    
    //Clear the file
    write.open("move.txt", std::ofstream::out | std::ofstream::trunc);
    write.close();
    write.open("move.txt", std::ofstream::out);

    write<<"setoption name Skill Level value "<<difficulty<<std::endl; //Set the difficulty again
    write<<"position startpos move "; //Tell stocckfish the board layout
    write<<record<<std::endl;
    write.close(); //Close the communication file

    Board.playerMove(move.first.first, move.first.second, move.second.first, move.second.second, false); //Make the move on the board
}

//Function to evaluate a chess position and return a score
double computerEnemy::evaluate(Chess::board_t& Board)
{
    /*If a side is in check, it is very bad, so just return that the board configuration is bad*/
    uint8_t check = Board.isCheck();
    if(check == WHITE_CHECK)
    {
        return 100000000;
    }
    else if(check == BLACK_CHECK)
    {
        return -100000000;
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
                    whiteScore += wPawnTable[x][y];
                break;

                case wKNIGHT: 
                    wKnights++; 
                    whiteScore += wKnightTable[x][y];
                break;

                case wROOK: 
                    wRooks++; 
                    whiteScore += wRookTable[x][y];
                break;

                case wBISHOP: 
                    wBishops++; 
                    whiteScore += wBishopTable[x][y];
                break;

                case wKING: 
                    wKings++; 
                    whiteScore += wKingTable[x][y];
                break;

                case wQUEEN: 
                    wQueens++; 
                    whiteScore += wQueenTable[x][y];
                break;

                case bPAWN: 
                    bPawns++; 
                    blackScore += bPawnTable[x][y];
                break;

                case bKNIGHT: 
                    bKnights++; 
                    blackScore += bKnightTable[x][y];
                break;

                case bROOK: 
                    bRooks++; 
                    blackScore += bRookTable[x][y];
                break;

                case bBISHOP: 
                    bBishops++; 
                    blackScore += bBishopTable[x][y];
                break;

                case bKING: 
                    bKings++; 
                    blackScore += bKingTable[x][y];
                break;

                case bQUEEN: 
                    bQueens++; 
                    blackScore += bQueenTable[x][y];
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
    bestMove = ABMax(-99999999999999, 9999999999999, Board, 4, d, graphic);
    std::cout<<bestMove.second.first.first<<", "<<bestMove.second.first.second<<"\t"<<bestMove.second.second.first<<","<<bestMove.second.second.second<<std::endl;
    Board.playerMove(bestMove.second.first.first, bestMove.second.first.second, bestMove.second.second.first, bestMove.second.second.second, false);
}

moveScore computerEnemy::ABMax(double alpha, double beta, Chess::board_t& Board, int depth, renderer::Drawer& d, bool graphic)
{
    moveScore ret;
    Chess::board_t testBoard = Board;

    if(depth == 0 )//|| Board.isCheck() != NO_CHECK)
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

    if(depth == 0 )//|| Board.isCheck() != NO_CHECK)
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

