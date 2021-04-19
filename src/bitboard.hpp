#pragma once
#include <inttypes.h>
#include <string>
#include <type_traits>



/**
 * @brief Nasty template function to convert an enum to its underlying type
 * 
 * @tparam T The type of enum for the template function
 * @tparam std::is_enum<T>::type SFINAE to make sure that the template can only be created for an enum
 * @param e The enum to convert to the underlying type
 * @return std::underlying_type<T>::type The underlying type representing the enumeration
 */
template<class T, class U = typename std::is_enum<T>::type >
constexpr inline typename std::underlying_type<T>::type rawenum(T e) {
    return static_cast< std::underlying_type<T>::type>(e);
}


/**
 * @brief bitboard is a simple typedef for uint64_t, it is a raw number with no
 * extra methods or functions to help with bitwise operations 
 * 
 */
typedef uint64_t bitboard;


/**
 * @brief Wrapper class around a raw bitboard allowing member methods for bitboards
 * sizeof(Bb) is 8 bytes, the same size as a raw bitboard, but with member methods and
 * implicit conversions to and from raw 64 bit numbers 
 * 
 * @see bitboard
 * 
 */
class Bb
{
public:
    
    /**
     * @brief Constructor of the Bitboard class that takes any type castable
     * 
     * to uint64_t and sets the internal board to it
     * 
     * @tparam T The castable to uint64_t type
     * @tparam typename SFINAE for enabling the template only for types that cast to uitn64_t
     */
    template<typename T, typename = std::enable_if<std::is_convertible<T, uint64_t>::value > >
        constexpr Bb(const T& in) : m_bb{static_cast<bitboard>(in)} {}

    Bb() = default;

    //Implicitly convert the bitboard struct to a uint64_t when needed
    inline constexpr operator uint64_t() const {return m_bb;}

    //----------BINARY MATH OPERATORS----------//

    /// @brief Perform an OR on the bitboard and assign the value
    template<typename T, typename = std::enable_if<std::is_convertible<T, uint64_t>::value>>
    constexpr inline Bb& operator |=(const T& other) 
    {
        this->m_bb |= static_cast<uint64_t>(other);
        return *this;
    }

    /// @brief Perform AND on another bitboard and assign the value
    template<typename T, typename = std::enable_if<std::is_convertible<T, uint64_t>::value>>
    constexpr inline Bb& operator &=(const T& other) 
    {
        this->m_bb &= static_cast<uint64_t>(other);
        return *this;
    }

    /// @brief XOR two bitboards together and assign the value to the LHS
    template<typename T, typename = std::enable_if<std::is_convertible<T, uint64_t>::value>>
    constexpr inline Bb& operator ^=(const T& other)
    {
        this->m_bb ^= static_cast<uint64_t>(other);
        return *this;
    }

    //Bitshift a bitboard to the left and assign the expression result to the LHS
    inline Bb& operator <<=(uint64_t lshift)
    {
        this->m_bb <<= lshift;
        return *this;
    }

    //Bitshift a bitboard to the right and assign the value to the LHS
    inline Bb& operator >>=(uint64_t rshift)
    {
        this->m_bb >>= rshift;
        return *this;
    }


    /// @brief Bit shift a bitboard to the left 
    inline constexpr Bb operator <<(uint64_t lshift) const
    {
        return Bb(this->m_bb << lshift);
    }

    /// @brief Bit shift a bitboard to the right
    constexpr inline Bb operator >>(uint64_t rshift) const
    {
        return Bb(this->m_bb >> rshift);
    }

    /// @brief Apply a bitwise OR with this bitboard and another value that must be convertible to uint64_t
    template<typename T, typename = std::enable_if<std::is_convertible<T, uint64_t>::value>>
    constexpr inline Bb operator |(const T& other) const
    {
        return Bb(this->m_bb | static_cast<uint64_t>(other) );
    }
    
    /// @brief Apply a bitwise AND with this bitboard and another value convertible to uint64_t
    template<typename T, typename = std::enable_if<std::is_convertible<T, uint64_t>::value>>
    constexpr inline Bb operator &(const T& other) const
    {
        return Bb(this->m_bb & static_cast<uint64_t>(other) );
    }

    /// @brief XOR this bitboard and another value together
    template<typename T, typename = std::enable_if<std::is_convertible<T, uint64_t>::value>>
    constexpr inline Bb operator ^(const T& other) const
    {
        return Bb(this->m_bb ^ static_cast<uint64_t>(other));
    }

    /// @brief Get the nth bit of the bitboard
    inline constexpr bool nth(uint8_t n) const {return (m_bb >> n) & 1;}

