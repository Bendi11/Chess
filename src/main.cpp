#include "bitboard.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    Bb b = BbFile::D;
    b |= (1 << BbSquare::a1);
    std::cout << bb_str(b) << std::endl;
    return 0;
}
