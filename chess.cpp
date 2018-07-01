/* Includes */
#include <iostream>
#include <string>
#include <vector>

/* Define Macros */




/* Header Files */
#include "chess.h" // chess namespace with structs and classes here



/* Define Prototypes */




/* Main Function */
int main (void) {
    
    chess::Player_One p1;
    chess::Player_Two p2;
    
    bool p2_is_comp;
    bool p1_is_white;
    
    char state;
    
    do {
        std::cout << "Would you like to play against a Human or the Computer? (h/c)" << std::endl;
        std::cin >> state;
        if (state == 'h') { p2_is_comp = false; } else
        if (state == 'c') { p2_is_comp = true;  } else
        { std::cout << "Sorry, I didn't understand." << std::endl; }
    } while (state != 'h' && state != 'c');
    
    p2.is_computer (p2_is_comp);
    
    do {
        std::cout << "Is Player 1 playing as White or Black? (w/b)" << std::endl;
        std::cin >> state;
        if (state == 'w') { p1_is_white = true;  } else
        if (state == 'b') { p1_is_white = false; } else
        { std::cout << "Sorry, I didn't understand." << std::endl; }
    } while (state != 'w' && state != 'b');
    
    p1.is_white ( p1_is_white);
    p2.is_white (!p1_is_white);
    
    chess::Board cb;
    cb.board_init();
    cb.print_board();
    
    chess::Game g;
    g.setup(p1, p2);
    
    p1.print_pieces();
    p2.print_pieces();
    
    return 0;
}

/* External Functions */

