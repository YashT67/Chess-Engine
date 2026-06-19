#include "move.hpp"
#include "board.hpp"
#include "table.hpp"
#include <cstdlib>

// Generate legal moves for rooks with color = 'color' and append it to the move list
static void gen_rook_moves(MoveList &move_list, bool color) {
    Bitboard rook = board.pieces[color][ROOK];
    while(rook) {
        int from = __builtin_ctzll(rook);
        rook &= (rook - 1);

        Bitboard blockers = board.common & table.rook_mask[from];
        int magic_index = (blockers * rook_magic[from]) >> (64 - rook_relevant_bits[from]);
        Bitboard vision = table.rook_table[from][magic_index] & ~board.by_color[color];

        while(vision) {
            int to = __builtin_ctzll(vision);
            vision &= (vision - 1);
            int flag = ((1ULL << to) & board.by_color[!color])? CAPTURE : QUIET_MOVE;
            move_list.add(encode_move(from, to, flag));
        }
    }
}

// Generate legal moves for bishops with color = 'color' and append it to the move list
static void gen_bishop_moves(MoveList &move_list, bool color) {
    Bitboard bishop = board.pieces[color][BISHOP];
    while(bishop) {
        int from = __builtin_ctzll(bishop);
        bishop &= (bishop - 1);

        Bitboard blockers = board.common & table.bishop_mask[from];
        int magic_index = (blockers * bishop_magic[from]) >> (64 - bishop_relevant_bits[from]);
        Bitboard vision = table.bishop_table[from][magic_index] & ~board.by_color[color];

        while(vision) {
            int to = __builtin_ctzll(vision);
            vision &= (vision - 1);
            int flag = ((1ULL << to) & board.by_color[!color])? CAPTURE : QUIET_MOVE;
            move_list.add(encode_move(from, to, flag));
        }
    }
}

// Generate legal moves for queens with color = 'color' and append it to the move list
static void gen_queen_moves(MoveList &move_list, bool color) {
    Bitboard queen = board.pieces[color][QUEEN];
    while(queen) {
        int from = __builtin_ctzll(queen);
        queen &= (queen - 1);

        Bitboard vision = ~board.by_color[color];
        Bitboard straight_blockers = board.common & table.rook_mask[from];
        int straight_magic_index = (straight_blockers * rook_magic[from]) >> (64 - rook_relevant_bits[from]);
        vision &= table.rook_table[from][straight_magic_index];
        Bitboard diagonal_blockers = board.common & table.bishop_mask[from];
        int diagonal_magic_index = (diagonal_blockers * bishop_magic[from]) >> (64 - bishop_relevant_bits[from]);
        vision &= table.bishop_table[from][diagonal_magic_index];

        while(vision) {
            int to = __builtin_ctzll(vision);
            vision &= (vision - 1);
            int flag = ((1ULL << to) & board.by_color[!color])? CAPTURE : QUIET_MOVE;
            move_list.add(encode_move(from, to, flag));
        }
    }
}

// Generate legal moves for knights with color = 'color' and append it to the move list
static void gen_knight_moves(MoveList &move_list, bool color) {
    Bitboard knight = board.pieces[color][KNIGHT];
    while(knight) {
        int from = __builtin_ctzll(knight);
        knight &= (knight - 1);

        Bitboard vision = table.knight_table[from] & ~board.by_color[color];
        while(vision) {
            int to = __builtin_ctzll(vision);
            vision &= (vision - 1);
            int flag = ((1ULL << to) & board.by_color[!color])? CAPTURE : QUIET_MOVE;
            move_list.add(encode_move(from, to, flag));
        }
    }
}

// Generate legal moves for king with color = 'color' and append it to the move list
static void gen_king_moves(MoveList &move_list, bool color) {
    Bitboard king = board.pieces[color][KNIGHT];
    while(king) {
        int from = __builtin_ctzll(king);
        king &= (king - 1);

        Bitboard vision = table.king_table[from] & ~board.by_color[color];
        while(vision) {
            int to = __builtin_ctzll(vision);
            vision &= (vision - 1);
            int flag = ((1ULL << to) & board.by_color[!color])? CAPTURE : QUIET_MOVE;
            move_list.add(encode_move(from, to, flag));
        }
    }
}

