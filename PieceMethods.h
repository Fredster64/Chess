#ifndef PIECEMETHODS_H
#define PIECEMETHODS_H

#include <iostream>
#include <vector>
#include <string>

#include "ChessClasses.h" // for Class Definitions

namespace chess {
    
    Piece :: Piece (const bool piece_colour, const pos coordinates, uint8_t& status_bits, uint8_t**& gb) {
        is_white = piece_colour;
        position = coordinates;
        pgs = &status_bits; // copy the address of status_bits to a pointer.
        pgb = &gb; // copy the address of the game board to a pointer.
    }
    
    Piece :: ~Piece (void) {
        valid_moves.clear();
        pgs = nullptr;
        pgb = nullptr;
    };
    
    void Piece :: print_info (void) {
        std::cout << (is_white ? "White " : "Black ") << this->get_type() << " at " << static_cast<char>(position.x + 'A') << static_cast<int>(position.y + 1) << std::endl;
    }
    
    void Piece::check_moves( std::vector<pos>& p, bool t ) { // Calls the check_moves function in the interface for this piece
        std::string type = this->get_type();
        moveChecker.check_moves( p, t, type, position, is_white );
    }
    
    uint8_t Piece :: move (const pos p) {
        // the default function for moving. Exceptions only for p and K.
        uint8_t** b = *pgb;
        uint8_t valid = 0;
        for (const auto& m : valid_moves) {
            if (m == p) {
                valid = 1;
                break;
            }
        }
        if (valid == 1) {
            uint8_t temp = b[position.x][position.y];
            b[position.x][position.y] = 0;
            position = p;
            b[position.x][position.y] = temp;
        }
        return valid;
    }
    
    void Piece :: pb_inc (pos p, std::vector<pos>& v, pos inc, bool t) {
        uint8_t** b = *pgb; // put the game board array in the current scope
        uint8_t comp = is_white ? 0x40 : 0x80;
        while (p.x >= 0 and p.y >= 0 and p.x < 8 and p.y < 8) {
            if ((b[p.x][p.y] & 0xC0) == comp) {
                if (!t) v.push_back({p.x, p.y});
                break;
            } else if (b[p.x][p.y] > 0) {
                v.push_back({p.x, p.y});
                break;
            } else {
                v.push_back({p.x, p.y});
                p+=inc;
            }
        }
    }
}

#endif
