/*
File used for defining functions for pieces like move,
update attackable squares, etc.
*/
#include "include/Chess.hpp"
using namespace Chess;

//Function to only allow player to move their pieces
uint8_t board_t::playerMove(unsigned int x, unsigned int y, unsigned int moveX, unsigned int moveY, bool WHITE)
{
    uint8_t rVal; //Return value
    findMoves();

    //Make sure the player is only controlling their pieces and that it is their turn and that nobody has won
    if( ( (container[x][y].type > WHITE_END && !WHITE) || (container[x][y].type <= WHITE_END && WHITE)  ) && WINNER == WINNER_NONE ) //UNCOMMENT FOR RELEASE //&& ( (WHITE && TURN == WHITE_TURN) || (!WHITE && TURN == BLACK_TURN) ) )
    {
        for(unsigned i = 0; i < container[x][y].moveable.size(); ++i) //Iterate through all of the moveable tiles of that piece
        {
            if(container[x][y].moveable[i] == std::make_pair(moveX, moveY) ) //If the moveable tiles contains the desired move, move the piece
            {  
                //Checking if the white king is castling
                if(container[x][y].type == wKING && container[x][y].hasMoved == false)
                {
                    //Checking for kingside castle
                    if(moveX == board_g && moveY == board_1)
                    {
                        //Move the rook if the king is castling
                        container[board_f][board_1] = piece_t(container[board_h][board_1].type, true);
                        container[board_h][board_1] = piece_t(EMPTY, true);
                    }
                    else if(moveX == board_c && moveY == board_1)
                    {
                        //Move the rook if the king is castling
                        container[board_d][board_1] = piece_t(container[board_a][board_1].type, true);
                        container[board_a][board_1] = piece_t(EMPTY, true);
                    }
                    
                }

                //Checking if the black king is castling
                if(container[x][y].type == bKING && container[x][y].hasMoved == false)
                {
                    //Checking for kingside castle
                    if(moveX == board_g && moveY == board_8)
                    {
                        //Move the rook if the king is castling
                        container[board_f][board_8] = piece_t(container[board_h][board_8].type, true);
                        container[board_h][board_8] = piece_t(EMPTY, true);
                    }
                    else if(moveX == board_c && moveY == board_8)
                    {
                        //Move the rook if the king is castling
                        container[board_d][board_8] = piece_t(container[board_a][board_8].type, true);
                        container[board_a][board_8] = piece_t(EMPTY, true);
                    }
                    
                }
                
                container[moveX][moveY] = piece_t(container[x][y].type, true); //Assign  the moving square's type as the moved piece
                container[x][y] = piece_t(EMPTY, true); //Make the old location empty

                checkPromotion(moveX, moveY, WHITE); //Check if the piece can be promoted

                

                rVal =  MOVE_GOOD; //Return that the move was good
            }
        }
        //Now go through all attackable tiles and see if one matches
        for(unsigned n = 0; n < container[x][y].attackable.size(); ++n)
        {
            if(container[x][y].attackable[n] == std::make_pair(moveX, moveY))
            {
                container[moveX][moveY] = piece_t(container[x][y].type, true); //Assign  the moving square's type as the moved piece
                container[x][y] = piece_t(EMPTY, true); //Make the old location empty

                checkPromotion(moveX, moveY, WHITE); //Check if the piece can be promoted

                rVal = MOVE_CAPTURED; //Return that the move captured a piece
            }
        }
        rVal = MOVE_BAD; //Return move didn't work if the piece couldn't move
    }
    else rVal = MOVE_BAD; //Return move bad if the player tried to move a peice they don't control

    //FOR RELEASE UNCOMMENT THIS
    //if(WHITE) TURN = BLACK_TURN; //White made a move, blacks turn now
    //else TURN = WHITE_TURN; //Black made a move, now it is white's turn

    bool foundWKing = false;
    bool foundBKing = false;

    for(unsigned __x = 0; __x < 8; ++__x)
    {
        for(unsigned __y = 0; __y < 8; ++__y)
        {
            if(container[__x][__y].type == wKING) foundWKing = true;
            else if(container[__x][__y].type == bKING) foundBKing = true;
        }
    }
    if(!foundWKing) WINNER = WINNER_BLACK;
    if(!foundBKing) WINNER = WINNER_WHITE;

    return rVal;
}

