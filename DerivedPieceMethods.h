#ifndef DERIVEDPIECEMETHODS_H
#define DERIVEDPIECEMETHODS_H

#include <iostream>
#include <vector>

#include "ChessClasses.h" // for Class Definitions
#include "PieceMethods.h" // Not sure if necessary but just in case (polymorphism)

namespace chess {
    
    void Pawn :: check_moves (std::vector<pos>& v, bool t) {
        int8_t posx = position.x;
        int8_t posy = position.y;
        uint8_t** b = *pgb; // put the game board array in the current scope
        
        // will work on 'check' case conditions and en-passant rules later.
        is_white ? ++posy : --posy;
        
        // 0xC0 : Either a black piece or a white piece present
        
        // 0x20 : A King is present
        
        // If t=true (default) : If the piece is white, then the comparison is for black pieces and v.v. If a oppositely coloured piece is in an upper diagonal square then the piece can be taken.
        
        // If t=false ("check" case): evaluate only upper diagonal squares.
        
        uint8_t comp = is_white ? 0x80 : 0x40;
        if (posx < 7) {
            if (((b[posx + 1][posy] & 0xC0) == comp) or (!t)) {
                v.push_back({(++posx)--, posy});
            }
        }
        if (posx > 0) {
            if (((b[posx - 1][posy] & 0xC0) == comp) or (!t)) {
                v.push_back({(--posx)++, posy});
            }
        }
        if (t) {
            if (b[posx][posy] == 0 and posy != 0 and posy != 7) {
                v.push_back({posx, posy});
            }
            if (first_move) {
                if (b[posx][posy] == 0) {
                    is_white ? ++posy : --posy;
                    if (b[posx][posy] == 0) {
                        valid_moves.push_back({posx, posy});
                    }
                }
            }
        }
    }
    
    void Knight :: check_moves (std::vector<pos>& v, bool t) {
        const pos p = position;
        pos L[4] = {{2, 1}, {-2, 1}, {1, 2}, {-1, 2}};
        uint8_t** b = *pgb; // put the game board array in the current scope
        uint8_t comp = is_white ? 0x40 : 0x80;
        
        // L-shapes: -2x-1y ; -2x+1y ; -1x-2y ; -1x+2y ; +1x-2y ; +1x+2y ; +2x-1y ; +2x+1y
        if (p.x > 1) {
            if (p.y > 0) {
                if (((b[p.x - 2][p.y - 1] & 0xC0) != comp) or (!t)) {
                    v.push_back(p - L[0]);
                }
            }
            if (p.y < 7) {
                if (((b[p.x - 2][p.y + 1] & 0xC0) != comp) or (!t)) {
                    v.push_back(p + L[1]);
                }
            }
        }
        if (p.x > 0) {
            if (p.y > 1) {
                if (((b[p.x - 1][p.y - 2] & 0xC0) != comp) or (!t)) {
                    v.push_back(p - L[2]);
                }
            }
            if (p.y < 6) {
                if (((b[p.x - 1][p.y + 2] & 0xC0) != comp) or (!t)) {
                    v.push_back(p + L[3]);
                }
            }
        }
        if (p.x < 6) {
            if (p.y > 0) {
                if (((b[p.x + 2][p.y - 1] & 0xC0) != comp) or (!t)) {
                    v.push_back(p - L[1]);
                }
            }
            if (p.y < 7) {
                if (((b[p.x + 2][p.y + 1] & 0xC0) != comp) or (!t)) {
                    v.push_back(p + L[0]);
                }
            }
        }
        if (p.x < 7) {
            if (p.y > 1) {
                if (((b[p.x + 1][p.y - 2] & 0xC0) != comp) or (!t)) {
                    v.push_back(p - L[3]);
                }
            }
            if (p.y < 6) {
                if (((b[p.x + 1][p.y + 2] & 0xC0) != comp) or (!t)) {
                    v.push_back(p + L[2]);
                }
            }
        }
    }
    
