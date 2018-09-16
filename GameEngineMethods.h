#ifndef GAMEENGINEMETHODS_H
#define GAMEENGINEMETHODS_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "ChessIncludes.h"
#include "ChessClasses.h"

namespace chess {
    
    GameEngine :: GameEngine (const bool player_colour) {
        is_white = player_colour;
        game_status = 0x01; // normal, white to move first.
        /* Set up the game board */
        board = new uint8_t*[8];
        for (int8_t i = 0; i < 8; ++i) {
            board[i] = new uint8_t[8];
            for (int8_t j = 0; j < 8; ++j) { board[i][j] = 0; }
        }
        bool colour = true;
        int8_t row = 1;
        do {
            this->place_pawns (colour, row);
            colour ? --row : ++row;
            this->place_knights (colour, row);
            this->place_bishops (colour, row);
            this->place_rooks (colour, row);
            this->place_royals (colour, row);
            colour = !colour;
            row = 6;
        } while (!colour);
        for (const auto& wp : white_pieces) { wp->check_moves (wp->valid_moves); }
        print_board ();
        play_game ();
    }
    
    GameEngine :: ~GameEngine (void) {
        for (uint8_t i = 0; i < 8; ++i) {
            board[i] = nullptr;
            delete board[i];
        }
        board = nullptr;
        delete board;
        for (auto& p : white_pieces) { p = nullptr; }
        white_pieces.clear();
        for (auto& p : black_pieces) { p = nullptr; }
        black_pieces.clear();
    }
    
    void GameEngine :: play_game (void) {
        bool allowed;
        uint8_t counter;
        while ((game_status & 0xC0) == 0) {
            allowed = false;
            char p[2];
            Pos pin, pout;
            std::cout << "Please enter the coordinates of the piece you are moving." << std::endl;
            std::cin >> p;
            pin = char2int(p);
            if ((board[pin.x][pin.y] & ((game_status & 0x01) == 0 ? 0x80 : 0x40)) == 0) {
                std::cout << "No " << ((game_status & 0x01) == 0 ? "Black" : "White") << " piece on this square. Please try again." << std::endl;
            } else do {
                std::cout << "Please enter where the piece is moving to." << std::endl;
                std::cin >> p;
                pout = this->char2int(p);
                if (move_piece (pin, pout)) {
                    print_board ();
                    allowed = true;
                    ((game_status & 0x01) > 0) ? game_status &= 0xFE : game_status |= 0x01; // toggles status bit 0.
                } else { std::cout << "Not a valid move. Please try again." << std::endl; }
                counter = 0;
                for (const auto& piece : ((game_status & 0x01) > 0) ? white_pieces : black_pieces) {
//                    piece->print_info ();
                    piece->check_moves (piece->valid_moves, true);
                    for (const auto& move : piece->valid_moves) {
                        ++counter;
                    }
                }
            } while (!allowed);
            
            lm.print_lm();
            
            // check if valid moves exist:
            if (counter == 0) { game_status |= 0x80; }
            // check if in 'Check':
            if (in_check((game_status & 0x01) > 0)) {
                game_status |= 0x20;
                std::cout << "Check." << std::endl;
            } else {
                game_status &= 0xDF;
            }
            // if both true then in Checkmate:
            if ((game_status & 0xA0) == 0xA0) { game_status |= 0x40; }
        }
    }
    
    void GameEngine :: place_pawns (const bool c, const int8_t r) {
        for (int8_t i = 0; i < 8; ++i) {
            PawnPtr pawn (new Pawn (c, {i, r}, lm, board));
            this->pb_ptr (white_pieces, black_pieces, pawn, c);
            board[i][r] |= c ? 0x41 : 0x81;
        }
    }
    
    void GameEngine :: place_knights (const bool c, const int8_t r) {
        for (int8_t i = 1; i < 8; i += 5) {
            KnightPtr knight (new Knight (c, {i, r}, lm, board));
            this->pb_ptr (white_pieces, black_pieces, knight, c);
            board[i][r] |= c ? 0x42 : 0x82;
        }
    }
    
