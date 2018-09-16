#ifndef CHESSINCLUDES_H
#define CHESSINCLUDES_H

#include <algorithm>

#include "ChessClasses.h" // Class definitions

/* All custom, non-member function prototypes go here. */

template <typename T>
bool vec_search (const std::vector<T>& v, const T item);
template <typename T>
void rm_dupes (std::vector<T>& v);

bool in_check (bool c, uint8_t** board); // Moved from GameEngine

/*******************************************************/

#include "GameEngineMethods.h" // Methods for class GameEngine
#include "PieceMethods.h" // Methods for class Piece
#include "MoveCheckInterfaceMethods.h" // Methods for the MCI
#include "DerivedPieceMethods.h" // Methods from derived classes of Piece

/* All custom, non-member function definitions go here. */

template <typename T>
bool vec_search (const std::vector<T>& v, const T item) {
    for (const auto& it : v) {
        if (it == item) return true;
    }
    return false;
}
template <typename T>
void rm_dupes (std::vector<T>& v) {
    int8_t k = 0;
    v.erase (std::remove_if (v.begin(), v.end(), [&v, &k] (T t) -> bool {
        for (int8_t i = v.size() - 1; i > k; --i) {
            if (t == v[i]) {
                ++k;
                return true;
            }
        }
        ++k;
        return false;
    }), v.end());
}

// in_check moved from GameEngine
bool in_check (bool c, uint8_t** board) {
        // find all cells that the king cannot exist in. If the current cell is in the list, then the king is in check.
        uint8_t k_score = c ? 0x60 : 0xA0;
        uint8_t comp = c ? 0x40 : 0x80;
        Pos k_pos;
        std::vector<Pos> vec;
        // search for the King's cell
        for (int8_t j = 0; j < 8; ++j) {
            for (int8_t i = 0; i < 8; ++i) {
                if ((board[i][j] & k_score) == k_score) {
                    k_pos = {i, j};
                    break;
                }
            }
        }
        // look for all cells that can be attacked by the opponent
        for (const auto& piece : (c ? black_pieces : white_pieces)) { piece->check_moves (vec, false); }
        rm_dupes<Pos>(vec);
        for (auto& cell : vec) {
            cell.print_pos();
        }
        
        // check if the k_pos is in v.
        return vec_search<Pos>(vec, k_pos);
    }

/********************************************************/

#endif
