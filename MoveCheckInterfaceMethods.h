#ifndef MOVECHECKINTERFACEMETHODS_H
#define MOVECHECKINTERFACEMETHODS_H

#include <iostream> 
#include <vector> 

#include "ChessClasses.h"

namespace chess {
    
    void MCI :: move_checker (std::vector<Pos>& v, std::string piece_type, bool w, bool t) {
  
        uint8_t** b = *pgb; // put the game board array in the current scope
        Pos p = position;
        uint8_t comp;

        if (piece_type == "Pawn") {

            Pos d0[2] = {{1, 0}, {0, 1}};
            
            // En-Passant
            if (lm_ptr->lpt == "Pawn" and (t)) {
                if ((lm_ptr->lmf.y == (w ? 6 : 1)) and (lm_ptr->lmt.y == (w ? 4 : 3)) and (p.y == (w ? 4 : 3))) {
                    if (((lm_ptr->lmt.x) == (p.x + 1)) and p.x < 6) {
                        w ? p += d0[1] : p -= d0[1];
                        v.push_back(p + d0[0]);
                    } else if (((lm_ptr->lmt.x) == (p.x - 1)) and p.x > 1) {
                        w ? p += d0[1] : p -= d0[1];
                        v.push_back(p - d0[0]);
                    }
                }
            }
            p = position; // resets position in case changed by en-passant.
            w ? p += d0[1] : p -= d0[1]; // adds/subtracts one y-unit from white/black.
            comp = w ? 0x80 : 0x40;
            if (p.x < 7) {
                if ((b[p.x + 1][p.y] & 0xC0) == comp or (!t)) {
                    v.push_back(p + d0[0]);
                }
            }
            if (p.x > 0) {
                if ((b[p.x - 1][p.y] & 0xC0) == comp or (!t)) {
                    v.push_back(p - d0[0]);
                }
            }
            if (t) {
                if (b[p.x][p.y] == 0 and p.y > 0 and p.y < 7) {
                    v.push_back({p.x, p.y});
                }
                if (first_move) {
                    if (b[p.x][p.y] == 0) {
                        w ? p += d0[1] : p -= d0[1];
                        if (b[p.x][p.y] == 0) {
                            v.push_back({p.x, p.y});
                        }
                    }
                }
            }
            return;
        }
        
        comp = w ? 0x40 : 0x80;

        if (piece_type == "Knight") {
            Pos L[4] = {{2, 1}, {-2, 1}, {1, 2}, {-1, 2}};
            // L-shapes: -2x-1y ; -2x+1y ; -1x-2y ; -1x+2y ; +1x-2y ; +1x+2y ; +2x-1y ; +2x+1y
            if (p.x > 1) {
              if (p.y > 0) {
                  if ((b[p.x - 2][p.y - 1] & 0xC0) != comp) v.push_back(p - L[0]);
              }
              if (p.y < 7) {
                  if ((b[p.x - 2][p.y + 1] & 0xC0) != comp) v.push_back(p + L[1]);
              }
            }
            if (p.x > 0) {
                if (p.y > 1) {
                    if ((b[p.x - 1][p.y - 2] & 0xC0) != comp) v.push_back(p - L[2]);
                }
                if (p.y < 6) {
                    if ((b[p.x - 1][p.y + 2] & 0xC0) != comp) v.push_back(p + L[3]);
                }
            }
            if (p.x < 6) {
                if (p.y > 0) {
                    if ((b[p.x + 2][p.y - 1] & 0xC0) != comp) v.push_back(p - L[1]);
                }
                if (p.y < 7) {
                    if ((b[p.x + 2][p.y + 1] & 0xC0) != comp) v.push_back(p + L[0]);
                }
            }
            if (p.x < 7) {
                if (p.y > 1) {
                    if ((b[p.x + 1][p.y - 2] & 0xC0) != comp) v.push_back(p - L[3]);
                }
                if (p.y < 6) {
                    if ((b[p.x + 1][p.y + 2] & 0xC0) != comp) v.push_back(p + L[2]);
                }
            }
            return;
        }

        if (piece_type == "Bishop" or piece_type == "Queen") {
            Pos d1[2] = {{1, 1}, {1, -1}};
            // up-left, up-right, down-left, down-right
            this->pb_inc (p + d1[0], v, d1[0], w, t);
            this->pb_inc (p + d1[1], v, d1[1], w, t);
            this->pb_inc (p - d1[0], v, -d1[0], w, t);
            this->pb_inc (p - d1[1], v, -d1[1], w, t);

            if (piece_type == "Bishop") return;
        }

        if (piece_type == "Rook" or piece_type == "Queen") {
            Pos d2[2] = {{1, 0}, {0, 1}};
            // up, down, left, right
            this->pb_inc (p + d2[0], v, d2[0], w, t);
            this->pb_inc (p + d2[1], v, d2[1], w, t);
            this->pb_inc (p - d2[0], v, -d2[0], w, t);
            this->pb_inc (p - d2[1], v, -d2[1], w, t);

            return;
        }

        if (piece_type == "King") {
            
            // castling
            Pos d3 = {2, 0};
            
            if (first_move) {
                // KS-Castle
                if ((b[p.x + 1][p.y] == 0) and (b[p.x + 2][p.y] == 0) and (b[p.x + 3][p.y] == w ? 0x48 : 0x88)) {
                    v.push_back(p + d3);
                }
                // QS-Castle
                if ((b[p.x - 1][p.y] == 0) and (b[p.x - 2][p.y] == 0) and (b[p.x - 3][p.y] == 0) and (b[p.x - 4][p.y] == w ? 0x48 : 0x88)) {
                    v.push_back(p - d3);
                }
            }
            
            
            
            
            int8_t posx = position.x + 1;
            int8_t posy = position.y + 1;
            
            // UP-RIGHT
            if (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
                if ((b[posx][posy] & 0xC0) != comp) {
                    v.push_back({posx, posy});
                }
            }
            // UP
            if (--posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
                if ((b[posx][posy] & 0xC0) != comp) {
                    v.push_back({posx, posy});
                }
            }
            // UP-LEFT
            if (--posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
                if ((b[posx][posy] & 0xC0) != comp) {
                    v.push_back({posx, posy});
                }
            }
            // LEFT
            if (posx >= 0 and --posy >= 0 and posx < 8 and posy < 8) {
                if ((b[posx][posy] & 0xC0) != comp) {
                    v.push_back({posx, posy});
                }
            }
            // DOWN-LEFT
            if (posx >= 0 and --posy >= 0 and posx < 8 and posy < 8) {
                if ((b[posx][posy] & 0xC0) != comp) {
                    v.push_back({posx, posy});
                }
            }
            // DOWN
            if (++posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
                if ((b[posx][posy] & 0xC0) != comp) {
                    v.push_back({posx, posy});
                }
            }
            // DOWN-RIGHT
            if (++posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
                if ((b[posx][posy] & 0xC0) != comp) {
                    v.push_back({posx, posy});
                }
            }
            // RIGHT
            if (posx >= 0 and ++posy >= 0 and posx < 8 and posy < 8) {
                if ((b[posx][posy] & 0xC0) != comp) {
                    v.push_back({posx, posy});
                }
            }
//            for (auto& move : v) { move.print_pos(); }
            return;
        }
    }
    
    void MCI :: pb_inc (Pos p, std::vector<Pos>& v, Pos inc, bool is_white, bool t) {
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
