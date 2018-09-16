#ifndef PIECEMETHODS_H
#define PIECEMETHODS_H

#include <iostream>
#include <vector>
#include <string>

#include "ChessClasses.h" // for Class Definitions

namespace chess {
    
    Piece :: Piece (const bool piece_colour, const Pos coordinates, LM& last_move, uint8_t**& gb) {
        is_white = piece_colour;
        mci.position = coordinates;
        mci.first_move = true;
        mci.lm_ptr = &last_move;
        mci.pgb = &gb; // copy the address of the game board to the MCI.
    }
    
    Piece :: ~Piece (void) {
        valid_moves.clear ();
        mci.pgb = nullptr;
        mci.lm_ptr = nullptr;
    };
    
    void Piece :: print_info (void) {
        std::cout << (is_white ? "White " : "Black ") << this->get_type () << " at " << static_cast<char>(mci.position.x + 'A') << static_cast<int>(mci.position.y + 1) << std::endl;
    }
    
    void Piece :: check_moves (std::vector<Pos>& v, bool t) { // Calls the check_moves function in the interface for this piece
        mci.move_checker (v, this->get_type(), is_white, t);
    }
    
    uint8_t Piece :: move (const Pos p_to) {
        // the default function for moving. Exceptions only for p and K.
        uint8_t** b = this->get_board ();
        Pos p_from = this->get_pos ();
        uint8_t valid = 0;
        for (const auto& m : valid_moves) {
            if (m == p_to) {
                valid = 1;
                break;
            }
        }
        if (valid == 1) {
            this->update_last_move(p_from, p_to, get_type ());
            uint8_t temp = b[p_from.x][p_from.y];
            b[p_from.x][p_from.y] = 0;
            this->update_pos (p_to);
            b[p_to.x][p_to.y] = temp;
        }
        return valid;
    }
}

#endif
