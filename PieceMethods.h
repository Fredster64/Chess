#ifndef PIECEMETHODS_H
#define PIECEMETHODS_H

#include <iostream>
#include <vector>
#include <string>

namespace chess {
    
    Piece :: Piece (const bool piece_colour, const pos coordinates, uint8_t& status_bits, uint8_t**& gb) {
        is_white = piece_colour;
        position = coordinates;
        pgs = &status_bits; // copy the address of status_bits to a pointer.
        pgb = &gb; // copy the address of the game board to a pointer.
    }
    
    Piece :: ~Piece (void) {
        delete pgb;
        delete pgs;
    };
    
    void Piece :: print_info (void) {
        std::cout << (is_white ? "White " : "Black ") << this->get_type() << " at " << static_cast<char>(position.x + 'A') << static_cast<int>(position.y + 1) << std::endl;
    }
    
    bool Piece :: move (const pos p) {
        // the default function for moving. Exceptions only for p and K.
        uint8_t** b = *pgb;
        bool valid = false;
        for (const auto& m : valid_moves) {
            if (m.x == p.x and m.y == p.y) {
                valid = true;
            }
        }
        if (valid) {
            uint8_t temp = b[position.x][position.y];
            b[position.x][position.y] = 0;
            position = p;
            b[position.x][position.y] = temp;
        }
        return valid;
    }
}

#endif
