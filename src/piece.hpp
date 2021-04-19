#pragma once

#include <variant>
#include <bitset>
#include "board.hpp"

class Board;

/**
 * @file piece.hpp
 * @brief piece.hpp defines all piece types like Pawn, Rook, etc.
 * 
 */


template<typename T, typename = std::enable_if<std::is_enum<T>::value>>
inline constexpr std::underlying_type_t<T> raw_enum(const T en)
{
    return static_cast<std::underlying_type_t<T>>(en);
}


/**
 * @brief The `Piece` type contains all data needed for every kind of chess piece
 * including kind of piece, wether the piece has moved, if the piece has just double moved (pawn specific), etc.
 * 
 */
class Piece
{
public:
    /**
     * @brief The `Piece::Type` struct wraps an enum specifying the type of piece that this is
     * and adds member methods to the enumeration
     * 
     */
    struct Type
    {
    public:
        /// @brief The unscoped enum inside of `Piece::Type` that specifies the type of piece that this is
        enum Ty
        {
            PAWN,
            ROOK,
            BISHOP,
            KNIGHT,
            KING,
            QUEEN,
        };

        /// Implicitly convert this struct to its underlying type
        inline constexpr operator Ty() {return m_type;}
        inline constexpr Type(Ty t) : m_type{t} {}
    private:
        /// @brief The internal enumeration value
        Ty m_type; 
    };

    /// Generate moves for this piece given a board configuration
    void gen_moves(const Board& b);
private:
    /// @brief The kind of piece that this is
    Type m_kind;
    /// The color of this chess piece
    enum class Flags : std::size_t
    {
        /// If this piece has just moved
        MOVED,
        /// The color of this piece: true is white
        COLOR,
        /// If this piece has just double moved
        JUST_DOUBLEMOVED,

        MAX,
    };

    /// Many chess values are simple ON/OFF switches with only two possible states, so
    /// if we keep them in a bitset, the memory required by all flags is ~1 byte
    std::bitset<static_cast<std::size_t>(Flags::MAX)> m_flags; 

    /// A list of legal moves that we can make
    std::vector<Move> m_moves;

    /// The position of this piece on the chess board
    Position m_pos;     

    /// Add a move if the square if empty
    void add_move_empty(const Board&, Position);

    /// Add a move if the square if filled with an enemy
    void add_move_attack(const Board&, Position);

    friend class Board;
};