    void GameEngine :: place_bishops (const bool c, const int8_t r) {
        for (int8_t i = 2; i < 8; i += 3) {
            BishopPtr bishop (new Bishop (c, {i, r}, lm, board));
            this->pb_ptr (white_pieces, black_pieces, bishop, c);
            board[i][r] |= c ? 0x44 : 0x84;
        }
    }
    
    void GameEngine :: place_rooks (const bool c, const int8_t r) {
        for (int8_t i = 0; i < 8; i+=7) {
            RookPtr rook (new Rook (c, {i, r}, lm, board));
            this->pb_ptr (white_pieces, black_pieces, rook, c);
            board[i][r] |= c ? 0x48 : 0x88;
        }
    }
    
    void GameEngine :: place_royals (const bool c, const int8_t r) {
        QueenPtr queen (new Queen (c, {3, r}, lm, board));
        this->pb_ptr (white_pieces, black_pieces, queen, c);
        board[3][r] |= c ? 0x50 : 0x90;
        KingPtr king (new King (c, {4, r}, lm, board));
        this->pb_ptr (white_pieces, black_pieces, king, c);
        board[4][r] |= c ? 0x60 : 0xA0;
    }
    
    bool GameEngine :: move_piece (Pos pfrom, Pos pto) {
        bool r = false;
        uint8_t pr;
        for (const auto& piece : ((game_status & 0x1) > 0 ? white_pieces : black_pieces)) {
            Pos temp = piece->check_position();
            if ((pfrom.x == temp.x) and (pfrom.y == temp.y)) { pr = piece->move(pto); }
            piece->valid_moves.clear();
        }
        if ((pr & 0x01) > 0) { r = true; }
        pr >>= 1;
        if (pr > 0) { // promotion condition for pawns
            bool c = (game_status & 0x1) > 0;
            this->rm_dlt ((c ? white_pieces : black_pieces), pto); // delete the pawn
            switch (pr) {
                case 0x01: {
                    KnightPtr knight (new Knight (is_white, pto, lm, board));
                    board[pto.x][pto.y] = (c ? 0x42 : 0x82);
                    this->pb_ptr (white_pieces, black_pieces, knight, c);
                    break;
                }
                case 0x02: {
                    BishopPtr bishop (new Bishop (is_white, pto, lm, board));
                    board[pto.x][pto.y] = (c ? 0x44 : 0x84);
                    this->pb_ptr (white_pieces, black_pieces, bishop, c);
                    break;
                }
                case 0x04: {
                    RookPtr rook (new Rook (is_white, pto, lm, board));
                    board[pto.x][pto.y] = (c ? 0x48 : 0x88);
                    this->pb_ptr (white_pieces, black_pieces, rook, c);
                    break;
                }
                case 0x08: {
                    QueenPtr queen (new Queen (is_white, pto, lm, board));
                    board[pto.x][pto.y] = (c ? 0x50 : 0x90);
                    this->pb_ptr (white_pieces, black_pieces, queen, c);
                    break;
                }
            }
        }
        // delete pieces of the opposite colour if taken
        this->rm_dlt (((game_status & 0x1) == 0 ? white_pieces : black_pieces), pto);
        return r;
    }
    
    bool GameEngine :: in_check (bool c) {
        // find all cells that the king cannot exist in. If the current cell is in the list, then the king is in check.
        uint8_t k_score = c ? 0x60 : 0xA0;
        uint8_t comp = c ? 0x40 : 0x80;
        Pos k_pos;
        std::vector<Pos> vec;
        // search for the King's cell
        for (int8_t j = 0; j < 8; ++j) {
            for (int8_t i = 0; i < 8; ++i) {
                if ((board[i][j] & k_score) == k_score) {
                    k_pos = {i, j};
                    break;
                }
            }
        }
        // look for all cells that can be attacked by the opponent
        for (const auto& piece : (c ? black_pieces : white_pieces)) { piece->check_moves (vec, false); }
        rm_dupes<Pos>(vec);
        for (auto& cell : vec) { cell.print_pos(); }
        
        // check if the k_pos is in v.
        return vec_search<Pos>(vec, k_pos);
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
    
}

#endif
