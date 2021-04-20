#pragma once

#include <vector>
#include <optional>

class Board;

/// @brief The position struct is used to specify a position on the chess board
struct Position
{
public:
    /// @brief X coordinate on the board; This is the file on chess boards
    std::size_t x = 0; 
    /// @brief Y coordinate on the board; This is the rank on chess boards
    std::size_t y = 0;

    Position(std::size_t x, std::size_t y) : x{x}, y{y} {}
};

/// @brief The `Move` struct holds all information about a move including some metadata
struct Move
{
public:
    Move(Position old, Position _new) : m_old{old}, m_new{_new} {}

    /// @brief The old position that this move moves from
    Position m_old; 
    /// @brief The new position that this move moves to
    Position m_new;
};


#include <variant>
#include <bitset>
#include <string>
#include <vector>
#include "board.hpp"

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

        MAX
    };

    /// Many chess values are simple ON/OFF switches with only two possible states, so
    /// if we keep them in a bitset, the memory required by all flags is ~1 byte
    std::bitset<static_cast<std::size_t>(Flags::MAX)> m_flags; 

    /// A list of legal moves that we can make
    std::vector<Move> m_moves;

    /// The position of this piece on the chess board
    Position m_pos;     

    /// Add a move if the square if empty
    void add_move_empty(const Board&, Position&);

    /// Add a move if the square if filled with an enemy
    void add_move_attack(const Board&, Position&);

    friend class Board;
};

/**
 * @brief The Board class contains the representation of the chess board and methods to make moves,
 * visualize the board, etc.
 * 
 */
class Board
{
public:
    inline const std::vector<std::optional<Piece>> operator [](const std::size_t s) const {return m_board[s];}

    inline bool valid(std::size_t x, std::size_t y) const {return (x < 8 && y < 8);}
private:
    /// @brief The 8x8 for chess array of pieces that are the board
    std::vector<std::vector<std::optional<Piece>>> m_board;

    /// @brief Generate moves for every piece on the board
    void gen_moves(void);
};

