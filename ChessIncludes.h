#ifndef CHESSINCLUDES_H
#define CHESSINCLUDES_H

#include "ChessClasses.h" // Class definitions

/* All custom, non-member function prototypes go here. */

template <typename T>
bool vec_search (const std::vector<T>& v, const T item);
template <typename T>
void rm_dupes (std::vector<T>& v);

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
template <typename T>
void rm_dupes (std::vector<T>& v) {
    for (int8_t i = v.size() - 1; i >= 0; --i) {
        for (int8_t j = i - 1; j >= 0; --j) {
            if (v[i] == v[j]) {
                v.erase(v.begin() + i);
                break;
            }
        }
    }
}

/********************************************************/

#endif
