/*
File used for defining functions for pieces like move,
update attackable squares, etc.
*/
#include "include/Chess.hpp"
using namespace Chess;

bool board_t::move(unsigned int x, unsigned int y, unsigned int moveX, unsigned int moveY)
{
    findMoves(x, y); //Get moves of the selected piece
    for(unsigned i = 0; i < container[x][y].moveable.size(); ++i) //Iterate through all of the moveable tiles of that piece
    {
        if(container[x][y].moveable[i] == std::make_pair(moveX, moveY) ) //If the moveable tiles contains the desired move, move the piece
        {  
            container[moveX][moveY] = container[x][y].type; //Assign  the moving square's type as the moved piece
            container[x][y].type = EMPTY; //Make the old location empty
            return true;
        }
    }
    //Now go through all attackable tiles and see if one matches
    for(unsigned n = 0; n < container[x][y].attackable.size(); ++n)
    {
        if(container[x][y].attackable[n] == std::make_pair(moveX, moveY))
        {
            container[moveX][moveY] = container[x][y].type; //Assign  the moving square's type as the moved piece
            container[x][y].type = EMPTY; //Make the old location empty
            return true;
        }
    }
    return false;
}

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
    container[board_a][board_8].type = bROOK; //Place rook at A8
    container[board_b][board_8].type = bKNIGHT; //Place knight at B8
    container[board_c][board_8].type = bBISHOP; //Place bishop at C8
    container[board_d][board_8].type = bQUEEN; //Place queen at D8
    container[board_e][board_8].type = bKING; //Place king at E8
    container[board_f][board_8].type = bBISHOP; //Place bishop at F8
    container[board_g][board_8].type = bKNIGHT; //Place knight at G8
    container[board_h][board_8].type = bROOK; //Place rook at H8
    for(i = 0; i < 8; ++i) //Place eight pawns
    {
        container[i][board_7].type = bPAWN;
    }
}


//Function to find where a piece can move on the board
void board_t::findMoves(unsigned int x, unsigned int y)
{
    //Make sure we're not trying to check a pawn that doesn't exist
    if(x > container.size() || y > container[0].size())
    {
        return;
    }

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
        break;

        case bKING:
            findKing(x, y, false); //Find black king moves
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

void board_t::findRook(unsigned int _x, unsigned int _y, bool WHITE) //Function to find black or white rook's moves
{
    //We have to cast to a signed int because I am not very smart and
    //Need to prevent underflowing when subtracting 2 from 0. 
    //I am an idiot
    int x = (int)_x;
    int y = (int)_y;
    int temp = 0;

    if(y != sizeY) //Check up first
    {
        while(y + temp < sizeY) //While we haven't hit the ceiling
        {
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
            temp++; //Increase y
        }
    }

    temp = 0;
    if(y != 0) //Check down next
    {
        while(y + temp > 0) //Go to floor
        {
            if(container[x][y + temp].type == EMPTY) //Make sure it's empty
            {
                container[x][y].moveable.push_back(std::make_pair(x, y + temp)); //Add this to moveable
            }
            else
            {
                if((container[x][y - temp].type > WHITE_END && WHITE) || (container[x][y - temp].type <= WHITE_END && !WHITE)) //make sure piece is black
                {
                    container[x][y].attackable.push_back(std::make_pair(x, y + temp));
                }
                break; //Exit the loop
            }
            temp--; //Decrease Y
            
        }
    }

    temp = 0;
    if(x != sizeX) //Check right next
    {
        while(x + temp < sizeX) //While we haven't hit the wall...
        {
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
    if(x != 0) //Check left finally
    {
        while(x + temp > 0) //While we haven't hit a wall...
        {
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

    if(!(x + 2 > sizeX || y + 1 > sizeY)) //Check up 1, right 2 first
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

    if(!(x + 1 > sizeX || y + 2 > sizeY)) //Check up 2, right 1
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

    if(!(x - 1 < 0 || y + 2 > sizeY)) //Check up 2, left 1
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

    if(!(x - 2 < 0 || y + 1 > sizeY)) //Check up 1, left 2
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

    if(!(x - 2 < 0 || y - 1 < 0)) //Check down 1, left 2
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

    if(!(x - 1 < 0 || y - 2 < 0)) //Check down 2, left 1
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

    if(!(x + 1 > sizeX || y - 2 < 0)) //Check down 2, right 1
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

    if(!(x + 2 > sizeX || y - 1 < 0)) //Check down 1, right 2
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
            std::cout<<x + temp<<" "<<y + temp<<std::endl;
            if(container[x + temp][y + temp].type == EMPTY) //If the tile is empty, add it to the moveable list
            {
                container[x][y].moveable.push_back(std::make_pair(x + temp, y + temp));
            }
            else
            {
                if( (container[x + temp][y + temp].type > WHITE_END && WHITE) ||(container[x + temp][y + temp].type <= WHITE_END && !WHITE)) //Check if piece is black
                {
                    container[x][y].attackable.push_back(std::make_pair(x + temp, y + temp)); //Add that to attackable list 
                }
                break; //Exit loop
            }
            ++temp; //Add to x and y
        }
    }   
    temp = 0;
    if(y != sizeY && x != 0) //Check up and left next
    {
        while(y + temp < sizeY && x - temp > 0) //While we haven't hit wall or ceiling...
        {
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
            temp++;
        }   
    }
    temp = 0;
    if(y != 0 && x != sizeX) //Check down and right next
    {
        while(y - temp > 0 && x + temp < sizeX) //While we haven't hit wall or ceiling
        {
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
            temp++;
        }
    }  
    temp = 0;
    if(y != 0 && x != 0) //Check down and left finally
    {
        while(y - temp > 0 && x - temp > 0) //While we haven't hit floor or wall...
        {
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
            temp++;
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
    for(int i = -1; i < 3; ++i)
    {
        if(!((x + i) > 0 || (x + i) < sizeX) || y + 1 < sizeY) //Make sure we aren't out of bounds
        {
            if(container[x + i][y + 1].type == EMPTY)
            {
                container[x][y].moveable.push_back(std::make_pair(x + i, y + 1)); //Add to movable list if it is empty
            }
            else
            {
                if((container[x + i][y + 1].type > WHITE_END && WHITE) || (container[x + i][y + 1].type <= WHITE_END && !WHITE))
                {
                    container[x][y].attackable.push_back(std::make_pair(x + i, y + 1)); //Add to attackable list if enemy
                }
            }
            
        }
    }

    //Check bottom 3 moves next
    for(int i = -1; i < 3; ++i)
    {
        if(!((x + i) > 0 || (x + i) < sizeX) || y - 1 > 0) //Make sure we aren't out of bounds
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
    if(x + 1 < sizeX)
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
    if(x - 1 > 0)
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

void board_t::findQueen(unsigned int x, unsigned int y, bool WHITE) //Function to find black or white queen's moves
{
    //Queen is just combination of rook and bishop, so just do both those checks
    findRook(x, y, WHITE);
    findBishop(x, y, WHITE);
}

