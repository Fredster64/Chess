#ifndef DERIVEDPIECEMETHODS_H
#define DERIVEDPIECEMETHODS_H

#include <iostream>
#include <vector>

#include "ChessClasses.h" // for Class Definitions
#include "PieceMethods.h" // Not sure if necessary but just in case (polymorphism)

namespace chess {
    
    uint8_t Pawn :: move (Pos p) {
        // if first move occurs, it must be set to false.
        uint8_t** b = *pgb;
        Pos p_now = this->get_pos();
        uint8_t valid = 0;
        for (const auto& m : valid_moves) {
            if (m == p) {
                valid = 1;
                break;
            }
        }
        if (valid == 1) {
            this->is_first_move (false);
            uint8_t temp = b[p_now.x][p_now.y];
            b[p_now.x][p_now.y] = 0;
            if (p.y == 0 or p.y == 7) {
                valid |= promotion ();
            } else {
                this->update_pos (p_now);
                b[p.x][p.y] = temp;
            }
           // 'Check' check 
           if ( in_check( is_white, b ) ) {
              // Revert back to pre-move board position 
              temp = b[p.x][p.y];
              b[p.x][p.y] = 0;
              mci.position = p.now; 
              b[p_now.x][p_now.y] = temp;
              // Let the player know what happened 
              std::cout << "This move is invalid - it would put you in check." << endl;
              // Move was invalid 
              valid = 0;
           }
        }
        return valid;
    }

    uint8_t King :: move (Pos p) {
        // an exception case is made for the castling move (not yet implemented).
        uint8_t** b = *pgb;
        Pos p_now = this->get_pos();
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
            this->update_pos (p_now);
            b[p.x][p.y] = temp;
        }
        // 'Check' check
       if ( in_check( is_white, b ) ) {
            // Revert back to pre-move board position 
            temp = b[p.x][p.y];
            b[p.x][p.y] = 0;
            mci.position = p.now; 
            b[p_now.x][p_now.y] = temp;
            // Let the player know what happened 
            std::cout << "This move is invalid - it would put you in check." << endl;
            // Move was invalid 
            valid = 0;
        }
        return valid;
    }
    
    uint8_t Pawn :: promotion (void) {
        char c;
        do {
            std::cout << "What would you like to promote the pawn to? (N, B, R, Q)" << std::endl;
            std::cin >> c;
            if (!std::cin) {
                /* If the input was invalid: */
                std::cout << "Something went wrong. Please try again." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                c = ' ';
            } else if (c != 'N' and c != 'B' and c != 'R' and c != 'Q') {
                std::cout << "That is not a valid piece. Please try again." << std::endl;
            } else switch (c) {
                case 'N': {
                    std::cout << "You have chosen to promote to a Knight!" << std::endl;
                    return 0x02;
                }
                case 'B': {
                    std::cout << "You have chosen to promote to a Bishop!" << std::endl;
                    return 0x04;
                }
                case 'R': {
                    std::cout << "You have chosen to promote to a Rook!" << std::endl;
                    return 0x08;
                }
                case 'Q': {
                    std::cout << "You have chosen to promote to a Queen!" << std::endl;
                    return 0x10;
                }
            }
        } while (c != 'N' and c != 'B' and c != 'R' and c != 'Q');
        /* Unreachable code, added to silence to compiler. */
        return 1;
    }
}

#endif
