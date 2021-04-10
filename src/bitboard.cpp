#include "bitboard.hpp"

std::string bb_str(Bb bb)
{
    std::string bb_s; //String holding the representation of the bitboard in text
    bb_s.resize(64, '\n'); //Resize the string to hold all bitboard bits in a string + a newline after every 8 bits
    size_t idx = 0;

    for(int y = 0; y < 8; ++y)
    {
        for(size_t x = 0; x < 8; ++x)
        {
            bb_s[8 * y + x] = ((bb >> (-(y - 7) * 8 + x)) & 1) ? '1' : '0'; //Index the string like a 2D array and insert the bitboard bit into the string as a '0' or '1'
        }
    }

    //Insert newlines into the bitboard string to separate it and make it look like an actual chess board
    for(size_t i = 0; i < 9; ++i)
        bb_s.insert(i * 9, "\n");

    return bb_s;
}
