#ifndef DERIVEDPIECEMETHODS_H
#define DERIVEDPIECEMETHODS_H

#include <iostream>
#include <vector>

#include "ChessClasses.h" // for Class Definitions
#include "PieceMethods.h" // Not sure if necessary but just in case (polymorphism)

namespace chess {
    
    void Pawn :: check_moves (void) {
        int8_t posx = position.x;
        int8_t posy = position.y;
        uint8_t** b = *pgb; // put the game board array in the current scope
        
        // will work on 'check' case conditions and en-passant rules later.
        is_white ? ++posy : --posy;
        if (b[posx][posy] == 0 and posy != 0 and posy != 7) {
            valid_moves.push_back({posx, posy});
        }
        uint8_t comp = is_white ? 0x80 : 0x40;
        if (posx < 7) {
            if ((b[posx + 1][posy] & 0xC0) == comp) {
                valid_moves.push_back({(++posx)--, posy});
            }
        }
        if (posx > 0) {
            if ((b[posx - 1][posy] & 0xC0) == comp) {
                valid_moves.push_back({(--posx)++, posy});
            }
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
    
    void Knight :: check_moves (void) {
        const int8_t posx = position.x;
        const int8_t posy = position.y;
        uint8_t** b = *pgb; // put the game board array in the current scope
        
        uint8_t comp = is_white ? 0x40 : 0x80;
        
        // L-shapes: -2x-1y ; -2x+1y ; -1x-2y ; -1x+2y ; +1x-2y ; +1x+2y ; +2x-1y ; +2x+1y
        if (posx > 1) {
            if (posy > 0) {
                if ((b[posx - 2][posy - 1] & 0xC0) != comp) {
                    valid_moves.push_back({static_cast<int8_t>(posx - 2), static_cast<int8_t>(posy - 1)});
                }
            }
            if (posy < 7) {
                if ((b[posx - 2][posy + 1] & 0xC0) != comp) {
                    valid_moves.push_back({static_cast<int8_t>(posx - 2), static_cast<int8_t>(posy + 1)});
                }
            }
        }
        if (posx > 0) {
            if (posy > 1) {
                if ((b[posx - 1][posy - 2] & 0xC0) != comp) {
                    valid_moves.push_back({static_cast<int8_t>(posx - 1), static_cast<int8_t>(posy - 2)});
                }
            }
            if (posy < 6) {
                if ((b[posx - 1][posy + 2] & 0xC0) != comp) {
                    valid_moves.push_back({static_cast<int8_t>(posx - 1), static_cast<int8_t>(posy + 2)});
                }
            }
        }
        if (posx < 6) {
            if (posy > 0) {
                if ((b[posx + 2][posy - 1] & 0xC0) != comp) {
                    valid_moves.push_back({static_cast<int8_t>(posx + 2), static_cast<int8_t>(posy - 1)});
                }
            }
            if (posy < 7) {
                if ((b[posx + 2][posy + 1] & 0xC0) != comp) {
                    valid_moves.push_back({static_cast<int8_t>(posx + 2), static_cast<int8_t>(posy + 1)});
                }
            }
        }
        if (posx < 7) {
            if (posy > 1) {
                if ((b[posx + 1][posy - 2] & 0xC0) != comp) {
                    valid_moves.push_back({static_cast<int8_t>(posx + 1), static_cast<int8_t>(posy - 2)});
                }
            }
            if (posy < 6) {
                if ((b[posx + 1][posy + 2] & 0xC0) != comp) {
                    valid_moves.push_back({static_cast<int8_t>(posx + 1), static_cast<int8_t>(posy + 2)});
                }
            }
        }
    }
    
    void Bishop :: check_moves (void) {
        int8_t posx = position.x + 1;
        int8_t posy = position.y + 1;
        uint8_t** b = *pgb; // put the game board array in the current scope
        
        uint8_t comp = is_white ? 0x40 : 0x80;
        
        // up-left, up-right, down-left, down-right
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx++, posy++});
            }
        }
        posx = position.x + 1;
        posy = position.y - 1;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx++, posy--});
            }
        }
        posx = position.x - 1;
        posy = position.y + 1;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx--, posy++});
            }
        }
        posx = position.x - 1;
        posy = position.y - 1;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx--, posy--});
            }
        }
    }
    
    void Rook :: check_moves (void) {
        int8_t posx = position.x + 1;
        int8_t posy = position.y;
        uint8_t** b = *pgb; // put the game board array in the current scope
        
        uint8_t comp = is_white ? 0x40 : 0x80;
        
        // up, down, left, right
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx++, posy});
            }
        }
        posx = position.x - 1;
        posy = position.y;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx--, posy});
            }
        }
        posx = position.x;
        posy = position.y + 1;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx, posy++});
            }
        }
        posx = position.x;
        posy = position.y - 1;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx, posy--});
            }
        }
    }
    
    void Queen :: check_moves (void) {
        int8_t posx = position.x + 1;
        int8_t posy = position.y + 1;
        uint8_t** b = *pgb; // put the game board array in the current scope
        
        uint8_t comp = is_white ? 0x40 : 0x80;
        
        // up-left, up-right, down-left, down-right, up, down, left, right
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx++, posy++});
            }
        }
        posx = position.x + 1;
        posy = position.y - 1;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx++, posy--});
            }
        }
        posx = position.x - 1;
        posy = position.y + 1;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx--, posy++});
            }
        }
        posx = position.x - 1;
        posy = position.y - 1;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx--, posy--});
            }
        }
        posx = position.x + 1;
        posy = position.y;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx++, posy});
            }
        }
        posx = position.x - 1;
        posy = position.y;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx--, posy});
            }
        }
        posx = position.x;
        posy = position.y + 1;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx, posy++});
            }
        }
        posx = position.x;
        posy = position.y - 1;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx, posy--});
            }
        }
    }
    
    void King :: check_moves (void) {
        
        int8_t posx = position.x + 1;
        int8_t posy = position.y + 1;
        uint8_t** b = *pgb; // put the game board array in the current scope
        
        uint8_t comp = is_white ? 0x40 : 0x80;
        
        // UP-RIGHT
        if (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) != comp) {
                valid_moves.push_back({posx, posy});
            }
        }
        // UP
        if (--posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) != comp) {
                valid_moves.push_back({posx, posy});
            }
        }
        // UP-LEFT
        if (--posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) != comp) {
                valid_moves.push_back({posx, posy});
            }
        }
        // LEFT
        if (posx >= 0 and --posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) != comp) {
                valid_moves.push_back({posx, posy});
            }
        }
        // DOWN-LEFT
        if (posx >= 0 and --posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) != comp) {
                valid_moves.push_back({posx, posy});
            }
        }
        // DOWN
        if (++posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) != comp) {
                valid_moves.push_back({posx, posy});
            }
        }
        // DOWN-RIGHT
        if (++posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) != comp) {
                valid_moves.push_back({posx, posy});
            }
        }
        // RIGHT
        if (posx >= 0 and ++posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) != comp) {
                valid_moves.push_back({posx, posy});
            }
        }
//        for (const auto& move : valid_moves) { print_pos(move); }
    }
    
    uint8_t Pawn :: move (pos p) {
        // if first move occurs, it must be set to false.
        uint8_t** b = *pgb;
        uint8_t valid = 0;
        for (const auto& m : valid_moves) {
            if (m.x == p.x and m.y == p.y) {
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
            if (m.x == p.x and m.y == p.y) {
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