//Function to init a piece with position and type
piece_t::piece_t(uint8_t _type)
{
    type = _type;
    hasMoved = false;
}

//Function to return a piece with the hasMoved variable set
piece_t::piece_t(uint8_t _type, bool _HAS_MOVED)
{
    type = _type;
    hasMoved = _HAS_MOVED;
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
    container[board_a][board_1] = piece_t(wROOK); //Place white rook at A1
    container[board_b][board_1] = piece_t(wKNIGHT); //Place knight at B1
    container[board_c][board_1] = piece_t(wBISHOP); //Place bishop at C1
    container[board_d][board_1] = piece_t(wQUEEN); //Place queen at D1
    container[board_e][board_1] = piece_t(wKING); //Place king at E1
    container[board_f][board_1] = piece_t(wBISHOP); //Place bishop at F1
    container[board_g][board_1] = piece_t(wKNIGHT); //Place knight at G1
    container[board_h][board_1] = piece_t(wROOK); //Place rook at H1
    for(i = 0; i < 8; ++i) //Place eight pawns
    {
        container[i][board_2] = piece_t(wPAWN);
    }

    //Start placing black pieces in their default locations
    container[board_a][board_8] = piece_t(bROOK); //Place rook at A8
    container[board_b][board_8] = piece_t(bKNIGHT); //Place knight at B8
    container[board_c][board_8] = piece_t(bBISHOP); //Place bishop at C8
    container[board_d][board_8] = piece_t(bQUEEN); //Place queen at D8
    container[board_e][board_8] = piece_t(bKING); //Place king at E8
    container[board_f][board_8] = piece_t(bBISHOP); //Place bishop at F8
    container[board_g][board_8] = piece_t(bKNIGHT); //Place knight at G8
    container[board_h][board_8] = piece_t(bROOK); //Place rook at H8
    for(i = 0; i < 8; ++i) //Place eight pawns
    {
        container[i][board_7] = piece_t(bPAWN);
    }
}

//Function to restart the chess game
void board_t::restart()
{
    counter = 0; //How many turns the game has gone for
    TURN = WHITE_TURN; //If it white or blacks turn
    WINNER = WINNER_NONE; //If white or black won
    unsigned int i; //Iterator variable

    //Make the board empty
    for(unsigned x = 0; x < 8; ++x)
    {
        for(unsigned y = 0; y < 8; ++y)
        {
            container[x][y] = piece_t(EMPTY);
        }
    }

    //Start placing white pieces in their default locations
    container[board_a][board_1] = piece_t(wROOK); //Place white rook at A1
    container[board_b][board_1] = piece_t(wKNIGHT); //Place knight at B1
    container[board_c][board_1] = piece_t(wBISHOP); //Place bishop at C1
    container[board_d][board_1] = piece_t(wQUEEN); //Place queen at D1
    container[board_e][board_1] = piece_t(wKING); //Place king at E1
    container[board_f][board_1] = piece_t(wBISHOP); //Place bishop at F1
    container[board_g][board_1] = piece_t(wKNIGHT); //Place knight at G1
    container[board_h][board_1] = piece_t(wROOK); //Place rook at H1
    for(i = 0; i < 8; ++i) //Place eight pawns
    {
        container[i][board_2] = piece_t(wPAWN);
    }

    //Start placing black pieces in their default locations
    container[board_a][board_8] = piece_t(bROOK); //Place rook at A8
    container[board_b][board_8] = piece_t(bKNIGHT); //Place knight at B8
    container[board_c][board_8] = piece_t(bBISHOP); //Place bishop at C8
    container[board_d][board_8] = piece_t(bQUEEN); //Place queen at D8
    container[board_e][board_8] = piece_t(bKING); //Place king at E8
    container[board_f][board_8] = piece_t(bBISHOP); //Place bishop at F8
    container[board_g][board_8] = piece_t(bKNIGHT); //Place knight at G8
    container[board_h][board_8] = piece_t(bROOK); //Place rook at H8
    for(i = 0; i < 8; ++i) //Place eight pawns
    {
        container[i][board_7] = piece_t(bPAWN);
    }
}

