#ifndef CHESSINCLUDES_H
#define CHESSINCLUDES_H

#include "ChessClasses.h" // Class definitions

/* All custom, non-member function prototypes go here. */

template <typename T>
bool vec_search (const std::vector<T>& v, const T item);

/*******************************************************/

#include "GameEngineMethods.h" // Methods for class GameEngine
#include "PieceMethods.h" // Methods for class Piece
#include "DerivedPieceMethods.h" // Methods from derived classes of Piece

/* All custom, non-member function definitions go here. */

template <typename T>
bool vec_search (const std::vector<T>& v, const T item) {
    for (const auto& it : v) {
        if (it == item) return true;
    }
    return false;
}

/********************************************************/

#endif
