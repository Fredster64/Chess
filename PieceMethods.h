#ifndef PIECEMETHODS_H
#define PIECEMETHODS_H

#include <iostream>
#include <vector>
#include <string>

#include "ChessClasses.h" // for Class Definitions

namespace chess {
    
    Piece :: Piece (const bool piece_colour, const pos coordinates, uint8_t& status_bits, uint8_t**& gb) {
        is_white = piece_colour;
        mci.position = coordinates;
        mci.first_move = true;
        pgs = &status_bits; // copy the address of status_bits to a pointer.
        pgb = &gb; // copy the address of the game board to a pointer.
        mci.gb = &gb; // copy the address of the game board to the MCI.
    }
    
    Piece :: ~Piece (void) {
        valid_moves.clear();
        pgs = nullptr;
        pgb = nullptr;
    };
    
    void Piece :: print_info (void) {
        std::cout << (is_white ? "White " : "Black ") << this->get_type() << " at " << static_cast<char>(mci.position.x + 'A') << static_cast<int>(mci.position.y + 1) << std::endl;
    }
    
    /* For review */
    void Piece :: check_moves (std::vector<pos>& v, bool t) { // Calls the check_moves function in the interface for this piece
        mci.check_moves (v, this->get_type(), is_white, t);
    }
    
    uint8_t Piece :: move (const pos p) {
        // the default function for moving. Exceptions only for p and K.
        uint8_t** b = *pgb;
        pos p_now = mci.position;
        uint8_t valid = 0;
        for (const auto& m : valid_moves) {
            if (m == p) {
                valid = 1;
                break;
            }
        }
        if (valid == 1) {
            uint8_t temp = b[p_now.x][p_now.y];
            b[p_now.x][p_now.y] = 0;
            mci.position = p;
            b[p.x][p.y] = temp;
        }
        return valid;
    }
}

#endif