//Function to find where a piece can move on the board
void board_t::findMoves()
{
    for(unsigned x = 0; x < 8; ++x)
    {
        for(unsigned y = 0; y < 8; ++y)
        {
            container[x][y].moveable.clear(); //Clear the current moveable tiles
            container[x][y].attackable.clear(); //Clear attackable tiles

            //Different piece types can move in different ways
            switch(container[x][y].type)
            {
                case wPAWN:
                    findPawn(x, y, true); //Find white pawn moves
                break;

                case bPAWN:
                    findPawn(x, y, false); //Find black pawn moves
                break;

                //White rooks can attack black pieces in all four directions
                case wROOK:
                    findRook(x, y, true); //Find white rook moves
                break;

                //Black rooks can attack black pieces in all four directions
                case bROOK:
                    findRook(x, y, false); //Find black rook moves
                break;

                //Bishops can attack in all four diagonals
                case wBISHOP:
                    findBishop(x, y, true); //Find white bishop moves
                break;

                case bBISHOP:
                    findBishop(x, y, false); //Find black bishop moves
                break;

                case wKNIGHT:
                    findKnight(x, y, true); //Find white knight moves
                break;

                case bKNIGHT:
                    findKnight(x, y, false); //Find black knight moves
                break;

                case wKING:
                    findKing(x, y, true); //Find white king moves
                    findCastle(true); //Find white castles
                break;

                case bKING:
                    findKing(x, y, false); //Find black king moves
                    findCastle(false); //Find black castles
                break;

                case wQUEEN:
                    findQueen(x, y, true); //Find white queen moves
                break;

                case bQUEEN:
                    findQueen(x, y, false); //Find black queen moves
                break;

                case EMPTY:

                break;

            }
        }
    }

    
}


//Functions for finding moves for each type of piece
void board_t::findPawn(unsigned int _x, unsigned int _y, bool WHITE) //Function to find black or white pawn's moves
{
    //We have to cast to a signed int because I am not very smart and
    //Need to prevent underflowing when subtracting 2 from 0. 
    //I am an idiot
    int x = (int)_x;
    int y = (int)_y;

    if(WHITE)
    {
        //If pawns haven't moved, they can move up two squares
        if(!container[x][y].hasMoved)
        {
            if(container[x][y + 2].type == EMPTY) //If the square is empty
                container[x][y].moveable.push_back(std::make_pair(x, y + 2)); //Add the square up if it is empty
        }
        //Pawns can move forwards if the square in front is empty
        if(y < sizeY)
        {
            if(container[x][y + 1].type == EMPTY)
                container[x][y].moveable.push_back(std::make_pair(x, y + 1)); //Add the square up if it is empty
        }

        if(x < sizeX && y < sizeY) //Check for up and right
        {
            if(container[x + 1][y + 1].type != EMPTY && container[x + 1][y + 1].type > WHITE_END)
                container[x][y].attackable.push_back(std::make_pair(x + 1, y + 1)); //Add the attackable square
        }
        if(x > 0 && y < sizeY) //Check up and left
        {
            if(container[x - 1][y + 1].type != EMPTY && container[x - 1][y + 1].type > WHITE_END)
                container[x][y].attackable.push_back(std::make_pair(x - 1, y + 1)); 
        }
    }
    else
    {
        //If pawns haven't moved, they can move down two squares
        if(!container[x][y].hasMoved)
        {
            if(container[x][y - 2].type == EMPTY) //If the square is empty
                container[x][y].moveable.push_back(std::make_pair(x, y - 2)); //Add the square up if it is empty
        }
        if(y > 0) //Check down
        {
            if(container[x][y - 1].type == EMPTY)
                container[x][y].moveable.push_back(std::make_pair(x, y - 1));
        }
        if(x < sizeX && y > 0) //Check for down and right
        {
            if(container[x + 1][y - 1].type != EMPTY && container[x + 1][y - 1].type <= WHITE_END)
                container[x][y].attackable.push_back(std::make_pair(x + 1, y - 1));
        }
        if(x > 0 && y > 0) //Check for down and left
        {
            if(container[x -1][y - 1].type != EMPTY && container[x - 1][y - 1].type <= WHITE_END)
                container[x][y].attackable.push_back(std::make_pair(x - 1, y - 1));
        }  
    }
    
}

