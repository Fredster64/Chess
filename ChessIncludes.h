#ifndef CHESSINCLUDES_H
#define CHESSINCLUDES_H

#include <memory>

#include "ChessClasses.h" // Class definitions

/* All custom, non-member function prototypes go here. */

void rm_dlt (std::vector<chess::PiecePtr>& v, const chess::pos p2);
template <typename T>
bool vec_search (const std::vector<T>& v, const T item);

/*******************************************************/

#include "GameEngineMethods.h" // Methods for class GameEngine
#include "PieceMethods.h" // Methods for class Piece
#include "DerivedPieceMethods.h" // Methods from derived classes of Piece

/* All custom, non-member function definitions go here. */

void rm_dlt (std::vector<chess::PiecePtr>& v, const chess::pos p2) {
    v.erase (std::remove_if (v.begin(), v.end(), [p2] (chess::PiecePtr piece) -> bool {
        chess::pos p = piece->check_position();
        if (p == p2) return true;
        return false;
    }), v.end());
}

template <typename T>
bool vec_search (const std::vector<T>& v, const T item) {
    for (const auto& it : v) {
        if (it == item) return true;
    }
    return false;
}

/********************************************************/

#endif