// Generate legal moves for pawns with color = 'color' and append it to the move list
static void gen_pawn_moves(MoveList &move_list, bool color) {
    Bitboard pawn = board.pieces[color][PAWN];

    if(color == WHITE) {
        Bitboard single_push = (pawn << 8) & ~board.common;
        Bitboard double_push = (single_push << 8) & table.rank_mask[RANK_4] & ~board.common;
        Bitboard targets = board.by_color[BLACK] | ((board.en_passent == NO_SQ)? 0 : (1ULL << board.en_passent));
        Bitboard left_capture = (pawn << 7) & targets & ~table.file_mask[FILE_H];
        Bitboard right_capture = (pawn << 9) & targets & ~table.file_mask[FILE_A];
        
        while(single_push) {
            int to = __builtin_ctzll(single_push);
            single_push &= (single_push - 1);
            if(to >= 56) {
                int flag = 8;
                for(int i = 0; i < 4; i++) move_list.add(encode_move(to-8, to, flag+i));
            }
            else move_list.add(encode_move(to-8, to, QUIET_MOVE));
        }
        while(double_push) {
            int to = __builtin_ctzll(double_push);
            double_push &= (double_push - 1);
            move_list.add(encode_move(to-16, to, DOUBLE_PAWN_PUSH));
        }
        while(left_capture) {
            int to = __builtin_ctzll(left_capture);
            left_capture &= (left_capture - 1);
            int flag = (to == board.en_passent)? EN_PASSANT : CAPTURE;
            if(to >= 56) {
                flag |= 8;
                for(int i = 0; i < 4; i++) move_list.add(encode_move(to-7, to, flag+i));
            }
            else move_list.add(encode_move(to-7, to, flag));
        }
        while(right_capture) {
            int to = __builtin_ctzll(right_capture);
            right_capture &= (right_capture - 1);
            int flag = (to == board.en_passent)? EN_PASSANT : CAPTURE;
            if(to >= 56) {
                flag |= 8;
                for(int i = 0; i < 4; i++) move_list.add(encode_move(to-9, to, flag+i));
            }
            else move_list.add(encode_move(to-9, to, flag));
        }
    }

    else {
        Bitboard single_push = (pawn >> 8) & ~board.common;
        Bitboard double_push = (single_push >> 8) & table.rank_mask[RANK_5] & ~board.common;
        Bitboard targets = board.by_color[WHITE] | ((board.en_passent == NO_SQ)? 0 : (1ULL << board.en_passent));
        Bitboard left_capture = (pawn >> 7) & targets & ~table.file_mask[FILE_A];
        Bitboard right_capture = (pawn >> 9) & targets & ~table.file_mask[FILE_H];
        
        while(single_push) {
            int to = __builtin_ctzll(single_push);
            single_push &= (single_push - 1);
            if(to < 8) {
                int flag = 8;
                for(int i = 0; i < 4; i++) move_list.add(encode_move(to+8, to, flag+i));
            }
            else move_list.add(encode_move(to+8, to, QUIET_MOVE));
        }
        while(double_push) {
            int to = __builtin_ctzll(double_push);
            double_push &= (double_push - 1);
            move_list.add(encode_move(to+16, to, DOUBLE_PAWN_PUSH));
        }
        while(left_capture) {
            int to = __builtin_ctzll(left_capture);
            left_capture &= (left_capture - 1);
            int flag = (to == board.en_passent)? EN_PASSANT : CAPTURE;
            if(to < 8) {
                flag |= 8;
                for(int i = 0; i < 4; i++) move_list.add(encode_move(to+7, to, flag+i));
            }
            else move_list.add(encode_move(to+7, to, flag));
        }
        while(right_capture) {
            int to = __builtin_ctzll(right_capture);
            right_capture &= (right_capture - 1);
            int flag = (to == board.en_passent)? EN_PASSANT : CAPTURE;
            if(to < 8) {
                flag |= 8;
                for(int i = 0; i < 4; i++) move_list.add(encode_move(to+9, to, flag+i));
            }
            else move_list.add(encode_move(to+9, to, flag));
        }
    }
}

MoveList gen_pseudo_legal_moves() {
    MoveList move_list;

    // Rooks
    gen_rook_moves(move_list, board.active_color);

    // Bishops
    gen_bishop_moves(move_list, board.active_color);

    // Queens
    gen_queen_moves(move_list, board.active_color);

    // Knights
    gen_knight_moves(move_list, board.active_color);

    // King
    gen_king_moves(move_list, board.active_color);

    // Pawns
    gen_pawn_moves(move_list, board.active_color);

    return move_list;
}