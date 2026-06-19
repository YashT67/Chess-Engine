#include "table.hpp"

// Returns the Bitboard of blockers for a particular index
static Bitboard set_occupancy(int index, int bits_in_mask, Bitboard mask) {
    Bitboard occupancy = 0ULL;
    for(int i = 0; i < bits_in_mask; i++) {
        int square = __builtin_ctzll(mask);
        mask ^= (1ULL << square);
        if(index & (1 << i)) {
            occupancy |= (1ULL << square);
        }
    }
    return occupancy;
}

// Returns a bitboard of rook vision / possible rook moves based on the square and the blockers present
static Bitboard calculate_rook_vision(int square, Bitboard blockers) {
    Bitboard vision = 0ULL;
    for(int i = square-1; i >= 0 && i%8 != 7; i--) {
        vision |= (1ULL << i);
        if(blockers & (1ULL << i)) break;
    }
    for(int i = square+1; i < 64 && i%8 != 0; i++) {
        vision |= (1ULL << i);
        if(blockers & (1ULL << i)) break;
    }
    for(int i = square-8; i >= 0; i -= 8) {
        vision |= (1ULL << i);
        if(blockers & (1ULL << i)) break;
    }
    for(int i = square+8; i < 64; i += 8) {
        vision |= (1ULL << i);
        if(blockers & (1ULL << i)) break;
    }
    return vision;
}

// Returns a bitboard of bishop vision / possible bishop moves based on the square and the blockers present
static Bitboard calculate_bishop_vision(int square, Bitboard blockers) {
    Bitboard vision = 0ULL;
    for(int i = square+9; i%8 != 0 && i < 64; i += 9) {
        vision |= (1ULL << i);
        if(blockers & (1ULL << i)) break;
    }
    for(int i = square-9; i%8 != 7 && i >= 0; i -= 9) {
        vision |= (1ULL << i);
        if(blockers & (1ULL << i)) break;
    }
    for(int i = square+7; i%8 != 7 && i < 64; i += 7) {
        vision |= (1ULL << i);
        if(blockers & (1ULL << i)) break;
    }
    for(int i = square-7; i%8 != 0 && i >= 0; i -= 7) {
        vision |= (1ULL << i);
        if(blockers & (1ULL << i)) break;
    }
    return vision;
}

void Lookup_Table::init_masks() {
    // Rook Mask
    for(int i = 0; i < TILES; i++) {
        rook_mask[i] = 0ULL;
        for(int j = i; j%8 > 0; j--) rook_mask[i] |= (1ULL << j);
        for(int j = i; j%8 < 7; j++) rook_mask[i] |= (1ULL << j);
        for(int j = i-8; j > 7; j -= 8) rook_mask[i] |= (1ULL << j);
        for(int j = i+8; j < 56; j += 8) rook_mask[i] |= (1ULL << j);
        rook_mask[i] ^= (1ULL << i);
    }

    // Bishop Mask
    for(int i = 0; i < TILES; i++) {
        bishop_mask[i] = 0ULL;
        for(int j = i; j%8 < 7 && j < 56; j += 9) bishop_mask[i] |= (1ULL << j);
        for(int j = i; j%8 > 0 && j > 7; j -= 9) bishop_mask[i] |= (1ULL << j);
        for(int j = i; j%8 > 0 && j < 56; j += 7) bishop_mask[i] |= (1ULL << j);
        for(int j = i; j%8 < 7 && j > 7; j -= 7) bishop_mask[i] |= (1ULL << j);
        bishop_mask[i] ^= (1ULL << i);
    }

    // File and Rank masks
    for(int i = 0; i < 8; i++) {
        file_mask[i] = 0ULL;
        rank_mask[i] = 0ULL;
    }
    for(int rank = 0; rank < 8; rank++) {
        for(int file = 0; file < 8; file++) {
            int square = 8 * rank + file;
            file_mask[rank] |= (1ULL << square);
            rank_mask[file] |= (1ULL << square);
        }
    }
}

void Lookup_Table::init_tables() {
    // Rook Table
    for(int i = 0; i < TILES; i++) {
        int relevant_bits = rook_relevant_bits[i];
        int count = (1 << relevant_bits);
        for(int j = 0; j < count; j++) {
            Bitboard blockers = set_occupancy(j, relevant_bits, rook_mask[i]);
            Bitboard vision = calculate_rook_vision(i, blockers);
            int magic_index = (blockers * rook_magic[i]) >> (64-relevant_bits);
            rook_table[i][magic_index] = vision;
        }
    }

    // Bishop Table
    for(int i = 0; i < TILES; i++) {
        int relevant_bits = bishop_relevant_bits[i];
        int count = (1 << relevant_bits);
        for(int j = 0; j < count; j++) {
            Bitboard blockers = set_occupancy(j, relevant_bits, bishop_mask[i]);
            Bitboard vision = calculate_bishop_vision(i, blockers);
            int magic_index = (blockers * bishop_magic[i]) >> (64-relevant_bits);
            bishop_table[i][magic_index] = vision;
        }
    }

    // King Table
    for(int i = 0; i < TILES; i++) {
        Bitboard vision = 0ULL;
        Bitboard king = (1ULL << i);
        int file = i%8;
        if(file > 0) {
            vision |= (king >> 1);
            vision |= (king << 7);
            vision |= (king >> 9);
        }
        if(file < 7) {
            vision |= (king << 1);
            vision |= (king >> 7);
            vision |= (king << 9);
        }
        vision |= (king << 8);
        vision |= (king >> 8);
        king_table[i] = vision;
    }

    // Knight Table
    for(int i = 0; i < TILES; i++) {
        Bitboard vision = 0ULL;
        Bitboard knight = (1ULL << i);
        int file = i%8;
        if(file > 0) {
            vision |= (knight << 15);
            vision |= (knight >> 17);
        }
        if(file > 1) {
            vision |= (knight << 6);
            vision |= (knight >> 10);
        }
        if(file < 7) {
            vision |= (knight >> 15);
            vision |= (knight << 17);
        }
        if(file < 6) {
            vision |= (knight >> 6);
            vision |= (knight << 10);
        }
        knight_table[i] = vision;
    }

    // White Pawn Table for pawn attacks
    for(int i = 0; i < TILES; i++) {
        Bitboard attack = 0ULL;
        Bitboard pawn = (1ULL << i);
        int file = i%8;
        if(file > 0) attack |= (pawn << 7);
        if(file < 7) attack |= (pawn << 9);
        white_pawn_table[i] = attack;
    }

    // Black Pawn Table for pawn attacks
    for(int i = 0; i < TILES; i++) {
        Bitboard attack = 0ULL;
        Bitboard pawn = (1ULL << i);
        int file = i%8;
        if(file > 0) attack |= (pawn >> 9);
        if(file < 7) attack |= (pawn >> 7);
        black_pawn_table[i] = attack;
    }
}

Lookup_Table::Lookup_Table() {
    init_masks();
    init_tables();
}