    /// @brief Set the nth bit of the bitboard on or off
    inline constexpr void set(uint8_t n, bool set) { set ? (m_bb |= (1ULL << n) ) : (m_bb &= ~(1ULL << n)); }
    
    /// @brief Convert this bitboard into a pretty string
    const char* str(void) const;

    /// @brief Reverse the bit order of this bitboard, returning a bitboard representing the reversed bits
    Bb reverse(void) const 
    {
        bitboard rev = m_bb;
        rev = (rev & 0x5555555555555555) << 1 | (rev >> 1) & 0x5555555555555555;
        rev = (rev & 0x3333333333333333) << 2 | (rev >> 2) & 0x3333333333333333;
        rev = (rev & 0x0f0f0f0f0f0f0f0f) << 4 | (rev >> 4) & 0x0f0f0f0f0f0f0f0f;
        rev = (rev & 0x00ff00ff00ff00ff) << 8 | (rev >> 8) & 0x00ff00ff00ff00ff;

        return (rev << 48) | ((rev & 0xffff0000) << 16) |
            ((rev >> 16) & 0xffff0000) | (rev >> 48);
    }

    /// @brief Reverse a bitboard from an existing bitboard
    static Bb reverse(bitboard bits) 
    {
        bitboard rev = bits;
        rev = (rev & 0x5555555555555555) << 1 | (rev >> 1) & 0x5555555555555555;
        rev = (rev & 0x3333333333333333) << 2 | (rev >> 2) & 0x3333333333333333;
        rev = (rev & 0x0f0f0f0f0f0f0f0f) << 4 | (rev >> 4) & 0x0f0f0f0f0f0f0f0f;
        rev = (rev & 0x00ff00ff00ff00ff) << 8 | (rev >> 8) & 0x00ff00ff00ff00ff;

        return (rev << 48) | ((rev & 0xffff0000) << 16) |
            ((rev >> 16) & 0xffff0000) | (rev >> 48);
    }
private:
    /// @brief Internal raw bitboard that member methods operate on, this is the *ONLY* data that Bb contains, so the size of Bb
    /// is always == the size of bitboard == 8 bytes or 64 bits
    bitboard m_bb;
};


class Board
{
public:

private:
    Bb w_pawns;
    Bb b_pawns;

};

/**
 * @brief Taken from the chess programming wiki, this represents where all squares
 * on a little endian rank file bitboard are. An object of this class takes the same amount of memory
 * as a bitboard, 8 bytes
 * 
 */
class BbSquare
{
public:
    //Unscoped enum inside class to avoid needing to write BbSquare::Sq::a1 and allowing BbSquare::a1, so it is still scoped
    enum Sq : uint8_t
    {
        a1, b1, c1, d1, e1, f1, g1, h1,
        a2, b2, c2, d2, e2, f2, g2, h2,
        a3, b3, c3, d3, e3, f3, g3, h3,
        a4, b4, c4, d4, e4, f4, g4, h4,
        a5, b5, c5, d5, e5, f5, g5, h5,
        a6, b6, c6, d6, e6, f6, g6, h6,
        a7, b7, c7, d7, e7, f7, g7, h7,
        a8, b8, c8, d8, e8, f8, g8, h8
    };

    constexpr operator uint8_t() {return rawenum(this->val);}

    /**
     * @brief Overloaded constructor so that creating a BbSquare struct from an Sq enum is possible
     * 
     */
    constexpr BbSquare(Sq s) : val(s) {}
    BbSquare() = default;

    //Cast this class implicitly to the enum representing it
    constexpr operator Sq() const {return this->val;}
    //Implicity convert this enum to a bitboard representing the square on one bit
    constexpr operator bitboard() const {return static_cast<uint64_t>(static_cast<std::underlying_type<Sq>::type >(this->val) ); }

    //Compare two bitboard squares and check for equality
    constexpr bool operator==(const BbSquare& other) const {return this->val == other.val;}
    constexpr bool operator!=(const BbSquare::Sq& other) const {return this->val != other;}
    constexpr bool operator!=(const  BbSquare& other) const {return this->val != other.val;}

    BbSquare& operator++() {this->val = static_cast<Sq>(rawenum(this->val) + 1); return *this;}

    /// @brief Get the file of this square
    inline uint8_t file(void)
    {
        return static_cast<uint8_t>(rawenum(val) & 7);
    }

    /// @brief Get the rank index of this square
    inline uint8_t rank(void)
    {
        return static_cast<uint8_t>(rawenum(val) >> 3);
    }

    /// @brief Get the diagonal of this square
    inline uint8_t diagonal(void)
    {
        return 7 + rank() - file();
    }