//Function to check promotion status of a pawn
void board_t::checkPromotion(unsigned int x, unsigned int y, bool WHITE)
{
    if(container[x][y].type == wPAWN && y == board_8) container[x][y] = piece_t(wQUEEN, true); //Set the pawn to a queen if it is promoted
    else if(container[x][y].type == bPAWN && y == board_1) container[x][y] = piece_t(bQUEEN, true); //Set the pawn to a queen if it is promoted
}

void board_t::findRook(unsigned int _x, unsigned int _y, bool WHITE) //Function to find black or white rook's moves
{
    //We have to cast to a signed int because I am not very smart and
    //Need to prevent underflowing when subtracting 2 from 0. 
    //I am an idiot
    int x = (int)_x;
    int y = (int)_y;
    int temp = 0;

    if(y < sizeY) //Check up first
    {
        while(y + temp < sizeY) //While we haven't hit the ceiling
        {
            ++temp;
            if(container[x][y + temp].type == EMPTY) //If type is empty, we can move
            {
                container[x][y].moveable.push_back(std::make_pair(x, y + temp));
            }
            //If something is in front of us, then check if we can attack it, then stop this line
            else 
            {
                if((container[x][y + temp].type > WHITE_END && WHITE ) || (container[x][y + temp].type <= WHITE_END && !WHITE ) ) //Make sure the piece is opposite color
                {
                    container[x][y].attackable.push_back(std::make_pair(x, y + temp)); //If it is, add it to attackable
                }
                break;
            }
        }
    }

    temp = 0;
    if(y > 0) //Check down next
    {
        while(y + temp > 0) //Go to floor
        {
            --temp; //Go lower
            if(container[x][y + temp].type == EMPTY) //Make sure it's empty
            {
                container[x][y].moveable.push_back(std::make_pair(x, y + temp)); //Add this to moveable
            }
            else
            {
                if((container[x][y + temp].type > WHITE_END && WHITE) || (container[x][y + temp].type <= WHITE_END && !WHITE)) //make sure piece is black
                {
                    container[x][y].attackable.push_back(std::make_pair(x, y + temp));
                }
                break; //Exit the loop
            }
            
        }
    }

    temp = 0;
    if(x < sizeX) //Check right next
    {
        while(x + temp < sizeX) //While we haven't hit the wall...
        {
            ++temp;
            if(container[x + temp][y].type == EMPTY)
            {
                container[x][y].moveable.push_back(std::make_pair(x + temp, y)); //Add that to the moveable list
            }
            else
            {
                if((container[x + temp][y].type > WHITE_END && WHITE) || (container[x + temp][y].type <= WHITE_END && !WHITE)) //Check is piece is black
                {
                    container[x][y].attackable.push_back(std::make_pair(x + temp, y)); //Add this to the attackable list
                }
                break;
            }
            
        }
    }

    temp = 0;
    if(x > 0) //Check left finally
    {
        while(x + temp > 0) //While we haven't hit a wall...
        {
            --temp;
            if(container[x + temp][y].type == EMPTY)
            {
                container[x][y].moveable.push_back(std::make_pair(x + temp, y)); //Add to the moveable list if the sqaure is empty
            }
            else
            {
                if( (container[x + temp][y].type > WHITE_END && WHITE) || (container[x + temp][y].type <= WHITE_END && !WHITE)) //Check if the piece is black
                {
                    container[x][y].attackable.push_back(std::make_pair(x + temp, y)); 
                }
                break;
            }
            
        }
    }

}

