#pragma once
#include "constants.hpp"

struct Board {
    char piece_on_board[TILES]; // 8 * 8 board representation

    // Bitboard representation
    Bitboard pieces[2][6];
    Bitboard by_color[2];
    Bitboard common;

    // Common terms needed, checkout board.cpp file for more explainations
    bool player_color;
    bool active_color;
    bool kingside_castling[2];
    bool queenside_castling[2];
    int en_passent;
    int halfmove_clock;
    int fullmove_clock;

    Board();

    void generate_fen();
    void print_board();
    
} ;

extern Board board;