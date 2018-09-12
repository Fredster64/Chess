#ifndef DERIVEDPIECEMETHODS_H
#define DERIVEDPIECEMETHODS_H

#include <vector>

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
        for (const auto& move : valid_moves) {
//            std::cout << static_cast<char>(move.x + 'A') << static_cast<int>(move.y + 1) << std::endl;
            print_pos(move);
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
        
        for (const auto& move : valid_moves) {
            print_pos(move);
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
        
        for (const auto& move : valid_moves) {
            print_pos(move);
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
        for (const auto& move : valid_moves) {
            print_pos(move);
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
        
        for (const auto& move : valid_moves) {
            print_pos(move);
        }
    }
    
    void King :: check_moves (void) {
        
        int8_t posx = position.x + 1;
        int8_t posy = position.y + 1;
        uint8_t** b = *pgb; // put the game board array in the current scope
        
        uint8_t comp = is_white ? 0x40 : 0x80;
        
        // up-left, up-right, down-left, down-right, up, down, left, right
        if (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) != comp) {
                valid_moves.push_back({posx--, posy});
            }
        }
        if (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) != comp) {
                valid_moves.push_back({posx--, posy});
            }
        }
        if (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) != comp) {
                valid_moves.push_back({posx, posy--});
            }
        }
        if (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) != comp) {
                valid_moves.push_back({posx, posy--});
            }
        }
        if (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) != comp) {
                valid_moves.push_back({posx++, posy});
            }
        }
        if (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) != comp) {
                valid_moves.push_back({posx++, posy});
            }
        }
        if (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) != comp) {
                valid_moves.push_back({posx, posy++});
            }
        }
        if (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) != comp) {
                valid_moves.push_back({posx, posy});
            }
        }
        for (const auto& move : valid_moves) {
            print_pos(move);
        }
    }
    
    void Pawn :: move (pos p) {
        // if first move occurs, it must be set to false.
        uint8_t** b = *pgb;
        bool valid = false;
        for (const auto& m : valid_moves) {
            if (m.x == p.x and m.y == p.y) {
                valid = true;
            }
        }
        if (valid) {
            first_move = false;
            uint8_t temp = b[position.x][position.y];
            b[position.x][position.y] = 0;
            position = p;
            b[position.x][position.y] = temp;
        }
    }

    void King :: move (pos p) {
        // an exception case is made for the castling move (not yet implemented).
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
    }
}

#endif
