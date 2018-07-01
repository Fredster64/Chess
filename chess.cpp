/*****************************************/
/* "chess.cpp", and its relevant header  */
/* file, "chess.h", were created by the  */
/* author, Sach Patel. They were written */
/* using C++11 rules and conventions.    */
/*****************************************/


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
    
    // Add the Players
    chess::Player_One p1;
    chess::Player_Two p2;
    
    bool p2_is_comp;
    bool p1_is_white;
    
    char state;
    // Ask the user if they want to play a computer or a friend
    do {
        std::cout << "Would you like to play against a Human or the Computer? (h/c)" << std::endl;
        std::cin >> state;
        if (state == 'h') { p2_is_comp = false; } else
        if (state == 'c') { p2_is_comp = true;  } else
        { std::cout << "Sorry, I didn't understand." << std::endl; }
    } while (state != 'h' && state != 'c');
    // Ask the user which colour Player 1 should be
    do {
        std::cout << "Is Player 1 playing as White or Black? (w/b)" << std::endl;
        std::cin >> state;
        if (state == 'w') { p1_is_white = true;  } else
        if (state == 'b') { p1_is_white = false; } else
        { std::cout << "Sorry, I didn't understand." << std::endl; }
    } while (state != 'w' && state != 'b');
    
    // Set the user parameters in the respective classes.
    p2.is_computer (p2_is_comp);
    p1.is_white (  p1_is_white);
    p2.is_white ( !p1_is_white);
    
    // Define and build the gameboard.
    chess::Board cb;
    cb.board_init();
    
    // Debug only: Print the vector of board elements to the console.
    cb.print_board();
    
    // Define and set up the game engine.
    chess::Game g;
    g.setup(p1, p2);
    
    // Debug only : Print the vector of pieces to the console.
    p1.print_pieces();
    p2.print_pieces();
    
    return 0;
}

/* External Functions */

