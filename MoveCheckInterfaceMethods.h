#ifndef MOVECHECKINTERFACEMETHODS_H
#define MOVECHECKINTERFACEMETHODS_H

#include <iostream> 
#include <vector> 

#include "ChessClasses.h"

namespace chess {
    
    void MCI :: check_moves (std::vector<Pos>& v, std::string piece_type, bool W, bool t) {

        // Idea is to check moves based on piece_type.
        // This should lessen the code repetition.
        // Moreover, it should make implementing promotion easier:
        // -- Change piece_type of a pawn to the relevant piece when it becomes promoted
        // -- Now, pawn will move as if it's the other piece
        // I've also written the Queen move in terms of the Rook and Bishop ones
  
        uint8_t** b = *gb; // put the game board array in the current scope

        int8_t posx, posy;
        uint8_t comp;

        if (piece_type == "Pawn") {
            posx = position.x;
            posy = position.y;

            // will work on 'check' case conditions and en-passant rules later.
            W ? ++posy : --posy;

            comp = W ? 0x80 : 0x40;
            if (posx < 7) {
                if ((b[posx + 1][posy] & 0xC0) == comp) {
                    v.push_back({(++posx)--, posy});
                }
            }
            if (posx > 0) {
                if ((b[posx - 1][posy] & 0xC0) == comp) {
                    v.push_back({(--posx)++, posy});
                }
            }
            if (t) {
                if (b[posx][posy] == 0 and posy != 0 and posy != 7) {
                    v.push_back({posx, posy});
                }
                if (first_move) {
                    if (b[posx][posy] == 0) {
                        W ? ++posy : --posy;
                        if (b[posx][posy] == 0) {
                            v.push_back({posx, posy});
                        }
                    }
                }
            }
            return;
        }

        if (piece_type == "Knight") {
            Pos p = position;
            Pos L[4] = {{2, 1}, {-2, 1}, {1, 2}, {-1, 2}};
            comp = W ? 0x40 : 0x80;
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
            
            Pos D[2] = {{1, 1}, {1, -1}};
            // up-left, up-right, down-left, down-right
            this->pb_inc (position + D[0], v, D[0], W, t);
            this->pb_inc (position + D[1], v, D[1], W, t);
            this->pb_inc (position - D[0], v, -D[0], W, t);
            this->pb_inc (position - D[1], v, -D[1], W, t);

            if (piece_type == "Bishop") return;
        }

        if (piece_type == "Rook" or piece_type == "Queen") {
            Pos D[2] = {{1, 0}, {0, 1}};

            // up, down, left, right
            this->pb_inc (position + D[0], v, D[0], W, t);
            this->pb_inc (position + D[1], v, D[1], W, t);
            this->pb_inc (position - D[0], v, -D[0], W, t);
            this->pb_inc (position - D[1], v, -D[1], W, t);

            return;
        }

        if (piece_type == "King") {
            posx = position.x + 1;
            posy = position.y + 1;
            comp = W ? 0x40 : 0x80;

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
            //        for (const auto& move : valid_moves) { print_pos(move); }
            return;
        }
    }
    
    void MCI :: pb_inc (Pos p, std::vector<Pos>& v, Pos inc, bool is_white, bool t) {
        uint8_t** b = *gb; // put the game board array in the current scope
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
