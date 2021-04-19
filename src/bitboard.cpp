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

constexpr bitboard BISHOP_MAGICS[64] = {
	0x0002020202020200, 0x0002020202020000, 0x0004010202000000, 0x0004040080000000,
	0x0001104000000000, 0x0000821040000000, 0x0000410410400000, 0x0000104104104000,
	0x0000040404040400, 0x0000020202020200, 0x0000040102020000, 0x0000040400800000,
	0x0000011040000000, 0x0000008210400000, 0x0000004104104000, 0x0000002082082000,
	0x0004000808080800, 0x0002000404040400, 0x0001000202020200, 0x0000800802004000,
	0x0000800400A00000, 0x0000200100884000, 0x0000400082082000, 0x0000200041041000,
	0x0002080010101000, 0x0001040008080800, 0x0000208004010400, 0x0000404004010200,
	0x0000840000802000, 0x0000404002011000, 0x0000808001041000, 0x0000404000820800,
	0x0001041000202000, 0x0000820800101000, 0x0000104400080800, 0x0000020080080080,
	0x0000404040040100, 0x0000808100020100, 0x0001010100020800, 0x0000808080010400,
	0x0000820820004000, 0x0000410410002000, 0x0000082088001000, 0x0000002011000800,
	0x0000080100400400, 0x0001010101000200, 0x0002020202000400, 0x0001010101000200,
	0x0000410410400000, 0x0000208208200000, 0x0000002084100000, 0x0000000020880000,
	0x0000001002020000, 0x0000040408020000, 0x0004040404040000, 0x0002020202020000,
	0x0000104104104000, 0x0000002082082000, 0x0000000020841000, 0x0000000000208800,
	0x0000000010020200, 0x0000000404080200, 0x0000040404040400, 0x0002020202020200
};
bitboard BISHOP_ATTACKS[64][512];
int BISHOP_ATTACK_SHIFTS[64];
bitboard BISHOP_ATTACK_MASKS[64];

constexpr bitboard k1 = 0x5555555555555555;
constexpr bitboard k2 = 0x3333333333333333;
constexpr bitboard k4 = 0x0f0f0f0f0f0f0f0f;
constexpr bitboard kf = 0x0101010101010101;

//Returns number of set bits in the bitboard
inline int pop_count(bitboard x) {
	x = x - ((x >> 1) & k1);
	x = (x & k2) + ((x >> 2) & k2);
	x = (x + (x >> 4)) & k4;
	x = (x * kf) >> 56;
	return int(x);
}

void init_bishop()
{
    bitboard edges, subset, index;

	for (BbSquare sq = BbSquare::a1; sq != BbSquare::h8; ++sq) {
		edges = ((RANK_MASKS[0] | RANK_MASKS[7]) & ~RANK_MASKS[sq.rank()]) |
			((FILE_MASKS[0] | FILE_MASKS[7]) & ~FILE_MASKS[sq.file()]);
		BISHOP_ATTACK_MASKS[rawenum((BbSquare::Sq)(sq) )] = (DIAG_MASKS[sq.diagonal()]
			^ ANTIDIAG_MASKS[sq.antidiagonal()]) & ~edges;
		BISHOP_ATTACK_SHIFTS[rawenum((BbSquare::Sq)(sq) )] = 64 - pop_count(BISHOP_ATTACK_MASKS[(uint8_t)sq]);

		subset = 0;
		do {
			index = subset;
			index = index * BISHOP_MAGICS[(uint8_t)sq];
			index = index >> BISHOP_ATTACK_SHIFTS[(uint8_t)sq];
			BISHOP_ATTACKS[(uint8_t)sq][index] = bishop_attacks(sq, subset);
			subset = (subset - BISHOP_ATTACK_MASKS[(uint8_t)sq]) & BISHOP_ATTACK_MASKS[(uint8_t)sq];
		} while (subset);
	}
}

constexpr bitboard get_bishop_attacks(BbSquare square, bitboard occ) 
{
	return BISHOP_ATTACKS[(uint8_t)square][((occ & BISHOP_ATTACK_MASKS[(uint8_t)square]) * BISHOP_MAGICS[(uint8_t)square])
		>> BISHOP_ATTACK_SHIFTS[(uint8_t)square]];
}