#ifndef CHESS_H
#define CHESS_H

#include <iostream>
#include <vector>

namespace chess {
    
    struct pos {
        uint8_t x;
        uint8_t y;
    };
    
    /* Classes for the Game Pieces. Created by the Engine directly. */
    class Piece {
    public:
        Piece (bool player_colour, pos coordinates, uint8_t& status_bits); // constructor
        ~Piece (); // destructor
        uint8_t check_gs () { return *pgs; };
        // virtual void check_moves () = 0;
    protected:
        bool is_white; // stores if the piece is White (1) or Black (0).
        bool is_taken;
        pos position; // the x-y posistion of the piece.
        std::vector<pos> valid_moves;
        uint8_t* pgs; // *pgs = game_status, pgs = &game_status.
    private:
    };
    
    class Pawn : public Piece {
    public:
        using Piece :: Piece;
        void check_moves ();
    protected:
    private:
    };
    
    class Knight : public Piece {
    public:
        using Piece :: Piece;
        void check_moves ();
    protected:
    private:
    };
    
    class Bishop : public Piece {
    public:
        using Piece :: Piece;
        void check_moves ();
    protected:
    private:
    };
    
    class Rook : public Piece {
    public:
        using Piece :: Piece;
        void check_moves ();
    protected:
    private:
    };
    
    class Queen : public Piece {
    public:
        using Piece :: Piece;
        void check_moves ();
    protected:
    private:
    };
    
    class King : public Piece {
    public:
        using Piece :: Piece;
        void check_moves ();
    protected:
    private:
    };
    /****************************************************************/
    
    /* Class for the Game Engine. Stores the game information. */
    class GameEngine {
    public:
        GameEngine (bool player_colour); // constructor
        ~GameEngine (void); // destructor
        void print_board (void); // prints the current board
    protected:
    private:
        bool is_white; // stores if the player White (1) or Black (0).
        uint8_t** board; // 8 bit-flags per square.
        /**************************************************/
        /* if Bit0 HIGH - Pawn on this square.            */
        /* if Bit1 HIGH - Knight on this square.          */
        /* if Bit2 HIGH - Bishop on this square.          */
        /* if Bit3 HIGH - Rook on this square.            */
        /* if Bit4 HIGH - Queen on this square.           */
        /* if Bit5 HIGH - King on this square.            */
        /* if Bit6 HIGH - Unassigned.                     */
        /* if Bit7 HIGH - Colour of the square.           */
        /**************************************************/
        uint8_t game_status; // 8 bit-flags to check the status of the game.
        /**************************************************/
        /* if Bit0 HIGH - Whites turn, else Blacks turn.  */
        /* if Bit1 HIGH - White won the game.             */
        /* if Bit2 HIGH - Black won the game.             */
        /* if Bit3 HIGH - White Pawn to be promoted.      */
        /* if Bit4 HIGH - Black Pawn to be promoted.      */
        /* if Bit5 HIGH - Current player is in Check.     */
        /* if Bit6 HIGH - Current player is in Checkmate. */
        /* if Bit7 HIGH - Game is in Stalemate.           */
        /**************************************************/
        std::vector<Piece*> game_pieces; // stores the game pieces in a vector.
        /* Functions that create and place the pieces upon class construction */
        void place_pawns (const bool& c, const uint8_t& r);
        void place_knights (const bool& c, const uint8_t& r);
        void place_bishops (const bool& c, const uint8_t& r);
        void place_rooks (const bool& c, const uint8_t& r);
        void place_royals (const bool& c, const uint8_t& r);
    };
    /***********************************************************/
    
    GameEngine :: GameEngine (bool player_colour) {
        is_white = player_colour;
        game_status = 0x01; // normal, white to move first.
        /* Set up the game board */
        board = new uint8_t*[8];
        for (uint8_t i = 0; i < 8; ++i) {
            board[i] = new uint8_t[8];
            for (uint8_t j = 0; j < 8; ++j) {
                board[i][j] = 0;
            }
        }
        bool colour = true;
        uint8_t row = 6;
        do {
            place_pawns (colour, row);
            colour ? ++row : --row;
            place_knights (colour, row);
            place_bishops (colour, row);
            place_rooks (colour, row);
            place_royals (colour, row);
            colour = !colour;
            row = 1;
        } while (!colour);
        
        print_board ();
    }
    
    GameEngine :: ~GameEngine (void) {
        for (uint8_t i = 0; i < 8; ++i) {
            delete board[i];
        }
        delete board;
    }
    
    void GameEngine :: place_pawns (const bool& c, const uint8_t& r) {
        for (uint8_t i = 0; i < 8; ++i) {
            Pawn* pawn = new Pawn (c, {i, r}, game_status);
            game_pieces.push_back(pawn);
            board[i][r] |= 0x01; // saves info that a pawn is on that square.
        }
    }
    
    void GameEngine :: place_knights (const bool& c, const uint8_t& r) {
        for (uint8_t i = 1; i < 8; i += 5) {
            Knight* knight = new Knight (c, {i, r}, game_status);
            game_pieces.push_back(knight);
            board[i][r] |= 0x02; // saves info that a knight is on that square.
        }
    }
    
    void GameEngine :: place_bishops (const bool& c, const uint8_t& r) {
        for (uint8_t i = 2; i < 8; i += 3) {
            Bishop* bishop = new Bishop (c, {i, r}, game_status);
            game_pieces.push_back(bishop);
            board[i][r] |= 0x04; // saves info that a bishop is on that square.
        }
    }
    
    void GameEngine :: place_rooks (const bool& c, const uint8_t& r) {
        for (uint8_t i = 0; i < 8; i+=7) {
            Rook* rook = new Rook (c, {i, r}, game_status);
            game_pieces.push_back(rook);
            board[i][r] |= 0x08; // saves info that a rook is on that square.
        }
    }
    
    void GameEngine :: place_royals (const bool& c, const uint8_t& r) {
        Queen* queen = new Queen (c, {3, r}, game_status);
        game_pieces.push_back(queen);
        board[3][r] |= 0x10; // saves info that a queen is on that square.
        King* king = new King (c, {4, r}, game_status);
        game_pieces.push_back(king);
        board[4][r] |= 0x20; // saves info that a king is on that square.
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
    
    /****************************************************/
    
    Piece :: Piece (bool piece_colour, pos coordinates, uint8_t& status_bits) {
        is_white = piece_colour;
        is_taken = false;
        position = coordinates;
        pgs = &status_bits; // copy the address of status_bits to a pointer.
    }
    
    Piece :: ~Piece (void) {
        
    }
    
    void Pawn :: check_moves (void) {
        
    }
}

#endif
