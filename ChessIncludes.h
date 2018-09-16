#ifndef CHESSINCLUDES_H
#define CHESSINCLUDES_H

#include <algorithm>

#include "ChessClasses.h" // Class definitions

/* All custom, non-member function prototypes go here. */

template <typename T>
bool vec_search (const std::vector<T>& v, const T item);
template <typename T>
void rm_dupes (std::vector<T>& v);

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

/********************************************************/

#endif
