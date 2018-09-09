#ifndef CHESS_H
#define CHESS_H

#include <iostream>
#include <vector>
#include <string>

namespace chess {
    
    struct pos {
        int8_t x;
        int8_t y;
    };
    
    /* Classes for the Game Pieces. Created by the Engine directly. */
    class Piece {
    public:
        Piece (const bool player_colour, const pos coordinates, uint8_t& status_bits, uint8_t**& gb); // constructor
        ~Piece (); // destructor
        uint8_t check_gs () { return *pgs; }
        pos check_position () { return position; }
        void print_info ();
        virtual void check_moves () = 0; // polymorphic function, not applicable to base class.
        virtual std::string get_type() = 0;
    protected:
        bool is_white; // stores if the piece is White (1) or Black (0).
        bool is_taken;
        pos position; // the x-y position of the piece.
        std::vector<pos> valid_moves;
        uint8_t* pgs; // *pgs = game_status, pgs = &game_status.
        uint8_t*** pgb; // *pbg = board, pbg = &board.
        virtual void move () = 0; // polymorphic, not applicable to base class.
    private:
    };
    
    class Pawn : public Piece {
    public:
        using Piece :: Piece;
        void check_moves ();
        void is_first_move (bool x) { first_move = x; }
        std::string get_type() { return "Pawn"; }
    protected:
    private:
        bool first_move;
        void move ();
        void promotion ();
    };
    
    class Knight : public Piece {
    public:
        using Piece :: Piece;
        void check_moves ();
        std::string get_type() { return "Knight"; }
    protected:
    private:
        void move ();
    };
    
    class Bishop : public Piece {
    public:
        using Piece :: Piece;
        void check_moves ();
        std::string get_type() { return "Bishop"; }
    protected:
    private:
        void move ();
    };
    
    class Rook : public Piece {
    public:
        using Piece :: Piece;
        void check_moves ();
        std::string get_type() { return "Rook"; }
    protected:
    private:
        void move ();
        void ks_castle ();
        void qs_castle ();
    };
    
    class Queen : public Piece {
    public:
        using Piece :: Piece;
        void check_moves ();
        std::string get_type() { return "Queen"; }
    protected:
    private:
        void move ();
    };
    
    class King : public Piece {
    public:
        using Piece :: Piece;
        void check_moves ();
        std::string get_type() { return "King"; }
    protected:
    private:
        void move ();
        void ks_castle ();
        void qs_castle ();
    };
    /****************************************************************/
    
    /* Class for the Game Engine. Stores the game information. */
    class GameEngine {
    public:
        GameEngine (const bool player_colour); // constructor
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
        /* if Bit6 HIGH - White Piece on this square.     */
        /* if Bit7 HIGH - Black Piece on this square.     */
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
        std::vector<Piece*> white_pieces; // stores the white game pieces in a vector.
        std::vector<Piece*> black_pieces; // stores the black game pieces in a vector.
        /* Functions that create and place the pieces upon class construction */
        void place_pawns (const bool c, const int8_t r);
        void place_knights (const bool c, const int8_t r);
        void place_bishops (const bool c, const int8_t r);
        void place_rooks (const bool c, const int8_t r);
        void place_royals (const bool c, const int8_t r);
    };
    /***********************************************************/
    
    GameEngine :: GameEngine (const bool player_colour) {
        is_white = player_colour;
        game_status = 0x01; // normal, white to move first.
        /* Set up the game board */
        board = new uint8_t*[8];
        for (int8_t i = 0; i < 8; ++i) {
            board[i] = new uint8_t[8];
            for (int8_t j = 0; j < 8; ++j) {
                board[i][j] = 0;
            }
        }
        bool colour = true;
        int8_t row = 1;
        do {
            place_pawns (colour, row);
            colour ? --row : ++row;
            place_knights (colour, row);
            place_bishops (colour, row);
            place_rooks (colour, row);
            place_royals (colour, row);
            colour = !colour;
            row = 6;
        } while (!colour);
        
        for (const auto& wp : white_pieces) {
            wp->check_moves ();
            wp->print_info ();
        }
        for (const auto& bp : black_pieces) {
            bp->check_moves ();
            bp->print_info ();
        }
        
        print_board ();
    }
    
    GameEngine :: ~GameEngine (void) {
        for (uint8_t i = 0; i < 8; ++i) {
            delete board[i];
        }
        delete board;
    }
    
    void GameEngine :: place_pawns (const bool c, const int8_t r) {
        for (int8_t i = 0; i < 8; ++i) {
            Pawn* pawn = new Pawn (c, {i, r}, game_status, board);
            pawn->is_first_move(true);
            c ? white_pieces.push_back(pawn) : black_pieces.push_back(pawn);
            board[i][r] |= c ? 0x41 : 0x81; // saves info that a pawn is on that square.
        }
    }
    
    void GameEngine :: place_knights (const bool c, const int8_t r) {
        for (int8_t i = 1; i < 8; i += 5) {
            Knight* knight = new Knight (c, {i, r}, game_status, board);
            c ? white_pieces.push_back(knight) : black_pieces.push_back(knight);
            board[i][r] |= c ? 0x42 : 0x82; // saves info that a pawn is on that square.
        }
    }
    