void board_t::findKnight(unsigned int _x, unsigned int _y, bool WHITE) //Function to find black or white knight's moves
{
    //We have to cast to a signed int because I am not very smart and
    //Need to prevent underflowing when subtracting 2 from 0. 
    //I am an idiot
    int x = (int)_x;
    int y = (int)_y;

    if((x + 2 <= sizeX && y + 1 <= sizeY)) //Check up 1, right 2 first
    {
        if(container[x + 2][y + 1].type == EMPTY)
        {
            container[x][y].moveable.push_back(std::make_pair(x + 2, y + 1)); //Add to moveable if it is empty
        }
        else
        {
            if( (container[x + 2][y + 1].type > WHITE_END && WHITE) || (container[x + 2][y + 1].type <= WHITE_END && !WHITE) )
            {
                container[x][y].attackable.push_back(std::make_pair(x + 2, y + 1)); //Add to attackable if other side
            }
        }
        
    }

    if((x + 1 <= sizeX && y + 2 <= sizeY)) //Check up 2, right 1
    {
        if(container[x + 1][y + 2].type == EMPTY)
        {
            container[x][y].moveable.push_back(std::make_pair(x + 1, y + 2)); //Add to moveable if empty
        }
        else
        {
            if((container[x + 1][y + 2].type > WHITE_END && WHITE) || (container[x + 1][y + 2].type <= WHITE_END && !WHITE) )
            {
                container[x][y].attackable.push_back(std::make_pair(x + 1, y + 2)); //Add to attackable if on other side
            }
        }
        
    }

    if((x - 1 >= 0 && y + 2 <= sizeY)) //Check up 2, left 1
    {
        if(container[x - 1][y + 2].type == EMPTY)
        {
            container[x][y].moveable.push_back(std::make_pair(x - 1, y + 2)); //Add to moveable if empty
        }
        else
        {
            if((container[x - 1][y + 2].type > WHITE_END && WHITE) || (container[x - 1][y + 2].type <= WHITE_END && !WHITE) )
            {
                container[x][y].attackable.push_back(std::make_pair(x - 1, y + 2)); //Add to attackable if on other side
            }
        }
        
    }

    if((x - 2 >= 0 && y + 1 <= sizeY)) //Check up 1, left 2
    {
        if(container[x - 2][y + 1].type == EMPTY)
        {
            container[x][y].moveable.push_back(std::make_pair(x - 2, y + 1)); //Add to moveable if empty
        }
        else
        {
            if((container[x - 2][y + 1].type > WHITE_END && WHITE) || (container[x - 2][y + 1].type <= WHITE_END && !WHITE) )
            {
                container[x][y].attackable.push_back(std::make_pair(x - 2, y + 1)); //Add to attackable if on other side
            }
        }
        
    }

    if((x - 2 >= 0 && y - 1 >= 0)) //Check down 1, left 2
    {
        if(container[x - 2][y - 1].type == EMPTY)
        {
            container[x][y].moveable.push_back(std::make_pair(x - 2, y - 1)); //Add to moveable if empty
        }
        else
        {
            if((container[x - 2][y - 1].type > WHITE_END && WHITE) || (container[x - 2][y - 1].type <= WHITE_END && !WHITE) )
            {
                container[x][y].attackable.push_back(std::make_pair(x - 2, y - 1)); //Add to attackable if on other side
            }
        }
        
    }

    if((x - 1 >= 0 && y - 2 >= 0)) //Check down 2, left 1
    {
        if(container[x - 1][y - 2].type == EMPTY)
        {
            container[x][y].moveable.push_back(std::make_pair(x - 1, y - 2)); //Add to moveable if empty
        }
        else
        {
            if((container[x - 1][y - 2].type > WHITE_END && WHITE) || (container[x - 1][y - 2].type <= WHITE_END && !WHITE) )
            {
                container[x][y].attackable.push_back(std::make_pair(x - 1, y - 2)); //Add to attackable if on other side
            }
        }
        
    }

    if((x + 1 <= sizeX && y - 2 >= 0)) //Check down 2, right 1
    {
        if(container[x + 1][y - 2].type == EMPTY)
        {
            container[x][y].moveable.push_back(std::make_pair(x + 1, y - 2)); //Add to moveable if empty
        }
        else
        {
            if((container[x + 1][y - 2].type > WHITE_END && WHITE) || (container[x + 1][y - 2].type <= WHITE_END && !WHITE) )
            {
                container[x][y].attackable.push_back(std::make_pair(x + 1, y - 2)); //Add to attackable if on other side
            }
        }
        
    }

    if((x + 2 <= sizeX && y - 1 >= 0)) //Check down 1, right 2
    {
        if(container[x + 2][y - 1].type == EMPTY)
        {
            container[x][y].moveable.push_back(std::make_pair(x + 2, y - 1)); //Add to moveable if empty
        }
        else
        {
            if((container[x + 2][y - 1].type > WHITE_END && WHITE) || (container[x + 2][y - 1].type <= WHITE_END && !WHITE) )
            {
                container[x][y].attackable.push_back(std::make_pair(x + 2, y - 1)); //Add to attackable if on other side
            }
        }
        
    }

}

