#include <cstdint>
#include <iostream>
#include <string>
#include "constants.hpp"
#include "board.hpp"
#include "table.hpp"

Lookup_Table table;
Board board;

int main() {
    std::string input = "";
    bool color;
    std::cout << "Input the preferred position: white, black or random\n";
    while(getline(std::cin, input)) {
        if(input == "white") color = 0;
        else if(input == "black") color = 1;
        else if(input == "random") color = rand() & 1;
        else {
            std::cout << "Error. Pls input white, black or random\n";
            continue;
        }
        break;
    }

    if(color == WHITE) std::cout << "Color = white\n";
    else std::cout << "Color chosen = black\n";
    board.player_color = color;
}