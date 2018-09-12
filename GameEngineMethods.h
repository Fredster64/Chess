#ifndef GAMEENGINEMETHODS_H
#define GAMEENGINEMETHODS_H

#include <iostream>
#include <vector>
#include <string>

namespace chess {
    
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
}

#endif