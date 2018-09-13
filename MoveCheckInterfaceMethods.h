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
  void MoveCheckInterface::check_moves (std::vector<pos>& v, bool t=true, std::string pieceType, pos startPos) 
  
    uint8_t** b = *pgb; // put the game board array in the current scope
  
    int8_t posx, posy;
  
    if ( pieceType == "Pawn" ) {
    
      posx = startPos.x;
      posy = startPos.y;

      // will work on 'check' case conditions and en-passant rules later.
      is_white ? ++posy : --posy;

      uint8_t comp = is_white ? 0x80 : 0x40;
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
      uint8_t comp = is_white ? 0x40 : 0x80;
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
        int8_t posx = startPos.x + 1;
        int8_t posy = startPos.y + 1;
        
        uint8_t comp = is_white ? 0x40 : 0x80;
        
        // up-left, up-right, down-left, down-right
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                v.push_back({posx, posy});
                break;
            } else {
                v.push_back({posx++, posy++});
            }
        }
        posx = startPos.x + 1;
        posy = startPos.y - 1;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                v.push_back({posx, posy});
                break;
            } else {
                v.push_back({posx++, posy--});
            }
        }
        posx = startPos.x - 1;
        posy = startPos.y + 1;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                v.push_back({posx, posy});
                break;
            } else {
                v.push_back({posx--, posy++});
            }
        }
        posx = startPos.x - 1;
        posy = startPos.y - 1;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                v.push_back({posx, posy});
                break;
            } else {
                v.push_back({posx--, posy--});
            }
        }
        if (pieceType == "Bishop") return; // We still have more Queen moves to do
    }
    
    // Can't use else if here, it would skip half the Queen moves
    if ( pieceType == "Rook" || pieceType == "Queen" ) {
      
    }
  
  }
  
 }
