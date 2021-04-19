#include "bitboard.hpp"
#include <iostream>



int main(int argc, char* argv[])
{
    init_bishop();
    Bb b = 0;
    b.set(BbSquare::c3, true);
    b.set(BbSquare::g7, true);
    //b |= 1 << BbSquare::a1;
    //b.set(BbSquare::a4, true);
    std::cout << Bb(get_bishop_attacks(BbSquare::g7, b) ).str() << "\n Occ:\n" << b.str() << std::endl;
    char c;
    std::cin >> c;
    return 0;
}
