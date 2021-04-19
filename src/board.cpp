#include "board.hpp"
#include "piece.hpp"

void Piece::gen_moves(const Board& board)
{
    m_moves.clear();
    
    switch(m_kind)
    {
        case Type::PAWN:
        {
            int modifier = (m_flags.test(raw_enum(Flags::COLOR)) ) ? 1 : -1; //Get a modifier for our movement direction based on color
            add_move_empty(board, Position(m_pos.x, m_pos.y + modifier));
            add_move_attack(board, Position(m_pos.x + 1, m_pos.y + modifier));
            add_move_attack(board, Position(m_pos.x - 1, m_pos.y + modifier));
        } break;

        default:

        break;
    }
}

void Piece::add_move_empty(const Board& b, Position p)
{
    if(b.valid(p.x, p.y) && !b[p.x][p.y]) m_moves.push_back(Move(m_pos, p));
}

void Piece::add_move_attack(const Board& b, Position p)
{
    if(b.valid(p.x, p.y) && b[p.x][p.y] && b[p.x][p.y]->m_flags[raw_enum(Flags::COLOR)] != m_flags[raw_enum(Flags::COLOR)]) m_moves.push_back(Move(m_pos, p));
}