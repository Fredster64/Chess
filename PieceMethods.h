#ifndef PIECEMETHODS_H
#define PIECEMETHODS_H

#include <iostream>
#include <vector>
#include <string>

#include "ChessClasses.h" // for Class Definitions
#include "ChessIncludes.h" // for in_check

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
        mci.move_checker (v, this->get_type (), is_white, t);
    }
    
    // Resets the board to pre-move position if the move leaves the player in check
    uint8_t Piece :: if_in_check (Pos p_before, Pos p_after) {
        uint8_t** b = this->get_board ();
        
        /** Added as dummy **/
        return 1;
        
//        if (in_check (is_white)) return 1;
//        else {
//            temp = b[p_after.x][p_after.y];
//            b[p_after.x][p_after.y] = 0;
//            mci.position = p_before;
//            b[p_before.x][p_before.y] = temp;
//            // Let the player know what happened
//            std::cout << "This move is invalid - it would put you in check." << std::endl;
//            // Move was invalid
//            return 0;
//        }
    }
    
    uint8_t Piece :: move (Pos p_to, GameEngine& game) {
        // the default function for moving. Exceptions only for p and K.
        ge = &game;
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
            // Reset and invalidate move if it leaves the player in check
            valid |= if_in_check (p_from, p_to);
        }
        return valid;
    }
}

#endif
