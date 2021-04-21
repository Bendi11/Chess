#pragma once

#include <optional>
#include <variant>
#include <bitset>
#include <array>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include "log.hpp"
#include "board.hpp"

/**
 * @file board.hpp
 * @brief The board.hpp files define commonly used objects in chess like `Piece`s, `Position`s,
 * `Move`s, and the only class that should be used externally, the `Board` class that contains and manages
 * a chess game, keeping track of all metadata and other information like castling rights, en passant, etc.
 * @see Board
 * @see Piece
 * 
 */

class Board;

/// @brief The position struct is used to specify a position on the chess board
struct Position
{
public:
    /// @brief X coordinate on the board; This is the file on chess boards
    std::size_t x = 0; 
    /// @brief Y coordinate on the board; This is the rank on chess boards
    std::size_t y = 0;

    /// Ensure that a position is valid
    inline bool valid(void) const {return (x >= 0 && y >= 0 && x < 8 && y < 8);}
    /// Ensure that a position is valid given an x and y coordinate
    static inline bool valid(std::size_t t_x, std::size_t t_y) {return (t_x >= 0 && t_y >= 0 && t_x < 8 && t_y < 8);}

    /**
     * @brief Construct a new Position object using a string formatted in the form
     * 'A3' or 'h4' 
     * 
     * @param str_mov The string reference to construct the position from
     */
    Position(const std::string_view& str_mov) 
    {
        if(str_mov.length() < 2) return; //Make sure that we have enough characters to make a move from
        switch(std::tolower(str_mov[0]))
        {
            case 'a': x = 0; break;
            case 'b': x = 1; break;
            case 'c': x = 2; break;
            case 'd': x = 3; break;
            case 'e': x = 4; break;
            case 'f': x = 5; break;
            case 'g': x = 6; break;
            case 'h': x = 7; break;
            default: x = 1000; break; //Make an invalid move if the string is bad
        };
        y = static_cast<std::size_t>(str_mov[1] - '0'); //Convert the ASCII of character 2 to a std::size_t
    }
    Position(std::size_t x, std::size_t y) : x{x}, y{y} {}
    Position() = default;
};

/// @brief The `Move` struct holds all information about a move including some metadata
struct Move
{
public:
    /// Construct a move using a string formatted in (oldpos)(newpos)
    /// like 'a2a4' or 'B3c4'
    Move(const std::string_view& str_mov) : m_old{str_mov.substr(0, 2)}, m_new{str_mov.substr(2)}
    {
        
    }
    Move(Position old, Position _new) : m_old{old}, m_new{_new} {}

    /// @brief The old position that this move moves from
    Position m_old; 
    /// @brief The new position that this move moves to
    Position m_new;

    bool operator ==(const Move& other) const
    {
        return (m_old.x == other.m_old.x && m_old.y == other.m_old.y && other.m_new.x == m_new.x && other.m_new.y == m_new.y);
    }

    bool valid(void) const {return m_old.valid() && m_new.valid();}
};

/**
 * @brief Convert an enum to its underlying type
 * 
 * @tparam T The type of enum that this function is called for
 * @tparam typename SFINAE to ensure that the function can only be called for enum types
 * @param en The enum type to convert to its underlying type
 * @return constexpr std::underlying_type_t<T> The underlying value of the enum
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

    Piece(Type ty, bool white) : m_kind{ty}, m_moves{}, m_pos{0, 0}
    {
        m_flags[Flags::COLOR] = white;
    }

    /// Generate moves for this piece given a board configuration
    void gen_moves(Board& b);
private:
    /// @brief The kind of piece that this is
    Type m_kind;
    /// The color of this chess piece
    enum Flags 
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
    bool add_move_empty(Board&, Position&);

    /// Add a move if the square if filled with an enemy
    /// @return true if an attack was made
    bool add_move_attack(Board&, Position&);

    friend class Board;
    friend class ChessGui;
};

/**
 * @brief The Board class contains the representation of the chess board and methods to make moves,
 * visualize the board, etc.
 * 
 */
