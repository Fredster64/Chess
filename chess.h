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
        std::vector<pos> valid_moves;
        uint8_t check_gs () { return *pgs; }
        pos check_position () { return position; }
        void print_info ();
        virtual void check_moves () = 0; // pure polymorphic function
        virtual std::string get_type() = 0; // pure polymorphic function
        virtual void move (pos p); // polymorphic, default for N, B, R, Q.
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
        void check_moves ();
        void move (pos p);
        void is_first_move (bool x) { first_move = x; }
        std::string get_type() { return "Pawn"; }
    protected:
    private:
        bool first_move;
        void promotion ();
    };
    
    class Knight : public Piece {
    public:
        using Piece :: Piece;
        void check_moves ();
        std::string get_type() { return "Knight"; }
    protected:
    private:
    };
    
    class Bishop : public Piece {
    public:
        using Piece :: Piece;
        void check_moves ();
        std::string get_type() { return "Bishop"; }
    protected:
    private:
    };
    
    class Rook : public Piece {
    public:
        using Piece :: Piece;
        void check_moves ();
        std::string get_type() { return "Rook"; }
    protected:
    private:
    };
    
    class Queen : public Piece {
    public:
        using Piece :: Piece;
        void check_moves ();
        std::string get_type() { return "Queen"; }
    protected:
    private:
    };
    
    class King : public Piece {
    public:
        using Piece :: Piece;
        void check_moves ();
        void move (pos p);
        std::string get_type() { return "King"; }
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
        void move_piece (pos pfrom, pos pto);
        void play_game (void);
        void print_pos (const pos p) { std::cout << static_cast<char>(p.x + 'A') << static_cast<int>(p.y + 1) << std::endl; }
        pos char2int (const char* p) { return {static_cast<int8_t>(p[0] - 'A'), static_cast<int8_t>(p[1] - '1')}; } };
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
        // piece information
        for (const auto& bp : black_pieces) {
            bp->print_info ();
        }
        for (const auto& wp : white_pieces) {
            wp->print_info ();
            wp->check_moves ();
        }
        
        print_board ();
        
        play_game ();
    }
    
    GameEngine :: ~GameEngine (void) {
        for (uint8_t i = 0; i < 8; ++i) {
            delete board[i];
        }
        delete board;
    }
    
    void GameEngine :: play_game (void) {
        
        while ((game_status & 0xC0) == 0) {
            
//            move_piece ({3, 1}, {3, 3}); // moves the pawn from D2 to D4.
            
            char p[2];
            pos pin, pout;
            std::cout << "Please enter the coordinates of the piece you are moving." << std::endl;
            std::cin >> p;
            pin = char2int(p);
            print_pos (pin);
            
            std::cout << "Please enter where the piece is moving to." << std::endl;
            std::cin >> p;
            pout = char2int(p);
            print_pos(pout);
            
            move_piece (pin, pout);
            
            for (const auto& wp : white_pieces) {
                wp->print_info ();
                wp->check_moves ();
            }
            
            print_board ();
            
            ((game_status & 0x01) > 0) ? game_status &= 0xFE : game_status |= 0x01; // toggles status bit 0.
            
            // Debug: break for the while loop.
//            std::cout << static_cast<int>(game_status) << std::endl;
        }
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
    
    void GameEngine :: move_piece (pos pfrom, pos pto) {
        for (const auto& piece : ((game_status & 0x1) > 0 ? white_pieces : black_pieces)) {
            pos temp = piece->check_position();
            if ((pfrom.x == temp.x) and (pfrom.y == temp.y)) {
                piece->move(pto);
            }
            piece->valid_moves.clear();
        }
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
        
        // will work on 'check' case conditions and en-passant rules later.
        is_white ? ++posy : --posy;
        if (b[posx][posy] == 0 and posy != 0 and posy != 7) {
            valid_moves.push_back({posx, posy});
        }
        uint8_t comp = is_white ? 0x80 : 0x40;
        if (posx < 7) {
            if ((b[posx + 1][posy] & 0xC0) == comp) {
                valid_moves.push_back({(++posx)--, posy});
            }
        }
        if (posx > 0) {
            if ((b[posx - 1][posy] & 0xC0) == comp) {
                valid_moves.push_back({(--posx)++, posy});
            }
        }
        if (first_move) {
            if (b[posx][posy] == 0) {
                is_white ? ++posy : --posy;
                if (b[posx][posy] == 0) {
                    valid_moves.push_back({posx, posy});
                }
            }
        }
        for (const auto& move : valid_moves) {
//            std::cout << static_cast<char>(move.x + 'A') << static_cast<int>(move.y + 1) << std::endl;
            print_pos(move);
        }
    }
    
    void Knight :: check_moves (void) {
        const int8_t posx = position.x;
        const int8_t posy = position.y;
        uint8_t** b = *pgb; // put the game board array in the current scope
        
        uint8_t comp = is_white ? 0x40 : 0x80;
        
        // L-shapes: -2x-1y ; -2x+1y ; -1x-2y ; -1x+2y ; +1x-2y ; +1x+2y ; +2x-1y ; +2x+1y
        if (posx > 1) {
            if (posy > 0) {
                if ((b[posx - 2][posy - 1] & 0xC0) != comp) {
                    valid_moves.push_back({static_cast<int8_t>(posx - 2), static_cast<int8_t>(posy - 1)});
                }
            }
            if (posy < 7) {
                if ((b[posx - 2][posy + 1] & 0xC0) != comp) {
                    valid_moves.push_back({static_cast<int8_t>(posx - 2), static_cast<int8_t>(posy + 1)});
                }
            }
        }
        if (posx > 0) {
            if (posy > 1) {
                if ((b[posx - 1][posy - 2] & 0xC0) != comp) {
                    valid_moves.push_back({static_cast<int8_t>(posx - 1), static_cast<int8_t>(posy - 2)});
                }
            }
            if (posy < 6) {
                if ((b[posx - 1][posy + 2] & 0xC0) != comp) {
                    valid_moves.push_back({static_cast<int8_t>(posx - 1), static_cast<int8_t>(posy + 2)});
                }
            }
        }
        if (posx < 6) {
            if (posy > 0) {
                if ((b[posx + 2][posy - 1] & 0xC0) != comp) {
                    valid_moves.push_back({static_cast<int8_t>(posx + 2), static_cast<int8_t>(posy - 1)});
                }
            }
            if (posy < 7) {
                if ((b[posx + 2][posy + 1] & 0xC0) != comp) {
                    valid_moves.push_back({static_cast<int8_t>(posx + 2), static_cast<int8_t>(posy + 1)});
                }
            }
        }
        if (posx < 7) {
            if (posy > 1) {
                if ((b[posx + 1][posy - 2] & 0xC0) != comp) {
                    valid_moves.push_back({static_cast<int8_t>(posx + 1), static_cast<int8_t>(posy - 2)});
                }
            }
            if (posy < 6) {
                if ((b[posx + 1][posy + 2] & 0xC0) != comp) {
                    valid_moves.push_back({static_cast<int8_t>(posx + 1), static_cast<int8_t>(posy + 2)});
                }
            }
        }
        
        for (const auto& move : valid_moves) {
            print_pos(move);
        }
    }
    
    void Bishop :: check_moves (void) {
        int8_t posx = position.x + 1;
        int8_t posy = position.y + 1;
        uint8_t** b = *pgb; // put the game board array in the current scope
        
        uint8_t comp = is_white ? 0x40 : 0x80;
        
        // up-left, up-right, down-left, down-right
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx++, posy++});
            }
        }
        posx = position.x + 1;
        posy = position.y - 1;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx++, posy--});
            }
        }
        posx = position.x - 1;
        posy = position.y + 1;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx--, posy++});
            }
        }
        posx = position.x - 1;
        posy = position.y - 1;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx--, posy--});
            }
        }
        
        for (const auto& move : valid_moves) {
            print_pos(move);
        }
    }
    
    void Rook :: check_moves (void) {
        int8_t posx = position.x + 1;
        int8_t posy = position.y;
        uint8_t** b = *pgb; // put the game board array in the current scope
        
        uint8_t comp = is_white ? 0x40 : 0x80;
        
        // up, down, left, right
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx++, posy});
            }
        }
        posx = position.x - 1;
        posy = position.y;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx--, posy});
            }
        }
        posx = position.x;
        posy = position.y + 1;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx, posy++});
            }
        }
        posx = position.x;
        posy = position.y - 1;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx, posy--});
            }
        }
        for (const auto& move : valid_moves) {
            print_pos(move);
        }
    }
    
    void Queen :: check_moves (void) {
        int8_t posx = position.x + 1;
        int8_t posy = position.y + 1;
        uint8_t** b = *pgb; // put the game board array in the current scope
        
        uint8_t comp = is_white ? 0x40 : 0x80;
        
        // up-left, up-right, down-left, down-right, up, down, left, right
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx++, posy++});
            }
        }
        posx = position.x + 1;
        posy = position.y - 1;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx++, posy--});
            }
        }
        posx = position.x - 1;
        posy = position.y + 1;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx--, posy++});
            }
        }
        posx = position.x - 1;
        posy = position.y - 1;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx--, posy--});
            }
        }
        posx = position.x + 1;
        posy = position.y;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx++, posy});
            }
        }
        posx = position.x - 1;
        posy = position.y;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx--, posy});
            }
        }
        posx = position.x;
        posy = position.y + 1;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx, posy++});
            }
        }
        posx = position.x;
        posy = position.y - 1;
        while (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) == comp) {
                break;
            } else if (b[posx][posy] > 0) {
                valid_moves.push_back({posx, posy});
                break;
            } else {
                valid_moves.push_back({posx, posy--});
            }
        }
        
        for (const auto& move : valid_moves) {
            print_pos(move);
        }
    }
    
    void King :: check_moves (void) {
        
        int8_t posx = position.x + 1;
        int8_t posy = position.y + 1;
        uint8_t** b = *pgb; // put the game board array in the current scope
        
        uint8_t comp = is_white ? 0x40 : 0x80;
        
        // up-left, up-right, down-left, down-right, up, down, left, right
        if (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) != comp) {
                valid_moves.push_back({posx--, posy});
            }
        }
        if (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) != comp) {
                valid_moves.push_back({posx--, posy});
            }
        }
        if (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) != comp) {
                valid_moves.push_back({posx, posy--});
            }
        }
        if (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) != comp) {
                valid_moves.push_back({posx, posy--});
            }
        }
        if (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) != comp) {
                valid_moves.push_back({posx++, posy});
            }
        }
        if (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) != comp) {
                valid_moves.push_back({posx++, posy});
            }
        }
        if (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) != comp) {
                valid_moves.push_back({posx, posy++});
            }
        }
        if (posx >= 0 and posy >= 0 and posx < 8 and posy < 8) {
            if ((b[posx][posy] & 0xC0) != comp) {
                valid_moves.push_back({posx, posy});
            }
        }
        for (const auto& move : valid_moves) {
            print_pos(move);
        }
    }
    
    void Piece :: move (pos p) {
        // the default function for moving. Exceptions only for p and K.
        // a failure case needs to be added.
        uint8_t** b = *pgb;
        bool valid = false;
        for (const auto& m : valid_moves) {
            if (m.x == p.x and m.y == p.y) {
                valid = true;
            }
        }
        if (valid) {
            uint8_t temp = b[position.x][position.y];
            b[position.x][position.y] = 0;
            position = p;
            b[position.x][position.y] = temp;
        }
    }
    
    void Pawn :: move (pos p) {
        // if first move occurs, it must be set to false.
        uint8_t** b = *pgb;
        bool valid = false;
        for (const auto& m : valid_moves) {
            if (m.x == p.x and m.y == p.y) {
                valid = true;
            }
        }
        if (valid) {
            first_move = false;
            uint8_t temp = b[position.x][position.y];
            b[position.x][position.y] = 0;
            position = p;
            b[position.x][position.y] = temp;
        }
    }

    void King :: move (pos p) {
        // an exception case is made for the castling move (not yet implemented).
        uint8_t** b = *pgb;
        bool valid = false;
        for (const auto& m : valid_moves) {
            if (m.x == p.x and m.y == p.y) {
                valid = true;
            }
        }
        if (valid) {
            uint8_t temp = b[position.x][position.y];
            b[position.x][position.y] = 0;
            position = p;
            b[position.x][position.y] = temp;
        }
    }
}

#endif
