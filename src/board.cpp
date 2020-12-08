/*
File for all piece and board logic like checking for movement legality,
loading icons for pieces, etc.
*/
#include "include/main.hpp"

void board_t::init()
{
    board.resize(8); //Resize the board X to be 8
    for(unsigned n = 0; n < 8; ++n)
    {
        board[n].resize(8); //Resize the board Y to be 8 for each X
    }
    
    //Now assign values to each board piece
    for(unsigned x = 0; x < 8; ++x)
    {
        for(unsigned y = 0; y < 8; ++y)
        {
            //On the top row...
            if(y == board_8)
            {
                switch(x)
                {
                    //a8 and h8 have black rook
                    case board_a:
                    case board_h:
                        board[x][y] = piece_t(bROOK); //Make the piece a black rook
                    break;

                    //b8 and g8 both have a black knight
                    case board_b:
                    case board_g:
                        board[x][y] = piece_t(bKNIGHT); //Make the piece a black knight
                    break;

                    //c8 and f8 have a black bishop
                    case board_c:
                    case board_f:
                        board[x][y] = piece_t(bBISHOP); //Make the piece a black bishop
                    break;
                    
                    //d8 is a black queen
                    case board_d:
                        board[x][y] = piece_t(bQUEEN); //Make the piece a black queen
                    break;

                    //e8 is a black king
                    case board_e:
                        board[x][y] = piece_t(bKING); //Make the piece a black king
                    break;

                }
            }

            //On the second to top row...
            if(y == board_7)
            {
                board[x][y] = piece_t(bPAWN); //Make the piece a black pawn
            }

            //On the bottom row...
            if(y == board_1)
            {
                switch(x)
                {
                    //a1 and h1 have white rook
                    case board_a:
                    case board_h:
                        board[x][y] = piece_t(wROOK); //Make the piece a white rook
                    break;

                    //b1 and g1 both have a white knight
                    case board_b:
                    case board_g:
                        board[x][y] = piece_t(wKNIGHT); //Make the piece a white knight
                    break;

                    //c1 and f1 have a black bishop
                    case board_c:
                    case board_f:
                        board[x][y] = piece_t(wBISHOP); //Make the piece a white bishop
                    break;
                    
                    //d1 is a white queen
                    case board_d:
                        board[x][y] = piece_t(wQUEEN); //Make the piece a white queen
                    break;

                    //e1 is a white king
                    case board_e:
                        board[x][y] = piece_t(wKING); //Make the piece a white king
                    break;

                }
            }

            //On the second to bottom row...
            if(y == board_2)
            {
                board[x][y] = piece_t(wPAWN); //Make the piece a white pawn
            }
        
        }
    }
}

