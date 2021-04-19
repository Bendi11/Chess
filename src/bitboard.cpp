#include "bitboard.hpp"

const char* Bb::str(void) const
{
    char *bb_s = new char[73];
    std::fill(bb_s, bb_s + 72, '\n');
    bb_s[72] = '\0';

    std::size_t str_y = 0;

    for(int y = 0; y < 8; ++y)
    {
        for(size_t x = 0; x < 8; ++x)
        {
            bb_s[8 * y + x + str_y] = ((m_bb >> (-(y - 7) * 8 + x)) & 1) ? '1' : '0'; //Index the string like a 2D array and insert the bitboard bit into the string as a '0' or '1'
        }
        str_y++;
    }

    return bb_s;
}

