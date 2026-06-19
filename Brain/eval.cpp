#include <algorithm>
#include "eval.hpp"
#include "../Data_And_Rules/board.hpp"

// Returns the evaluated score using PSTO and tapered eval
int evaluate(const struct Board &board) {
    int mg_score = board.mg_material[WHITE] - board.mg_material[BLACK];
    int eg_score = board.eg_material[WHITE] - board.eg_material[BLACK];
    int phase = std::min(board.game_phase, 24);

    for(int i = 0; i < 6; i++) {
        Bitboard white_pieces = board.pieces[WHITE][i];
        while(white_pieces) {
            int square = __builtin_ctzll(white_pieces);
            white_pieces &= (white_pieces - 1);
            mg_score += mg_table[i][square];
            eg_score += eg_table[i][square];
        }

        Bitboard black_pieces = board.pieces[BLACK][i];
        while(black_pieces) {
            int square = __builtin_ctzll(black_pieces) ^ 56;
            black_pieces &= (black_pieces - 1);
            mg_score -= mg_table[i][square];
            eg_score -= eg_table[i][square];
        }
    }

    int score = ((mg_score * phase) + (eg_score * (24 - phase))) / 24;
    return (board.active_color == WHITE)? score : -score;
}