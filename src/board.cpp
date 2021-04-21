#include "board.hpp"
#include "log.hpp"

void Piece::gen_moves(const Board& board)
{
    m_moves.clear();
    Position move_to;

    #define SLIDE(vertical, horizontal)                    \
    move_to = m_pos;                                       \
    move_to.x ##horizontal ;                               \
    move_to.y ##vertical   ;                               \
    do                                                     \
    {                                                      \
        if(!move_to.valid())                               \
            break;                                         \
        add_move_empty(board, move_to);                    \
        add_move_attack(board, move_to);                   \
        move_to.x ##horizontal ;                           \
        move_to.y ##vertical ;                             \
    } while(move_to.valid() && !board.filled(move_to))    

    switch(m_kind)
    {
        case Type::PAWN:
        {
            int modifier = (m_flags.test(Flags::COLOR) ) ? 1 : -1; //Get a modifier for our movement direction based on color
            add_move_empty(board, Position(m_pos.x, m_pos.y + modifier));
            add_move_attack(board, Position(m_pos.x + 1, m_pos.y + modifier));
            add_move_attack(board, Position(m_pos.x - 1, m_pos.y + modifier));
        } break;
        case Type::BISHOP:
        {
            SLIDE(++, ++);
            SLIDE(++, --);
            SLIDE(--, ++);
            SLIDE(-- ,--);
        } break;
        
        case Type::ROOK:
        {
            SLIDE(++, ;);
            SLIDE(--, ;);
            SLIDE(;, --);
            SLIDE(;, ++);
        } break;

        default:

        break;
    }
    #undef SLIDE
}

void Board::gen_moves(void)
{
    for(auto& piece : pieces())
        piece.gen_moves(*this); //Get moves for each piece 
    
}

bool Piece::add_move_empty(const Board& b, Position& p)
{
    if(p.valid() && !b.filled(p)) m_moves.push_back(Move(m_pos, p));
    return false;
}

bool Piece::add_move_attack(const Board& b, Position& p)
{
    if(p.valid() && b[p]) 
    if(b[p]->m_flags[Flags::COLOR] != m_flags[Flags::COLOR])
    {
         m_moves.push_back(Move(m_pos, p));
         return true;
    }
    return false;
    
}