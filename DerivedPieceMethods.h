#ifndef DERIVEDPIECEMETHODS_H
#define DERIVEDPIECEMETHODS_H

#include <iostream>
#include <vector>

#include "ChessClasses.h" // for Class Definitions
#include "PieceMethods.h" // Not sure if necessary but just in case (polymorphism)

namespace chess {
    
    uint8_t Pawn :: move (pos p) {
        // if first move occurs, it must be set to false.
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
            mci.first_move = false;
            uint8_t temp = b[p_now.x][p_now.y];
            b[p_now.x][p_now.y] = 0;
            if (p.y == 0 or p.y == 7) {
                valid |= promotion ();
            } else {
                mci.position = p;
                b[p.x][p.y] = temp;
            }
        }
        return valid;
    }

    uint8_t King :: move (pos p) {
        // an exception case is made for the castling move (not yet implemented).
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
