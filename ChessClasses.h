#ifndef CHESSCLASSES_H
#define CHESSCLASSES_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>

namespace chess {
    
    struct pos {
        int8_t x;
        int8_t y;
    };
    bool operator==(const pos& p1, const pos& p2) {
        return (p1.x == p2.x) and (p1.y == p2.y);
    }
    
    /* Classes for the Game Pieces. Created by the Engine directly. */
    class Piece {
    public:
        Piece (const bool player_colour, const pos coordinates, uint8_t& status_bits, uint8_t**& gb); // constructor
        ~Piece (void); // destructor
        std::vector<pos> valid_moves;
        uint8_t check_gs (void) { return *pgs; }
        pos check_position (void) { return position; }
        void print_info (void);
        virtual void check_moves (std::vector<pos>& v, bool t=true) = 0; // pure polymorphic function
        virtual std::string get_type (void) = 0; // pure polymorphic function
        virtual uint8_t move (const pos p); // polymorphic, default for N, B, R, Q.
    protected:
        bool is_white; // stores if the piece is White (1) or Black (0).
        pos position; // the x-y position of the piece.
        uint8_t* pgs; // *pgs = game_status, pgs = &game_status.
        uint8_t*** pgb; // *pbg = board, pbg = &board.
        void print_pos (const pos p) { std::cout << static_cast<char>(p.x + 'A') << static_cast<int>(p.y + 1) << std::endl; }
    private:
    };
    
    class Pawn : public Piece {
    public:
        using Piece :: Piece;
        virtual void check_moves (std::vector<pos>& v, bool t=true);
        uint8_t move (const pos p);
        std::string get_type (void) { return "Pawn"; }
        void is_first_move (bool x) { first_move = x; }
    protected:
    private:
        uint8_t promotion (void);
        bool first_move;
    };
    
    class Knight : public Piece {
    public:
        using Piece :: Piece;
        void check_moves (std::vector<pos>& v, bool t=true);
        std::string get_type (void) { return "Knight"; }
    protected:
    private:
    };
    
    class Bishop : public Piece {
    public:
        using Piece :: Piece;
        void check_moves (std::vector<pos>& v, bool t=true);
        std::string get_type (void) { return "Bishop"; }
    protected:
    private:
    };
    
    class Rook : public Piece {
    public:
        using Piece :: Piece;
        void check_moves (std::vector<pos>& v, bool t=true);
        std::string get_type (void) { return "Rook"; }
    protected:
    private:
    };
    
    class Queen : public Piece {
    public:
        using Piece :: Piece;
        void check_moves (std::vector<pos>& v, bool t=true);
        std::string get_type (void) { return "Queen"; }
    protected:
    private:
    };
    
    class King : public Piece {
    public:
        using Piece :: Piece;
        void check_moves (std::vector<pos>& v, bool t=true);
        uint8_t move (const pos p);
        std::string get_type (void) { return "King"; }
    protected:
    private:
    };
    /****************************************************************/
    
    /* Create some smart pointers for the pieces */
    typedef std::shared_ptr<Piece> PiecePtr;
    typedef std::shared_ptr<Pawn> PawnPtr;
    typedef std::shared_ptr<Knight> KnightPtr;
    typedef std::shared_ptr<Bishop> BishopPtr;
    typedef std::shared_ptr<Rook> RookPtr;
    typedef std::shared_ptr<Queen> QueenPtr;
    typedef std::shared_ptr<King> KingPtr;
    
    /* Class for the Game Engine. Stores the game information. */
    class GameEngine {
    public:
        GameEngine (const bool player_colour); // constructor
        ~GameEngine (void); // destructor
    protected:
    private:
        // stores if the player White (1) or Black (0).
        bool is_white;
        // 8 bit-flags per square.
        uint8_t** board;
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
        // 8 bit-flags to check the status of the game
        uint8_t game_status;
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
        // Vectors to store the game pieces in.
        std::vector<PiecePtr> white_pieces;
        std::vector<PiecePtr> black_pieces;
        // Vector to save the King's danger cells.
        std::vector<pos> danger_cells;
        // The Game is played in this function
        void play_game (void);
        // Functions that create and place the pieces upon class construction
        void place_pawns (const bool c, const int8_t r);
        void place_knights (const bool c, const int8_t r);
        void place_bishops (const bool c, const int8_t r);
        void place_rooks (const bool c, const int8_t r);
        void place_royals (const bool c, const int8_t r);
        // Functions that interact with the Pieces
        bool move_piece (pos pfrom, pos pto);
        bool in_check (bool c);
        
        // Prints the current board
        void print_board (void);
        // One-Line Functions
        void print_pos (const pos p) { std::cout << static_cast<char>(p.x + 'A') << static_cast<int>(p.y + 1) << std::endl; }
        pos char2int (const char* p) { return {static_cast<int8_t>(p[0] - 'A'), static_cast<int8_t>(p[1] - '1')}; }
        void pbp (std::vector<PiecePtr>& v1, std::vector<PiecePtr>& v2, const PiecePtr& pp, const bool c) { c ? v1.push_back(std::move(pp)) : v2.push_back(std::move(pp)); }
    };
    /***********************************************************/
}

#endif
