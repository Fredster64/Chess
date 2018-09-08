#ifndef CHESS_H
#define CHESS_H

#include <iostream>

namespace chess {
    
    struct pos {
        uint8_t x;
        uint8_t y;
    };
    
    /* Class for the Game Engine. Stores the game information. */
    class GameEngine {
    public:
        GameEngine (bool player_colour); // constructor
        ~GameEngine (void); // destructor
        void place_pawns (bool c);
        void place_knights (bool c);
        void place_bishops (bool c);
        void place_rooks (bool c);
        void place_royals (bool c);
        void print_board (void); // prints the current board
    private:
        bool is_white; // stores if the player White (1) or Black (0).
        uint8_t board[8][8]; // 8 bit-flags per square.
        bool turn; // saves the colour of the piece currently moving
    };
    /***********************************************************/
    
    /* Classes for the Game Pieces. Created by the Engine directly. */
    class Piece {
    public:
        Piece (bool player_colour, pos coordinates); // constructor
        ~Piece (); // destructor
    protected:
        bool is_white; // stores if the piece is White (1) or Black (0).
        pos position; // the x-y posistion of the piece.
    private:
    };
    
    Piece :: Piece (bool piece_colour, pos coordinates) {
        is_white = piece_colour;
        position = coordinates;
    }
    
    class Pawn : public Piece {
    public:
        using Piece :: Piece;
    protected:
    private:
    };
    
    class Knight : public Piece {
    public:
        using Piece :: Piece;
    protected:
    private:
    };
    
    class Bishop : public Piece {
    public:
        using Piece :: Piece;
    protected:
    private:
    };
    
    class Rook : public Piece {
    public:
        using Piece :: Piece;
    protected:
    private:
    };
    
    class Queen : public Piece {
    public:
        using Piece :: Piece;
    protected:
    private:
    };
    
    class King : public Piece {
    public:
        using Piece :: Piece;
    protected:
    private:
    };
    /****************************************************************/
    
    GameEngine :: GameEngine (bool player_colour) {
        is_white = player_colour;
        turn = true;
        /* Set up the game board */
        for (uint8_t i = 0; i < 8; ++i) {
            for (uint8_t j = 0; j < 8; ++j) {
                board[i][j] = 0;
            }
        }
        bool colour = true;
        do {
            place_pawns (colour);
            place_knights (colour);
            place_bishops (colour);
            place_rooks (colour);
            place_royals (colour);
            colour = !colour;
        } while (!colour);
        print_board ();
    }
    
    GameEngine :: ~GameEngine (void) {
        
    }
    
    void GameEngine :: place_pawns (bool c) {
        uint8_t row = c ? 6 : 1; // white on R6, black on R1.
        for (uint8_t i = 0; i < 8; ++i) {
            Pawn* pawn = new Pawn (c, {i, row});
            board[i][row] |= 0x01; // saves info that a pawn is on that square.
        }
    }
    
    void GameEngine :: place_knights (bool c) {
        uint8_t row = c ? 7 : 0; // white on R7, black on R0.
        for (uint8_t i = 1; i < 8; i += 5) {
            Knight* knight = new Knight (c, {i, row});
            board[i][row] |= 0x02; // saves info that a knight is on that square.
        }
    }
    
    void GameEngine :: place_bishops (bool c) {
        uint8_t row = c ? 7 : 0; // white on R7, black on R0.
        for (uint8_t i = 2; i < 8; i += 3) {
            Bishop* bishop = new Bishop (c, {i, row});
            board[i][row] |= 0x04; // saves info that a bishop is on that square.
        }
    }
    
    void GameEngine :: place_rooks (bool c) {
        uint8_t row = c ? 7 : 0; // white on R7, black on R0.
        for (uint8_t i = 0; i < 8; i+=7) {
            Rook* rook = new Rook (c, {i, row});
            board[i][row] |= 0x08; // saves info that a rook is on that square.
        }
    }
    
    void GameEngine :: place_royals (bool c) {
        uint8_t row = c ? 7 : 0; // white on R7, black on R0.
        Queen* queen = new Queen (c, {3, row});
        board[3][row] |= 0x10; // saves info that a queen is on that square.
        King* king = new King (c, {4, row});
        board[4][row] |= 0x20; // saves info that a king is on that square.
    }
    
    void GameEngine :: print_board (void) {
        uint8_t sq;
        for (uint8_t j = 0; j < 8; ++j) {
            for (uint8_t i = 0; i < 8; ++i) {
                sq = board[i][j];
                if (sq == 0x01) { // pawn
                    std::cout << "[p]";
                } else if (sq == 0x02) { // knight
                    std::cout << "[N]";
                } else if (sq == 0x04) { // bishop
                    std::cout << "[B]";
                } else if (sq == 0x08) { // rook
                    std::cout << "[R]";
                } else if (sq == 0x10) { // queen
                    std::cout << "[Q]";
                } else if (sq == 0x20) { // king
                    std::cout << "[K]";
                } else { // empty
                    std::cout << "[ ]";
                }
            }
            std::cout << std::endl;
        }
    }
}

#endif