void board_t::findBishop(unsigned int _x, unsigned int _y, bool WHITE) //Function to find black or white bishop's moves
{
    //We have to cast to a signed int because I am not very smart and
    //Need to prevent underflowing when subtracting 2 from 0. 
    //I am an idiot
    int x = (int)_x;
    int y = (int)_y;

    int temp = 0;

    if(y < sizeY && x < sizeX) //Check up and right first
    {
        while((y + temp) < sizeY && (x + temp) < sizeX) //While we haven't hit ceiling or wall...
        {   
            ++temp;
            if(container[x + temp][y + temp].type == EMPTY) //If the tile is empty, add it to the moveable list
            {
                container[x][y].moveable.push_back(std::make_pair(x + temp, y + temp));
            }
            else
            {
                if( (container[x + temp][y + temp].type > WHITE_END && WHITE) || (container[x + temp][y + temp].type <= WHITE_END && !WHITE)) //Check if piece is black
                {
                    container[x][y].attackable.push_back(std::make_pair(x + temp, y + temp)); //Add that to attackable list 
                }
                break; //Exit loop
            }
        }
    }   
    temp = 0;
    if(y != sizeY && x != 0) //Check up and left next
    {
        while(y + temp < sizeY && x - temp > 0) //While we haven't hit wall or ceiling...
        {
            ++temp;
            if(container[x - temp][y + temp].type == EMPTY)
            {
                container[x][y].moveable.push_back(std::make_pair(x - temp, y + temp)); //Add that square to moveable if it is empty
            }
            else
            {
                if( (container[x - temp][y + temp].type > WHITE_END && WHITE) || (container[x - temp][y + temp].type <= WHITE_END && !WHITE)) //Check if piece is black
                {
                    container[x][y].attackable.push_back(std::make_pair(x - temp, y + temp)); //Add to attackable list
                }
                break;
            }
        }   
    }
    temp = 0;
    if(y > 0 && x < sizeX) //Check down and right next
    {
        while(y - temp > 0 && x + temp < sizeX) //While we haven't hit wall or ceiling
        {
            ++temp;
            if(container[x + temp][y - temp].type == EMPTY)
            {
                container[x][y].moveable.push_back(std::make_pair(x + temp, y - temp)); //Add to moveable if the tile is empty
            }
            else
            {
                if( (container[x + temp][y - temp].type > WHITE_END && WHITE) || (container[x + temp][y - temp].type <= WHITE_END && !WHITE)) //Check if the piece is black
                {
                    container[x][y].attackable.push_back(std::make_pair(x + temp, y - temp));
                }
                break;
            }
        }
    }  
    temp = 0;
    if(y != 0 && x != 0) //Check down and left finally
    {
        while(y - temp > 0 && x - temp > 0) //While we haven't hit floor or wall...
        {
            ++temp;
            if(container[x - temp][y - temp].type == EMPTY)
            {
                container[x][y].moveable.push_back(std::make_pair(x - temp, y - temp));
            }
            else
            {
                if( (container[x - temp][y - temp].type > WHITE_END && WHITE) || (container[x - temp][y - temp].type <= WHITE_END && !WHITE)) //If the piece is black
                {
                    container[x][y].moveable.push_back(std::make_pair(x - temp, y - temp));
                }
            }
        }
    }

}

