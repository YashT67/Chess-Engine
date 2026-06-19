#include "board.hpp"
#include "table.hpp"
#include <iostream>
#include <cctype>

Board::Board() {
    // Pieces of Board / Board representation
    piece_on_board[0] = 'R', piece_on_board[1] = 'N', piece_on_board[2] = 'B', piece_on_board[3] = 'Q';
    piece_on_board[4] = 'K', piece_on_board[5] = 'B', piece_on_board[6] = 'N', piece_on_board[7] = 'R';
    for(int i = 8; i < 16; i++) piece_on_board[i] = 'P';
    for(int i = 16; i < 48; i++) piece_on_board[i] = '.';
    for(int i = 48; i < 56; i++) piece_on_board[i] = 'p';
    piece_on_board[56] = 'r', piece_on_board[57] = 'n', piece_on_board[58] = 'b', piece_on_board[59] = 'q';
    piece_on_board[60] = 'k', piece_on_board[61] = 'b', piece_on_board[62] = 'n', piece_on_board[63] = 'r';

    // White Pieces
    pieces[WHITE][PAWN] = (255ULL << 8);
    pieces[WHITE][KING] = (1ULL << 4);
    pieces[WHITE][QUEEN] = (1ULL << 3);
    pieces[WHITE][BISHOP] = (1ULL << 2) | (1ULL << 5);
    pieces[WHITE][KNIGHT] = (1ULL << 1) | (1ULL << 6);
    pieces[WHITE][ROOK] = 1 | (1ULL << 7);

    // Black Pieces
    pieces[BLACK][PAWN] = (255ULL << 48);
    pieces[BLACK][KING] = (1ULL << 60);
    pieces[BLACK][QUEEN] = (1ULL << 59);
    pieces[BLACK][BISHOP] = (1ULL << 58) | (1ULL << 61);
    pieces[BLACK][KNIGHT] = (1ULL << 57) | (1ULL << 62);
    pieces[BLACK][ROOK] = (1ULL << 63) | (1ULL << 56);

    // Common board pieces by color
    by_color[0] = 0, by_color[1] = 0;
    for(int i = 0; i < 6; i++) {
        by_color[WHITE] |= pieces[WHITE][i];
        by_color[BLACK] |= pieces[BLACK][i];
    }
    common = by_color[WHITE] | by_color[BLACK];

    // Castling
    kingside_castling[0] = true, kingside_castling[1] = true;
    queenside_castling[0] = true, queenside_castling[1] = true;

    // En passent square i.e. the square behind the pawn that just moved 2 squares
    en_passent = NO_SQ;

    // A counter tracking half-moves since the last pawn advance or capture, used for the fifty-move draw rule
    halfmove_clock = 0;

    // The current move number starting at 1 and incrementing after Black's move
    fullmove_clock = 1;

    // Active Color
    active_color = WHITE;
}

void Board::print_board() {
    if(player_color == WHITE) {
        for(int i = 7; i >= 0; i--) {
            std::cout << i+1 << "  | ";
            for(int j = 0; j < 8; j++) {
                std::cout << piece_on_board[i*8+j] << ' ';
            }
            std::cout << '\n';
        }
        std::cout << "   ------------------\n";
        std::cout << "     a b c d e f g h \n";
    }
    else {
        for(int i = 0; i < 8; i++) {
            std::cout << i+1 << "  | ";
            for(int j = 7; j >= 0; j--) {
                std::cout << piece_on_board[i*8+j] << ' ';
            }
            std::cout << '\n';
        }
        std::cout << "   ------------------\n";
        std::cout << "     h g f e d c b a \n";
    }
}

void Board::generate_fen() {
    // Piece Placement
    for(int i = 7; i >= 0; i--) {
        int curr = 0;
        for(int j = 0; j < 8; j++) {
            int index = i*8+j;
            if(piece_on_board[index] != '.') {
                if(curr) {
                    std::cout << curr;
                    curr=0;
                }
                std::cout << piece_on_board[index];
            }
            else curr++;
        }
        if(curr) std::cout << curr;
        if(i > 0) std::cout << '/';
    }
    std::cout << ' ';

    // Active Color
    if(active_color == WHITE) std::cout << "w ";
    else std::cout << "b ";

    // Castling
    bool has_castling = false;
    if(kingside_castling[WHITE]) {
        std::cout << 'K';
        has_castling = true;
    }
    if(queenside_castling[WHITE]) {
        std::cout << 'Q';
        has_castling = true;
    }
    if(kingside_castling[BLACK]) {
        std::cout << 'k';
        has_castling = true;
    }
    if(queenside_castling[BLACK]) {
        std::cout << 'q';
        has_castling = true;
    }
    if(!has_castling) std::cout << '-';
    std::cout << ' ';

    // En Passent
    if(en_passent < 64) {
        char rank = '1' + en_passent/8;
        char file = 'a' + en_passent%8;
        std::cout << file << rank << ' ';
    }
    else std::cout << "- ";

    // Halfmove and Fullmove
    std::cout << halfmove_clock << ' ';
    std::cout << fullmove_clock << '\n';
}