    /// @brief Get the anti diagonal of this square
    inline uint8_t antidiagonal(void)
    {
        return rank() + file();
    }

    operator bool() = delete;
    
private:
    Sq val; //The value of the bitboard square
};


extern bitboard BISHOP_ATTACKS[64][512];
extern constexpr bitboard get_bishop_attacks(BbSquare square, bitboard occ);

#define REPEAT_8(bb) 0b##bb##bb##bb##bb##bb##bb##bb##bb
/// @brief Bitboard masks of each file on a chess board, 0 through 7
constexpr inline bitboard FILE_MASKS[8] = 
{
    REPEAT_8(00000001), //A file
    REPEAT_8(00000010), //B
    REPEAT_8(00000100), //C
    REPEAT_8(00001000), //D
    REPEAT_8(00010000), //E 
    REPEAT_8(00100000), //F
    REPEAT_8(01000000), //G
    REPEAT_8(10000000), //H
};
#undef REPEAT_8

/// @brief Rank bitboard masks
constexpr inline bitboard RANK_MASKS[8] = 
{
    0b11111111Ui64 << 0,
    0b11111111Ui64 << 8,
    0b11111111Ui64 << 16,
    0b11111111Ui64 << 24,
    0b11111111Ui64 << 32,
    0b11111111Ui64 << 40,
    0b11111111Ui64 << 48,
    0b11111111Ui64 << 56,
};   

/// @brief Diagonal bitboard masks
constexpr inline bitboard DIAG_MASKS[15] = 
{
    128, //H1 square
    32832, //G1 - H2 diag
    0b100000000100000000100000, //F1-H3 
    0b10000000010000000010000000010000,  //E1-H4
    0b1000000001000000001000000001000000001000, //D1-H5
    0b100000000100000000100000000100000000100000000100, //C1-H6
    0b10000000010000000010000000010000000010000000010000000010, //B1-H7
    0b1000000001000000001000000001000000001000000001000000001000000001, //A1-H8
    0b100000000100000000100000000100000000100000000100000000100000000,  //A2-G8
    0b10000000010000000010000000010000000010000000010000000000000000,   //A3-F8
    0b1000000001000000001000000001000000001000000000000000000000000,    //A4-E8
    0b100000000100000000100000000100000000000000000000000000000000,     //A5-D8
    0b10000000010000000010000000000000000000000000000000000000000,      //A6-C8
    0b1000000001000000000000000000000000000000000000000000000000,       //A7-B8
    0b100000000000000000000000000000000000000000000000000000000,        //A8
};

/// @brief Anti diagonal masks 
constexpr inline bitboard ANTIDIAG_MASKS[15]
{
    0b1, //A1 square
    0b100000010, //B1-A2
    0b10000001000000100, //You get the idea
    0b1000000100000010000001000,
    0b100000010000001000000100000010000,
    0b10000001000000100000010000001000000100000,
    0b1000000100000010000001000000100000010000001000000,
    0b100000010000001000000100000010000001000000100000010000000, //A8-H1
    0b1000000100000010000001000000100000010000001000000000000000,
    0b10000001000000100000010000001000000100000000000000000000000,
    0b100000010000001000000100000010000000000000000000000000000000,
    0b1000000100000010000001000000000000000000000000000000000000000,
    0b10000001000000100000000000000000000000000000000000000000000000,
    0b100000010000000000000000000000000000000000000000000000000000000,
    0b1000000000000000000000000000000000000000000000000000000000000000, //H8 square
};


/**
 * @brief Generate sliding piece attacks for a given mask of squares to travel, occupancy bitboard,
 * and square to move from
 * 
 * @param sq The square to move from
 * @param occupancy The occupancy bitboard for enemy pieces
 * @param mask The mask of squares to move
 * @return Bb The bitboard that represents valid attacks
 */
inline Bb slide_attacks(BbSquare sq, Bb occupancy, Bb mask)
{
    return (((mask & occupancy) - static_cast<Bb>(sq) * 2) ^ Bb::reverse((mask & occupancy).reverse() - (static_cast<Bb>(sq).reverse() * 2) )) & mask;
}

inline Bb bishop_attacks(BbSquare sq, Bb occ)
{
    return slide_attacks(sq, occ, DIAG_MASKS[sq.diagonal()]) | slide_attacks(sq, occ, ANTIDIAG_MASKS[sq.antidiagonal()]);
}

void init_bishop();

/**
 * @brief Structure with information about a direction on a compass rose,
 * with methods to manipulate a bitboard based on the direction
 * 
 */
struct Direction
{

};