class Board
{
public:
    /// Create a chess board with the default layout for chess, white pieces on the bottom and black pieces on the top
    Board() :
    m_board{
        {
            {Piece(Piece::Type::ROOK, true),   Piece(Piece::Type::PAWN, true), std::nullopt, std::nullopt, std::nullopt, std::nullopt, Piece(Piece::Type::PAWN, false), Piece(Piece::Type::ROOK, false)}, 
            {Piece(Piece::Type::KNIGHT, true), Piece(Piece::Type::PAWN, true), std::nullopt, std::nullopt, std::nullopt, std::nullopt, Piece(Piece::Type::PAWN, false), Piece(Piece::Type::KNIGHT, false)},
            {Piece(Piece::Type::BISHOP, true), Piece(Piece::Type::PAWN, true), std::nullopt, std::nullopt, std::nullopt, std::nullopt, Piece(Piece::Type::PAWN, false), Piece(Piece::Type::BISHOP, false)},
            {Piece(Piece::Type::QUEEN, true),  Piece(Piece::Type::PAWN, true), std::nullopt, std::nullopt, std::nullopt, std::nullopt, Piece(Piece::Type::PAWN, false), Piece(Piece::Type::QUEEN, false)},
            {Piece(Piece::Type::KING, true),   Piece(Piece::Type::PAWN, true), std::nullopt, std::nullopt, std::nullopt, std::nullopt, Piece(Piece::Type::PAWN, false), Piece(Piece::Type::KING, false)},
            {Piece(Piece::Type::BISHOP, true), Piece(Piece::Type::PAWN, true), std::nullopt, std::nullopt, std::nullopt, std::nullopt, Piece(Piece::Type::PAWN, false), Piece(Piece::Type::BISHOP, false)},
            {Piece(Piece::Type::KNIGHT, true), Piece(Piece::Type::PAWN, true), std::nullopt, std::nullopt, std::nullopt, std::nullopt, Piece(Piece::Type::PAWN, false), Piece(Piece::Type::KNIGHT, false)},
            {Piece(Piece::Type::ROOK, true),   Piece(Piece::Type::PAWN, true), std::nullopt, std::nullopt, std::nullopt, std::nullopt, Piece(Piece::Type::PAWN, false), Piece(Piece::Type::ROOK, false)}
        }
    }
    {
        update_pos();
        gen_moves();
    }

    /// Get the rank at the specified file index
    inline const std::array<std::optional<Piece>, 8> operator [](const std::size_t s) const {return m_board[s];}

    inline std::optional<Piece>& operator [](const Position& pos) {return m_board[pos.x][pos.y];}

    /// Return a list containing all pieces on the board
    inline std::list<Piece> pieces(void) const
    {
        std::list<Piece> ret; //The returned list of pieces
        for(auto& file : m_board)
            for(auto& rank : file)
            {
                if(rank.has_value())
                    ret.push_back(rank.value()); //Add the piece to our list
            }
        return ret;
    }

    /// Get if a position is filled in the board
    inline bool filled(Position pos) const {return m_board[pos.x][pos.y].has_value();}

    /// Make a move using a Move struct
    inline bool make_move(const Move& mov)
    {
        if(!mov.valid()) return false;
        if(!m_board[mov.m_old.x][mov.m_old.y].has_value()) //Ensure there is a piece there
            return false;
        
        Piece& moved = *m_board[mov.m_old.x][mov.m_old.y];
        bool has_move = false;
        for(auto& move : moved.m_moves)
        {
            if(mov == move)
                has_move = true;
        }
        if(!has_move) return false; //Ensure that the piece has the move 

        moved.m_flags.set(Piece::Flags::MOVED);
        this->operator[](mov.m_new).swap(this->operator [](mov.m_old)); //Move the piece
        this->operator[](mov.m_old).reset(); //Remove the old piece
        update_pos();
        
        return true; //Return that the move was good
    }


    /// @brief Generate moves for every piece on the board
    void gen_moves(void);
private:
    /// @brief The 8x8 for chess array of pieces that are the board
    std::array<std::array<std::optional<Piece>, 8>, 8> m_board;

    void update_pos(void)
    {
        //Set the positions of each chess piece
        std::size_t x = 0;
        std::for_each(m_board.begin(), m_board.end(), [&x](std::array<std::optional<Piece>, 8>& file) 
        {
            std::size_t y = 0;
            std::for_each(file.begin(), file.end(), [&x, &y](std::optional<Piece>& piece) 
            {
                if(piece.has_value()) piece.value().m_pos = Position(x, y);
                y++;
            });
            x++;
        });
    }
};

