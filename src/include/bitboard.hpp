#pragma once
#include <inttypes.h>
#include <string>
#include <type_traits>

//Bitboard type that is just a 64 bit integer
typedef uint64_t bitboard;


class Bb
{
public:
    //Bb(const uint64_t& c) : m_bb{c} {}
    
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

    inline Bb& operator |=(const Bb& other) 
    {
        this->m_bb |= other.m_bb;
        return *this;
    }

    const bool nth(uint8_t n) const {return (m_bb >> n) & 1;}

private:
    bitboard m_bb; //Internal 64 bit number representing the bitboard
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
    enum Sq : bitboard
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

    /**
     * @brief Overloaded constructor so that creating a BbSquare struct from an Sq enum is possible
     * 
     */
    constexpr BbSquare(Sq s) : val(s) {}
    BbSquare() = default;

    //Cast this class implicitly to the enum representing it
    constexpr operator Sq() const {return this->val;}
    //Implicity convert this enum to a bitboard representing the square on one bit
    constexpr operator bitboard() const {return static_cast<std::underlying_type<Sq>::type >(this->val); }

    //Compare two bitboard squares and check for equality
    constexpr bool operator==(const BbSquare& other) const {return this->val == other.val;}
    constexpr bool operator!=(const  BbSquare& other) const {return this->val != other.val;}

    operator bool() = delete;
    
private:
    Sq val; //The value of the bitboard square
};

/**
 * @brief Enumeration representing all bitboard files 
 * 
 */
enum class BbFile : bitboard
{
    A = 0b00000001'00000001'00000001'00000001'00000001'00000001'00000001'00000001,
    B = A * 2,
    C = B * 2,
    D = C * 2,
    E = D * 2,
    F = E * 2,
    G = F * 2,
    H = G * 2,
};

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
 * @brief Function to convert a bitboard u64 into a pretty string
 * 
 * @param bitboard The bitboard to stringify
 * @return std::string The bitboard string 
 */
std::string bb_str(Bb);

/**
 * @brief Structure with information about a direction on a compass rose,
 * with methods to manipulate a bitboard based on the direction
 * 
 */
struct Direction
{

};