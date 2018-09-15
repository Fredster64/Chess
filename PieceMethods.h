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
    
    uint8_t Piece :: move (const Pos p) {
        // the default function for moving. Exceptions only for p and K.
        uint8_t** b = *pgb;
        Pos p_now = mci.position;
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
            /* We can implement 'check' here: 
              -- Keep a copy of p_now 
              -- Perform a 'check' test (method in GameEngine I think, could move somewhere else to make it easily accessible)
              -- If we're in check, revert back to pre-move board position (i.e. swap p and p_now back)
              -- Print some kind of "you're in check!" message, and try again
              -- Set valid = 0 so that the GameEngine thinks we've made an invalid move */
        }
        return valid;
    }
}

#endif
