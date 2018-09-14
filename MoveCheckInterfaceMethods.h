#ifndef MOVECHECKINTERFACEMETHODS_H
#define MOVECHECKINTERFACEMETHODS_H

#include <iostream> 
#include <vector> 

#include "ChessClasses.h"

namespace chess {

  // Idea is to check moves based on pieceType. 
  // This should lessen the code repetition. 
  // Moreover, it should make implementing promotion easier: 
  // -- Change pieceType of a pawn to the relevant piece when it becomes promoted 
  // -- Now, pawn will move as if it's the other piece
  
  // I've also written the Queen move in terms of the Rook and Bishop ones
  
  void MoveCheckInterface :: check_moves (std::vector<pos>& v, bool t, std::string pieceType, pos startPos, bool isWhite) {
  
    uint8_t** b = *pgb; // put the game board array in the current scope
  
    int8_t posx, posy;
    uint8_t comp;
    bool is_white = isWhite;
  
    if ( pieceType == "Pawn" ) {
    
      posx = startPos.x;
      posy = startPos.y;

      // will work on 'check' case conditions and en-passant rules later.
      is_white ? ++posy : --posy;

      comp = is_white ? 0x80 : 0x40;
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
                  is_white ? ++posy : --posy;
                  if (b[posx][posy] == 0) {
                      valid_moves.push_back({posx, posy});
                  }
              }
          }
      }
      return;
    }
    
    else if ( pieceType == "Knight" ) {
      pos p = startPos;
      pos L[4] = {{2, 1}, {-2, 1}, {1, 2}, {-1, 2}};
      comp = is_white ? 0x40 : 0x80;
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
    
    else if ( pieceType == "Bishop" || pieceType == "Queen" ) {
        pos D[2] = {{1, 1}, {1, -1}};
        
        // up-left, up-right, down-left, down-right
        this->pb_inc (position + D[0], v, D[0], t);
        this->pb_inc (position + D[1], v, D[1], t);
        this->pb_inc (position - D[0], v, -D[0], t);
        this->pb_inc (position - D[1], v, -D[1], t);
        
        if (pieceType == "Bishop") return; // We still have more Queen moves to do
    }
    
    // Can't use else if here, it would skip half the Queen moves
    if ( pieceType == "Rook" || pieceType == "Queen" ) {
        pos D[2] = {{1, 0}, {0, 1}};
        
        // up, down, left, right
        this->pb_inc (position + D[0], v, D[0], t);
        this->pb_inc (position + D[1], v, D[1], t);
        this->pb_inc (position - D[0], v, -D[0], t);
        this->pb_inc (position - D[1], v, -D[1], t);
        
      return;
    }
  
    else if ( pieceType == "King" ) {
        posx = position.x + 1;
        posy = position.y + 1;
        
        comp = is_white ? 0x40 : 0x80;
        
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
//        for (const auto& move : valid_moves) { print_pos(move);
      return;
    } 
  return;
  }// end of function defn
  
 }// end of namespace declaration
