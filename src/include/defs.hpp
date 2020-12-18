
#define PIECE_X 77
#define PIECE_Y 77

#define MOVE_BAD 0
#define MOVE_GOOD 1
#define MOVE_CAPTURED 2

#define WHITE_TURN true
#define BLACK_TURN false

#define WINNER_NONE 0
#define WINNER_WHITE 1
#define WINNER_BLACK 2
#define STALEMATE 3
#define DRAW 4

#define WHITE_CHECK 0
#define BLACK_CHECK 1
#define NO_CHECK 2

#define WHITE_STALE 0
#define BLACK_STALE 1
#define NO_STALE 2

//--------------------CHESS PIECE DEFINITIONS--------------------//

#define EMPTY 0

#define wPAWN 1
#define wKNIGHT 2
#define wBISHOP 3
#define wROOK 4
#define wKING 5
#define wQUEEN 6
/*
A note on WHITE_END:
because all white pieces' IDs are lower or equal to WHITE_END, and all black pieces' 
IDs are higher than WHITE_END, you can use WHITE_END to check if a piece
is white or black
*/
#define WHITE_END 6

//----------BLACK PIECES----------//
#define bPAWN 7
#define bKNIGHT 8
#define bBISHOP 9
#define bROOK 10
#define bKING 11
#define bQUEEN 12

//--------------------POSITION DEFINITIONS--------------------//
#define board_a 0
#define board_b 1
#define board_c 2
#define board_d 3
#define board_e 4
#define board_f 5
#define board_g 6
#define board_h 7

#define board_1 0
#define board_2 1
#define board_3 2
#define board_4 3
#define board_5 4
#define board_6 5
#define board_7 6
#define board_8 7
