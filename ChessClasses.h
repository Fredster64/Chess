#ifndef CHESSCLASSES_H
#define CHESSCLASSES_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

namespace chess {
    
    typedef struct Position {
        int8_t x;
        int8_t y;
        void print_pos (void) { std::cout << static_cast<char>(x + 'A') << static_cast<int>(y + 1) << std::endl; }
    } Pos;
    bool operator==(const Pos& p1, const Pos& p2) { // define pos equality
        return (p1.x == p2.x) and (p1.y == p2.y);
    }
    bool operator!=(const Pos& p1, const Pos& p2) { // define pos inequality
        return (p1.x != p2.x) or (p1.y != p2.y);
    }
    Pos operator+(const Pos& p1, const Pos& p2) { // define pos addition
        return { static_cast<int8_t>(p1.x + p2.x), static_cast<int8_t>(p1.y + p2.y) };
    }
    Pos operator-(const Pos& p1, const Pos& p2) { // define pos subtraction
        return { static_cast<int8_t>(p1.x - p2.x), static_cast<int8_t>(p1.y - p2.y) };
    }
    Pos operator+(const Pos& p) { // define unary positive
        return { static_cast<int8_t>(p.x), static_cast<int8_t>(p.y) };
    }
    Pos operator-(const Pos& p) { // define unary negative
        return { static_cast<int8_t>(-p.x), static_cast<int8_t>(-p.y) };
    }
    void operator+=(Pos& p1, const Pos& p2) { // define assignment addition
        p1 = p1 + p2;
    }
    void operator-=(Pos& p1, const Pos& p2) { // define assignment subtraction
        p1 = p1 - p2;
    }
    
    typedef struct LastMove {
        Pos lmf;
        Pos lmt;
        std::string lpt;
        void print_lm (void) { std::cout << lpt << " from " << static_cast<char>(lmf.x + 'A') << static_cast<int>(lmf.y + 1) << " to " << static_cast<char>(lmt.x + 'A') << static_cast<int>(lmt.y + 1) << std::endl;}
    } LM;
    
    typedef struct MovementCheckerInterface { // Movement-Checker Interface
        Pos position;
        bool first_move;
        uint8_t*** pgb; // *gb = board, gb = &board.
        void move_checker (std::vector<Pos>& v, std::string piece_type, bool w, bool t); // Handles movement of all pieces
        void pb_inc (Pos p, std::vector<Pos>& v, Pos inc, bool is_white, bool t);
        LM* lm_ptr;
    } MCI;
    
    /* Classes for the Game Pieces. Created by the Engine directly. */
    class Piece {
    public:
        Piece (const bool player_colour, const Pos coordinates, LM& last_move, uint8_t**& gb); // constructor
        ~Piece (void); // destructor
        std::vector<Pos> valid_moves;
        Pos get_pos (void) { return mci.position; }
        void print_info (void);
        void check_moves (std::vector<Pos>& v, bool t=true);
        virtual uint8_t move (const Pos p); // polymorphic, default for N, B, R, Q.
    protected:
        bool is_white; // stores if the piece is White (1) or Black (0).
        /* One-Line Functions */
        uint8_t** get_board (void) { return *mci.pgb; }
        void is_first_move (bool x) { mci.first_move = x; }
        void update_pos (Pos p) { mci.position = p; }
        void update_last_move (const Pos p_f, const Pos p_t, const std::string& s_pt) { *mci.lm_ptr = {p_f, p_t, s_pt}; }
    private:
        MCI mci; // Piece movement controller
        virtual std::string get_type (void) = 0; // pure polymorphic function
    };
    
    class Pawn : public Piece {
    public:
        using Piece :: Piece;
        uint8_t move (const Pos p);
    protected:
    private:
        std::string get_type (void) { return "Pawn"; }
        uint8_t promotion (void);
    };
    
    class Knight : public Piece {
    public:
        using Piece :: Piece;
    protected:
    private:
        std::string get_type (void) { return "Knight"; }
    };
    
    class Bishop : public Piece {
    public:
        using Piece :: Piece;
    protected:
    private:
        std::string get_type (void) { return "Bishop"; }
    };
    
    class Rook : public Piece {
    public:
        using Piece :: Piece;
        void castled (Pos p) { update_pos(p); }
    protected:
    private:
        std::string get_type (void) { return "Rook"; }
    };
    
    class Queen : public Piece {
    public:
        using Piece :: Piece;
    protected:
    private:
        std::string get_type (void) { return "Queen"; }
    };
    
    class King : public Piece {
    public:
        using Piece :: Piece;
        uint8_t move (const Pos p);
    protected:
    private:
        std::string get_type (void) { return "King"; }
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
        LM lm;
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
        /* if Bit3 HIGH - Unassigned                      */
        /* if Bit4 HIGH - Unassigned                      */
        /* if Bit5 HIGH - Current player is in Check.     */
        /* if Bit6 HIGH - Current player is in Checkmate. */
        /* if Bit7 HIGH - Game is in Stalemate.           */
        /**************************************************/
        // Vectors to store the game pieces in.
        std::vector<PiecePtr> white_pieces;
        std::vector<PiecePtr> black_pieces;
        // The Game is played in this function
        void play_game (void);
        // Functions that create and place the pieces upon class construction
        void place_pawns (const bool c, const int8_t r);
        void place_knights (const bool c, const int8_t r);
        void place_bishops (const bool c, const int8_t r);
        void place_rooks (const bool c, const int8_t r);
        void place_royals (const bool c, const int8_t r);
        // Functions that interact with the Pieces
        bool move_piece (Pos pfrom, Pos pto);
        bool in_check (bool c);
        // Prints the current board
        void print_board (void);
        // One-Line Functions
        Pos char2int (const char* p) { return {static_cast<int8_t>(p[0] - 'A'), static_cast<int8_t>(p[1] - '1')}; }
        void pb_ptr (std::vector<PiecePtr>& v1, std::vector<PiecePtr>& v2, const PiecePtr& pp, const bool c) { c ? v1.push_back(std::move(pp)) : v2.push_back(std::move(pp)); }
        void rm_dlt (std::vector<PiecePtr>& v, const Pos p2) {
            v.erase (std::remove_if (v.begin(), v.end(), [p2] (PiecePtr piece) -> bool {
                Pos p = piece->get_pos ();
                if (p == p2) return true;
                return false;
            }), v.end());
        }
    };
    /***********************************************************/
}

#endif
