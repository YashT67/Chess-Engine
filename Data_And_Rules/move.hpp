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

inline int get_move_source(Move m) { 
    return m & 0x3F;
}

inline int get_move_target(Move m) { 
    return (m >> 6) & 0x3F;
}

inline int get_move_flag(Move m) { 
    return (m >> 12) & 0x0F;
}

MoveList gen_legal_moves();

