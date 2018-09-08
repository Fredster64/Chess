#include <iostream>
#include <string>

#include "chess.h"

int main (void) {
    
    std::cout << "Would you like to be White or Black?" << std::endl;
    std::string a;
    // note: while a is of type 'std::string', a[k] is of type 'const char *' for k in [0, a.length() - 1].
    do {
        std::cin >> a;
        if (!std::cin) {
            /* If the input was invalid: */
            std::cout << "Something went wrong. Please try again." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            a = "";
        } else if (a == "White" or a == "white" or a == "w" or a == "W") {
            /* If White was chosen: */
            a = "W";
        } else if (a == "Black" or a == "black" or a == "b" or a == "B") {
            /* If Black was chosen: */
            a = "B";
        } else if (a[0] == 'B' or a[0] == 'b') { // use type 'char' for comparison
            /* If there was a spelling error but starts with 'B' or 'b': */
            std::cout << "Did you mean 'Black'? (y/n)" << std::endl;
            std::cin >> a;
            if (a == "y" or a == "yes" or a == "Y" or a == "Yes") {
                a = "B";
            } else if (a == "n" or a == "no" or a == "N" or a == "No") {
                a = "W";
            } else {
                std::cout << "Sorry, I didn't understand." << std::endl;
                a = "";
            }
        } else if (a[0] == 'W' or a[0] == 'w') { // use type 'char' for comparison
            /* If there was a spelling error but starts with 'W' or 'w': */
            std::cout << "Did you mean 'White'? (y/n)" << std::endl;
            std::cin >> a;
            if (a == "y" or a == "yes" or a == "Y" or a == "Yes") {
                a = "W";
            } else if (a == "n" or a == "no" or a == "N" or a == "No") {
                a = "B";
            } else {
                std::cout << "Sorry, I didn't understand." << std::endl;
                a = "";
            }
        } else {
            /* If neither White nor Black was selected. */
            std::cout << "That's not a valid answer. Please try again." << std::endl;
            a = "";
        }
    } while (a != "W" and a != "B");
    
    bool is_white = (a == "W") ? true : false;
    std::cout << "You have chosen to be: " << (is_white ? "White" : "Black") << "!" << std::endl;
    
    // Allocate memory for the game, and execute its constructor.
    chess::GameEngine* game = new chess::GameEngine (is_white);
    
    // Deallocate the memory created for 'game'.
    delete game;
    return 0;
}

