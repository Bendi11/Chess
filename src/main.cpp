#include "bitboard.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    Bb b = RANK_MASKS[3];
    //b |= 1 << BbSquare::a1;
    //b.set(BbSquare::a4, true);
    std::cout << b.str() << std::endl;
    return 0;
}