void board_t::findKing(unsigned int _x, unsigned int _y, bool WHITE) //Function to find black or white king's moves
{
    //We have to cast to a signed int because I am not very smart and
    //Need to prevent underflowing when subtracting 2 from 0. 
    //I am an idiot
    int x = (int)_x;
    int y = (int)_y;


    //Check top 3 moves first
    for(int i = -1; i < 2; ++i)
    {
        if( ( (x + i) >= 0 && (x + i) <= sizeX)  && (y + 1) <= sizeY) //Make sure we aren't out of bounds
        { 
            if(container[x + i][y + 1].type == EMPTY) //If the tile is empty...
            {
                container[x][y].moveable.push_back(std::make_pair(x + i, y + 1)); //Add to movable list if it is empty
            }
            else
            {
                if((container[x + i][y + 1].type > WHITE_END && WHITE) || (container[x + i][y + 1].type <= WHITE_END && !WHITE)) //If the tile isn't empty but contains a color we aren't...
                {
                    container[x][y].attackable.push_back(std::make_pair(x + i, y + 1)); //Add to attackable list if enemy
                }
            }
            
        }
    }

    //Check bottom 3 moves next
    for(int i = -1; i < 2; ++i)
    {
        if( ( (x + i) >= 0 && (x + i) <= sizeX) && (y - 1) >= 0) //Make sure we aren't out of bounds
        {
            if(container[x + i][y +-1].type == EMPTY)
            {
                container[x][y].moveable.push_back(std::make_pair(x + i, y - 1)); //Add to movable list if it is empty
            }
            else
            {
                if((container[x + i][y - 1].type > WHITE_END && WHITE) || (container[x + i][y - 1].type <= WHITE_END && !WHITE))
                {
                    container[x][y].attackable.push_back(std::make_pair(x + i, y - 1)); //Add to attackable list if enemy
                }
            }
            
        }
    }

    //Check left and right moves
    if( (x + 1) <= sizeX)
    {
        if(container[x + 1][y].type == EMPTY)
        {
            container[x][y].moveable.push_back(std::make_pair(x + 1, y)); //Add to movable list if it is empty
        }
        else
        {
            if((container[x + 1][y].type > WHITE_END && WHITE) || (container[x + 1][y].type <= WHITE_END && !WHITE))
            {
                container[x][y].attackable.push_back(std::make_pair(x + 1, y)); //Add to attackable list if enemy
            }
        }
    }
    if( (x - 1) >= 0)
    {
        if(container[x - 1][y].type == EMPTY)
        {
            container[x][y].moveable.push_back(std::make_pair(x - 1, y)); //Add to movable list if it is empty
        }
        else
        {
            if((container[x - 1][y].type > WHITE_END && WHITE) || (container[x - 1][y].type <= WHITE_END && !WHITE))
            {
                container[x][y].attackable.push_back(std::make_pair(x - 1, y)); //Add to attackable list if enemy
            }
        }
    }
}

void board_t::findCastle(bool WHITE) //Function to check for availible castles
{
    //Checking for white king castling
    if(WHITE)
    {
        //Checking for kingside castle
        if(container[board_e][board_1].type == wKING && container[board_e][board_1].hasMoved == false && container[board_f][board_1].type == EMPTY && container[board_g][board_1].type == EMPTY && container[board_h][board_1].type == wROOK && container[board_h][board_1].hasMoved == false)
        {
            container[board_e][board_1].moveable.push_back(std::make_pair<unsigned int, unsigned int>(board_g, board_1));
        }
        //Checking for queenside castle
        if(container[board_e][board_1].type == wKING && container[board_e][board_1].hasMoved == false && container[board_d][board_1].type == EMPTY && container[board_c][board_1].type == EMPTY && container[board_b][board_1].type == EMPTY && container[board_a][board_1].type == wROOK && container[board_a][board_1].hasMoved == false)
        {
            container[board_e][board_1].moveable.push_back(std::make_pair<unsigned int, unsigned int>(board_c, board_1));
        }
    }
    
    //Checking for black king castling
    else
    {
        //Checking for kingside castle
        if(container[board_e][board_8].type == bKING && container[board_e][board_8].hasMoved == false && container[board_f][board_8].type == EMPTY && container[board_g][board_8].type == EMPTY && container[board_h][board_8].type == bROOK && container[board_h][board_8].hasMoved == false)
        {
            container[board_e][board_8].moveable.push_back(std::make_pair<unsigned int, unsigned int>(board_g, board_8));
        }
        //Checking for queenside castle
        if(container[board_e][board_8].type == bKING && container[board_e][board_8].hasMoved == false && container[board_d][board_8].type == EMPTY && container[board_c][board_8].type == EMPTY && container[board_b][board_8].type == EMPTY && container[board_a][board_8].type == bROOK && container[board_a][board_8].hasMoved == false)
        {
            container[board_e][board_1].moveable.push_back(std::make_pair<unsigned int, unsigned int>(board_c, board_8));
        }
    }
}

void board_t::findQueen(unsigned int x, unsigned int y, bool WHITE) //Function to find black or white queen's moves
{
    //Queen is just combination of rook and bishop, so just do both those checks
    findRook(x, y, WHITE);
    findBishop(x, y, WHITE);
}