    void GameEngine :: place_bishops (const bool c, const int8_t r) {
        for (int8_t i = 2; i < 8; i += 3) {
            Bishop* bishop = new Bishop (c, {i, r}, game_status, board);
            c ? white_pieces.push_back(bishop) : black_pieces.push_back(bishop);
            board[i][r] |= c ? 0x44 : 0x84; // saves info that a pawn is on that square.
        }
    }
    
    void GameEngine :: place_rooks (const bool c, const int8_t r) {
        for (int8_t i = 0; i < 8; i+=7) {
            Rook* rook = new Rook (c, {i, r}, game_status, board);
            c ? white_pieces.push_back(rook) : black_pieces.push_back(rook);
            board[i][r] |= c ? 0x48 : 0x88; // saves info that a pawn is on that square.
        }
    }
    
    void GameEngine :: place_royals (const bool c, const int8_t r) {
        Queen* queen = new Queen (c, {3, r}, game_status, board);
        c ? white_pieces.push_back(queen) : black_pieces.push_back(queen);
        board[3][r] |= c ? 0x50 : 0x90; // saves info that a pawn is on that square.
        King* king = new King (c, {4, r}, game_status, board);
        c ? white_pieces.push_back(king) : black_pieces.push_back(king);
        board[4][r] |= c ? 0x60 : 0xA0; // saves info that a pawn is on that square.
    }
    
    void GameEngine :: print_board (void) {
        std::cout << std::endl;
        uint8_t sq;
        for (int8_t j = 7; j >= 0; --j) {
            std::cout << " " << (j + 1) << " ";
            for (uint8_t i = 0; i < 8; ++i) {
                sq = board[i][j];
                if ((sq & 0x01) > 0) { // pawn
                    std::cout << "[p]";
                } else if ((sq & 0x02) > 0) { // knight
                    std::cout << "[N]";
                } else if ((sq & 0x04) > 0) { // bishop
                    std::cout << "[B]";
                } else if ((sq & 0x08) > 0) { // rook
                    std::cout << "[R]";
                } else if ((sq & 0x10) > 0) { // queen
                    std::cout << "[Q]";
                } else if ((sq & 0x20) > 0) { // king
                    std::cout << "[K]";
                } else { // empty
                    std::cout << "[ ]";
                }
            }
            std::cout << std::endl;
        }
        std::cout << "   ";
        for (char c = 'A'; c < 'I'; ++c) {
            std::cout << " " << c << " ";
        }
        std::cout << std::endl << std::endl;
    }
    
    /****************************************************/
    
    Piece :: Piece (const bool piece_colour, const pos coordinates, uint8_t& status_bits, uint8_t**& gb) {
        is_white = piece_colour;
        is_taken = false;
        position = coordinates;
        pgs = &status_bits; // copy the address of status_bits to a pointer.
        pgb = &gb; // copy the address of the game board to a pointer.
    }
    
    Piece :: ~Piece (void) {
        
    }
    
    void Piece :: print_info (void) {
        std::cout << (is_white ? "White " : "Black ") << this->get_type() << " at " << static_cast<char>(position.x + 'A') << static_cast<int>(position.y + 1) << std::endl;
    }
    
    
    void Pawn :: check_moves (void) {
        int8_t posx = position.x;
        int8_t posy = position.y;
        uint8_t** b = *pgb; // put the game board array in the current scope
        // check if the piece can move forwards (i.e. not blocked). we will work on 'check' case conditions and en-passant rules later.
        is_white ? ++posy : --posy;
        if (b[posx][posy] == 0) {
            valid_moves.push_back({posx, posy});
        }
        if (posx < 7) {
            if ((b[posx + 1][posy] & 0x80) > 0) {
                valid_moves.push_back({(++posx)--, posy});
            }
        }
        if (posx > 0) {
            if ((b[posx - 1][posy] & 0x80) > 0) {
                valid_moves.push_back({(--posx)++, posy});
            }
        }
        
        if (first_move) {
            is_white ? ++posy : --posy;
            if (b[posx][posy] == 0) {
                valid_moves.push_back({posx, posy});
            }
        }
        
        
        for (const auto& move : valid_moves) {
            std::cout << static_cast<int>(move.x) << " " << static_cast<int>(move.y) << std::endl;
        }
    }
    
    void Knight :: check_moves (void) {
        
    }
    
    void Bishop :: check_moves (void) {
        
    }
    
    void Rook :: check_moves (void) {
        
    }
    
    void Queen :: check_moves (void) {
        
    }
    
    void King :: check_moves (void) {
        
    }
    
    void Pawn :: move (void) {
        
    }
    
    void Knight :: move (void) {
        
    }
    
    void Bishop :: move (void) {
        
    }
    
    void Rook :: move (void) {
        
    }
    
    void Queen :: move (void) {
        
    }
    
    void King :: move (void) {
        
    }
}

#endif
