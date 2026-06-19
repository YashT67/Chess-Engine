#pragma once
#include <cstdint>

// Transparent sheet of board packed in 64 bits
typedef uint64_t Bitboard;

// Bits 0-5: Source Square (0-63)
// Bits 6-11: Target Square (0-63)
// Bits 12-15: Flags (Promotion, Castling, En Passant, etc.)
typedef uint16_t Move;

enum Color : bool {WHITE, BLACK};
enum Piece {QUEEN, ROOK, BISHOP, KNIGHT, PAWN, KING};
enum Square {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8
};
enum MoveFlag : uint16_t {
    // Basic Moves
    QUIET_MOVE = 0,
    DOUBLE_PAWN_PUSH = 1,
    KING_CASTLE = 2,
    QUEEN_CASTLE = 3,
    
    // Captures (Notice how the 4-bit is turned on)
    CAPTURE = 4,
    EN_PASSANT = 5,
    // 6 and 7 are reserved/unused
    
    // Promotions (Notice how the 8-bit is turned on)
    PROMO_QUEEN = 8,
    PROMO_ROOK = 9,
    PROMO_BISHOP = 10,
    PROMO_KNIGHT = 11,
    
    // Promotion Captures (Promotion Flag + CAPTURE Flag)
    // Example: PROMO_QUEEN (8) + CAPTURE (4) = 12
    PROMO_QUEEN_CAPTURE = 12,
    PROMO_ROOK_CAPTURE = 13,
    PROMO_BISHOP_CAPTURE = 14,
    PROMO_KNIGHT_CAPTURE = 15
};
enum File {
    FILE_A, FILE_B, FILE_C, FILE_D, 
    FILE_E, FILE_F, FILE_G, FILE_H
};
enum Rank {
    RANK_1, RANK_2, RANK_3, RANK_4, 
    RANK_5, RANK_6, RANK_7, RANK_8
};

constexpr int BOARD_SIZE = 64;
inline int NO_SQ = 64;

const int piece_values[6] = { 9, 5, 3, 3, 1, 100};