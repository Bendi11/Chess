#include "board.hpp"
#include "log.hpp"
#include "portaudio.h"

void Piece::gen_moves(Board& board)
{
    m_moves.clear();
    Position move_to;

    #define SLIDE(vertical, horizontal)                    \
    move_to = m_pos;                                       \
    do                                                     \
    {                                                      \
        move_to.x ##horizontal ;                           \
        move_to.y ##vertical ;                      \
        if(!move_to.valid())                               \
            break;                                         \
        add_move_empty(board, move_to);                    \
        add_move_attack(board, move_to);                   \
        \
    } while(move_to.valid() && !board.filled(move_to))    

    switch(m_kind)
    {
        case Type::PAWN:
        {
            int modifier = (m_flags.test(Flags::COLOR) ) ? 1 : -1; //Get a modifier for our movement direction based on color
            add_move_empty(board, Position(m_pos.x, m_pos.y + modifier));
            add_move_attack(board, Position(m_pos.x + 1, m_pos.y + modifier));
            add_move_attack(board, Position(m_pos.x - 1, m_pos.y + modifier));
            if(!m_flags.test(Flags::MOVED) && !board.filled(Position(m_pos.x, m_pos.y + modifier)))
                add_move_empty(board, Position(m_pos.x, m_pos.y + modifier + modifier));
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
        
        case Type::KNIGHT:
        {
            add_move_attack(board, Position(m_pos.x + 2, m_pos.y + 1) );
            add_move_empty(board, Position(m_pos.x + 2, m_pos.y + 1) );

            add_move_attack(board, Position(m_pos.x + 2, m_pos.y - 1) );
            add_move_empty(board, Position(m_pos.x + 2, m_pos.y - 1) );

            add_move_attack(board, Position(m_pos.x - 2, m_pos.y + 1) );
            add_move_empty(board, Position(m_pos.x - 2, m_pos.y + 1) );

            add_move_attack(board, Position(m_pos.x - 2, m_pos.y - 1) );
            add_move_empty(board, Position(m_pos.x - 2, m_pos.y - 1) );


            add_move_attack(board, Position(m_pos.x + 1, m_pos.y + 2) );
            add_move_empty(board, Position(m_pos.x + 1, m_pos.y + 2) );

            add_move_attack(board, Position(m_pos.x - 1, m_pos.y + 2) );
            add_move_empty(board, Position(m_pos.x - 1, m_pos.y + 2) );

            add_move_attack(board, Position(m_pos.x + 1, m_pos.y - 2) );
            add_move_empty(board, Position(m_pos.x + 1, m_pos.y - 2) );

            add_move_attack(board, Position(m_pos.x - 1, m_pos.y - 2) );
            add_move_empty(board, Position(m_pos.x - 1, m_pos.y - 2) );
        } break;
        default:

        case Type::KING:
        {
            // Top of king
            add_move_attack(board, Position(m_pos.x + 1, m_pos.y + 1) );
            add_move_empty(board, Position(m_pos.x + 1, m_pos.y + 1) );
            add_move_attack(board, Position(m_pos.x, m_pos.y + 1) );
            add_move_empty(board, Position(m_pos.x, m_pos.y + 1) );
            add_move_attack(board, Position(m_pos.x - 1, m_pos.y + 1) );
            add_move_empty(board, Position(m_pos.x - 1, m_pos.y + 1) );

            // Bottom of king
            add_move_attack(board, Position(m_pos.x + 1, m_pos.y - 1) );
            add_move_empty(board, Position(m_pos.x + 1, m_pos.y - 1) );
            add_move_attack(board, Position(m_pos.x, m_pos.y - 1) );
            add_move_empty(board, Position(m_pos.x, m_pos.y - 1) );
            add_move_attack(board, Position(m_pos.x - 1, m_pos.y - 1) );
            add_move_empty(board, Position(m_pos.x - 1, m_pos.y - 1) );

            // Two sides
            add_move_attack(board, Position(m_pos.x + 1, m_pos.y) );
            add_move_empty(board, Position(m_pos.x + 1, m_pos.y) );
            add_move_attack(board, Position(m_pos.x - 1, m_pos.y) );
            add_move_empty(board, Position(m_pos.x - 1, m_pos.y) );
        } break;

        case Type::QUEEN:
        {
            SLIDE(++, ;);
            SLIDE(--, ;);
            SLIDE(;, --);
            SLIDE(;, ++);
            SLIDE(++, ++);
            SLIDE(++, --);
            SLIDE(--, ++);
            SLIDE(-- ,--);
        } break;
        break;
    }
    #undef SLIDE
}

void Board::gen_moves(void)
{
    for(auto& file : m_board)
        for(auto& rank : file)
            if(rank.has_value()) rank.value().gen_moves(*this); //Get moves for each piece 

    white_checkmate = true;
    black_checkmate = true;

    //Check if a move would put a side in check and remove that move
    for(auto& piece : pieces())
    {
        for(std::size_t idx = 0; idx < piece.get().m_moves.size(); ++idx)
        {
            auto& mov = piece.get().m_moves[idx];
            Board cpy = Board(*this);
            cpy.make_move(mov);
            for(auto& _file : cpy.m_board)
                for(auto& _rank : _file)
                    if(_rank.has_value()) _rank.value().gen_moves(cpy); //Get moves for each piece 

            if(cpy.in_check(piece.get().m_flags[Piece::Flags::COLOR]))  //Remove the move if it puts the side in check
            {
                piece.get().m_moves.erase(std::remove(piece.get().m_moves.begin(), piece.get().m_moves.end(), mov));
                idx--;
            }
            else 
            {
                if(piece.get().m_flags[Piece::Flags::COLOR])
                    white_checkmate = false; 
                else 
                    black_checkmate = false;
            }
        }
    }

    msg::info("White %s in checkmate, and black %s in checkmate", (white_checkmate) ? "is" : "isn't", (black_checkmate) ? "is" : "isn't");
    
}

bool Piece::add_move_empty(Board& b, Position& p)
{
    if(p.valid() && !b.filled(p)) m_moves.push_back(Move(m_pos, p));
    return false;
}

bool Piece::add_move_attack(Board& b, Position& p)
{
    if(p.valid() && b[p]) 
    if(b[p]->m_flags[Flags::COLOR] != m_flags[Flags::COLOR])
    {
         m_moves.push_back(Move(m_pos, p));
         return true;
    }
    return false;
    
}