    void Bishop :: check_moves (std::vector<pos>& v, bool t) {
        pos D[2] = {{1, 1}, {1, -1}};
        
        // up-left, up-right, down-left, down-right
        this->pb_inc (position + D[0], v, D[0], t);
        this->pb_inc (position + D[1], v, D[1], t);
        this->pb_inc (position - D[0], v, -D[0], t);
        this->pb_inc (position - D[1], v, -D[1], t);
    }
    
    void Rook :: check_moves (std::vector<pos>& v, bool t) {
        
        pos D[2] = {{1, 0}, {0, 1}};
        
        // up, down, left, right
        this->pb_inc (position + D[0], v, D[0], t);
        this->pb_inc (position + D[1], v, D[1], t);
        this->pb_inc (position - D[0], v, -D[0], t);
        this->pb_inc (position - D[1], v, -D[1], t);
    }
    
    void Queen :: check_moves (std::vector<pos>& v, bool t) {
        
        pos D[4] = {{1, 1}, {1, 0}, {1, -1}, {0, 1}};
        
        // up-left, up-right, down-left, down-right, up, down, left, right
        this->pb_inc (position + D[0], v, D[0], t);
        this->pb_inc (position + D[1], v, D[1], t);
        this->pb_inc (position - D[0], v, -D[0], t);
        this->pb_inc (position - D[1], v, -D[1], t);
        this->pb_inc (position + D[2], v, D[2], t);
        this->pb_inc (position + D[3], v, D[3], t);
        this->pb_inc (position - D[2], v, -D[2], t);
        this->pb_inc (position - D[3], v, -D[3], t);
    }
    
    void King :: check_moves (std::vector<pos>& v, bool t) {
        
        int8_t posx = position.x + 1;
        int8_t posy = position.y + 1;
        uint8_t** b = *pgb; // put the game board array in the current scope
        
        uint8_t comp = is_white ? 0x40 : 0x80;
        
        // UP-RIGHT
        if (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if (((b[posx][posy] & 0xC0) != comp) or (!t)) {
                v.push_back({posx, posy});
            }
        }
        // UP
        if (--posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if (((b[posx][posy] & 0xC0) != comp) or (!t)) {
                v.push_back({posx, posy});
            }
        }
        // UP-LEFT
        if (--posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if (((b[posx][posy] & 0xC0) != comp) or (!t)) {
                v.push_back({posx, posy});
            }
        }
        // LEFT
        if (posx >= 0 and --posy >= 0 and posx < 8 and posy < 8) {
            if (((b[posx][posy] & 0xC0) != comp) or (!t)) {
                v.push_back({posx, posy});
            }
        }
        // DOWN-LEFT
        if (posx >= 0 and --posy >= 0 and posx < 8 and posy < 8) {
            if (((b[posx][posy] & 0xC0) != comp) or (!t)) {
                v.push_back({posx, posy});
            }
        }
        // DOWN
        if (++posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if (((b[posx][posy] & 0xC0) != comp) or (!t)) {
                v.push_back({posx, posy});
            }
        }
        // DOWN-RIGHT
        if (++posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if (((b[posx][posy] & 0xC0) != comp) or (!t)) {
                v.push_back({posx, posy});
            }
        }
        // RIGHT
        if (posx >= 0 and ++posy >= 0 and posx < 8 and posy < 8) {
            if (((b[posx][posy] & 0xC0) != comp) or (!t)) {
                v.push_back({posx, posy});
            }
        }
//        for (const auto& move : valid_moves) { print_pos(move); }
    }
    
    uint8_t Pawn :: move (pos p) {
        // if first move occurs, it must be set to false.
        uint8_t** b = *pgb;
        uint8_t valid = 0;
        for (const auto& m : valid_moves) {
            if (m == p) {
                valid = 1;
                break;
            }
        }
        if (valid == 1) {
            first_move = false;
            uint8_t temp = b[position.x][position.y];
            b[position.x][position.y] = 0;
            if (p.y == 0 or p.y == 7) {
                valid |= promotion ();
            } else {
                position = p;
                b[position.x][position.y] = temp;
            }
        }
        return valid;
    }

    uint8_t King :: move (pos p) {
        // an exception case is made for the castling move (not yet implemented).
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
