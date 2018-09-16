#ifndef PIECEMETHODS_H
#define PIECEMETHODS_H

#include <iostream>
#include <vector>
#include <string>

#include "ChessClasses.h" // for Class Definitions

namespace chess {
    
    Piece :: Piece (const bool piece_colour, const Pos coordinates, /*uint8_t& status_bits,*/ uint8_t**& gb) {
        is_white = piece_colour;
        mci.position = coordinates;
        mci.first_move = true;
        pgb = &gb; // copy the address of the game board to a pointer.
        mci.gb = &gb; // copy the address of the game board to the MCI.
    }
    
    Piece :: ~Piece (void) {
        valid_moves.clear();
        pgb = nullptr;
    };
    
    void Piece :: print_info (void) {
        std::cout << (is_white ? "White " : "Black ") << this->get_type() << " at " << static_cast<char>(mci.position.x + 'A') << static_cast<int>(mci.position.y + 1) << std::endl;
    }
    
    void Piece :: check_moves (std::vector<Pos>& v, bool t) { // Calls the check_moves function in the interface for this piece
        mci.move_checker (v, this->get_type(), is_white, t);
    }
    
    uint8_t Piece :: move (const Pos p_to) {
        // the default function for moving. Exceptions only for p and K.
        uint8_t** b = *pgb;
        Pos p_from = this->get_pos ();
        uint8_t valid = 0;
        for (const auto& m : valid_moves) {
            if (m == p_to) {
                valid = 1;
                break;
            }
        }
        if (valid == 1) {
            uint8_t temp = b[p_from.x][p_from.y];
            b[p_from.x][p_from.y] = 0;
            this->update_pos (p_to);
            b[p_to.x][p_to.y] = temp;
        }
        return valid;
    }
}

#endif
