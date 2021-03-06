#ifndef DERIVEDPIECEMETHODS_H
#define DERIVEDPIECEMETHODS_H

#include <iostream>
#include <vector>

#include "ChessClasses.h" // for Class Definitions
#include "PieceMethods.h" // Not sure if necessary but just in case (polymorphism)

namespace chess {
    
    uint8_t Pawn :: move (Pos p_to) {
        // if first move occurs, it must be set to false.
        uint8_t** b = this->get_board ();
        Pos p_from = this->get_pos ();
        p_from.print_pos ();
        p_to.print_pos ();
        uint8_t valid = 0;
        for (const auto& m : valid_moves) {
            if (m == p_to) {
                valid = 1;
                break;
            }
        }
        if (valid == 1) {
            this->is_first_move (false);
            this->update_last_move(p_from, p_to, "Pawn");
            // en-passant modification
            if ((p_from.x != p_to.x) and (b[p_to.x][p_to.y] == 0)) { // if it moves diagonally
                // remove the piece directly to the side and move diagonally.
                valid |= 0x80;
                Pos p_dlt;
                if (is_white) p_dlt = {p_to.x, 4};
                else p_dlt = {p_to.x, 3};
                b[p_dlt.x][p_dlt.y] = 0;
            }
            
            uint8_t temp = b[p_from.x][p_from.y];
            b[p_from.x][p_from.y] = 0;
            // promotion modification
            if (p_to.y == 0 or p_to.y == 7) {
                valid |= promotion ();
            } else {
                this->update_pos (p_to);
                b[p_to.x][p_to.y] = temp;
            }
        }
        return valid;
    }

    uint8_t King :: move (Pos p_to) {
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
            this->is_first_move (false);
            this->update_last_move (p_from, p_to, "King");
            uint8_t temp = b[p_from.x][p_from.y];
            b[p_from.x][p_from.y] = 0;
            this->update_pos (p_to);
            b[p_to.x][p_to.y] = temp;
            // castling modification
            if ((p_from.x - p_to.x) == 2) { // QS-Castle
                valid |= 0x20;
            } else if ((p_from.x - p_to.x) == -2) { // KS-Castle
                valid |= 0x40;
            }
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
