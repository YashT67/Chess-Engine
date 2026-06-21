#pragma once
#include "../constants.hpp"

struct MoveList {
    Move moves[256];
    int count=0;

    inline void add(Move m) {
        moves[count++] = m;
    }
} ;

inline Move encode_move(int from, int to, int flag) {
    return from | (to << 6) | (flag << 12);
}

inline int get_from(Move m) { 
    return m & 0x3F;
}

inline int get_to(Move m) { 
    return (m >> 6) & 0x3F;
}

inline int get_flag(Move m) { 
    return (m >> 12) & 0x0F;
}

inline int get_promotion(Move m) {
    return ((m >> 12) & 8)? (m >> 12) & 3 : -1; 
}

MoveList gen_pseudo_legal_moves(struct Board &board);

char make_move(struct Board &board, Move move);
void unmake_move(struct Board &board, Move move, char captured);
bool king_is_attacked(struct Board &board);
