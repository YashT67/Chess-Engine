#pragma once
#include "../constants.hpp"

struct Board {
    char piece_on_board[BOARD_SIZE]; // 8 * 8 board representation

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
    int game_phase;

    // Material values left on board
    int mg_material[2];
    int eg_material[2];

    Board();

    void generate_fen();
    void print_board();
    
} ;

const int phase_piece_values[6] = { 4, 2, 1, 1, 0, 0};

extern Board board;