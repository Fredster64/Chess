#ifndef CHESSCLASSES_H
#define CHESSCLASSES_H

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
        virtual ~Piece (void); // destructor
        std::vector<pos> valid_moves;
        uint8_t check_gs (void) { return *pgs; }
        pos check_position (void) { return position; }
        void print_info (void);
        virtual void check_moves (void) = 0; // pure polymorphic function
        virtual std::string get_type (void) = 0; // pure polymorphic function
        virtual bool move (const pos p); // polymorphic, default for N, B, R, Q.
    protected:
        bool is_white; // stores if the piece is White (1) or Black (0).
        bool is_taken;
        pos position; // the x-y position of the piece.
        uint8_t* pgs; // *pgs = game_status, pgs = &game_status.
        uint8_t*** pgb; // *pbg = board, pbg = &board.
        void print_pos (const pos p) { std::cout << static_cast<char>(p.x + 'A') << static_cast<int>(p.y + 1) << std::endl; }
    private:
    };
    
    class Pawn : public Piece {
    public:
        using Piece :: Piece;
        void check_moves (void);
        bool move (const pos p);
        void is_first_move (bool x) { first_move = x; }
        std::string get_type (void) { return "Pawn"; }
    protected:
    private:
        bool first_move;
        void promotion (void);
    };
    
    class Knight : public Piece {
    public:
        using Piece :: Piece;
        void check_moves (void);
        std::string get_type (void) { return "Knight"; }
    protected:
    private:
    };
    
    class Bishop : public Piece {
    public:
        using Piece :: Piece;
        void check_moves (void);
        std::string get_type (void) { return "Bishop"; }
    protected:
    private:
    };
    
    class Rook : public Piece {
    public:
        using Piece :: Piece;
        void check_moves (void);
        std::string get_type (void) { return "Rook"; }
    protected:
    private:
    };
    
    class Queen : public Piece {
    public:
        using Piece :: Piece;
        void check_moves (void);
        std::string get_type (void) { return "Queen"; }
    protected:
    private:
    };
    
    class King : public Piece {
    public:
        using Piece :: Piece;
        void check_moves (void);
        bool move (const pos p);
        std::string get_type (void) { return "King"; }
    protected:
    private:
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
        bool move_piece (pos pfrom, pos pto);
        void play_game (void);
        void print_pos (const pos p) { std::cout << static_cast<char>(p.x + 'A') << static_cast<int>(p.y + 1) << std::endl; }
        pos char2int (const char* p) { return {static_cast<int8_t>(p[0] - 'A'), static_cast<int8_t>(p[1] - '1')}; } };
    /***********************************************************/
}

#endif
