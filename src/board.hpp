#pragma once

#include <vector>
#include <optional>

#include "piece.hpp"

using std::optional;

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

/**
 * @brief The Board class contains the representation of the chess board and methods to make moves,
 * visualize the board, etc.
 * 
 */
class Board
{
public:
    inline const std::vector<optional<Piece>> operator [](const std::size_t s) const {return m_board[s];}

    inline bool valid(std::size_t x, std::size_t y) const {return (x < 8 && y < 8);}
private:
    /// @brief The 8x8 for chess array of pieces that are the board
    std::vector<std::vector<optional<Piece>>> m_board;

    /// @brief Generate moves for every piece on the board
    void gen_moves(void);
};