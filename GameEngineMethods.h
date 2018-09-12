#ifndef GAMEENGINEMETHODS_H
#define GAMEENGINEMETHODS_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "ChessIncludes.h" // for function rm_dlt ();
#include "ChessClasses.h" // for GameEngine definition

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
            place_pawns (colour, row);
            colour ? --row : ++row;
            place_knights (colour, row);
            place_bishops (colour, row);
            place_rooks (colour, row);
            place_royals (colour, row);
            colour = !colour;
            row = 6;
        } while (!colour);
        for (const auto& wp : white_pieces) { wp->check_moves (); }
        print_board ();
        play_game ();
    }
    
    GameEngine :: ~GameEngine (void) {
        for (uint8_t i = 0; i < 8; ++i) {
            delete board[i];
            board[i] = nullptr;
        }
        delete board;
        board = nullptr;
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
            pos pin, pout;
            std::cout << "Please enter the coordinates of the piece you are moving." << std::endl;
            std::cin >> p;
            pin = char2int(p);
            if ((board[pin.x][pin.y] & ((game_status & 0x01) == 0 ? 0x80 : 0x40)) == 0) {
                std::cout << "No " << ((game_status & 0x01) == 0 ? "Black" : "White") << " piece on this square. Please try again." << std::endl;
            } else do {
                std::cout << "Please enter where the piece is moving to." << std::endl;
                std::cin >> p;
                pout = char2int(p);
                if (move_piece (pin, pout)) {
                    print_board ();
                    allowed = true;
                    ((game_status & 0x01) > 0) ? game_status &= 0xFE : game_status |= 0x01; // toggles status bit 0.
                } else { std::cout << "Not a valid move. Please try again." << std::endl; }
                counter = 0;
                for (const auto& piece : ((game_status & 0x01) > 0) ? white_pieces : black_pieces) {
//                    piece->print_info ();
                    piece->check_moves ();
                    for (const auto& move : piece->valid_moves) {
                        ++counter;
                    }
                }
                // check if valid moves exist:
                if (counter == 0) { game_status |= 0x80; }
                // check if in 'Check':
                
                // if both true then in Checkmate:
                
            } while (!allowed);
        }
    }
    
    void GameEngine :: place_pawns (const bool c, const int8_t r) {
        for (int8_t i = 0; i < 8; ++i) {
            PawnPtr pawn (new Pawn (c, {i, r}, game_status, board));
            pawn->is_first_move(true);
            c ? white_pieces.push_back(std::move(pawn)) : black_pieces.push_back(std::move(pawn));
            board[i][r] |= c ? 0x41 : 0x81;
            pawn = nullptr;
        }
    }
    
    void GameEngine :: place_knights (const bool c, const int8_t r) {
        for (int8_t i = 1; i < 8; i += 5) {
            KnightPtr knight (new Knight (c, {i, r}, game_status, board));
            c ? white_pieces.push_back(std::move(knight)) : black_pieces.push_back(std::move(knight));
            board[i][r] |= c ? 0x42 : 0x82;
            knight = nullptr;
        }
    }
    
    void GameEngine :: place_bishops (const bool c, const int8_t r) {
        for (int8_t i = 2; i < 8; i += 3) {
            BishopPtr bishop (new Bishop (c, {i, r}, game_status, board));
            c ? white_pieces.push_back(std::move(bishop)) : black_pieces.push_back(std::move(bishop));
            board[i][r] |= c ? 0x44 : 0x84;
            bishop = nullptr;
        }
    }
    
    void GameEngine :: place_rooks (const bool c, const int8_t r) {
        for (int8_t i = 0; i < 8; i+=7) {
            RookPtr rook (new Rook (c, {i, r}, game_status, board));
            c ? white_pieces.push_back(std::move(rook)) : black_pieces.push_back(std::move(rook));
            board[i][r] |= c ? 0x48 : 0x88;
            rook = nullptr;
        }
    }
    
    void GameEngine :: place_royals (const bool c, const int8_t r) {
        QueenPtr queen (new Queen (c, {3, r}, game_status, board));
        c ? white_pieces.push_back(std::move(queen)) : black_pieces.push_back(std::move(queen));
        board[3][r] |= c ? 0x50 : 0x90;
        KingPtr king (new King (c, {4, r}, game_status, board));
        c ? white_pieces.push_back(std::move(king)) : black_pieces.push_back(std::move(king));
        board[4][r] |= c ? 0x60 : 0xA0;
        queen = nullptr;
        king = nullptr;
    }
    
    bool GameEngine :: move_piece (pos pfrom, pos pto) {
        bool r = false;
        uint8_t pr;
        for (const auto& piece : ((game_status & 0x1) > 0 ? white_pieces : black_pieces)) {
            pos temp = piece->check_position();
            if ((pfrom.x == temp.x) and (pfrom.y == temp.y)) {
                pr = piece->move(pto);
                if ((pr & 0x01) > 0) r = true;
                pr >>= 1;
                if (pr > 0) { // promotion condition for pawns
                    bool c = (game_status & 0x1) > 0;
                    rm_dlt ((c ? white_pieces : black_pieces), pto); // delete the pawn
                    switch (pr) {
                        case 0x01: {
                            board[pto.x][pto.y] = (c ? 0x42 : 0x82);
                            KnightPtr knight (new Knight (is_white, pto, game_status, board));
                            c ? white_pieces.push_back(std::move(knight)) : black_pieces.push_back(std::move(knight));
                            knight = nullptr;
                            break;
                        }
                        case 0x02: {
                            board[pto.x][pto.y] = (c ? 0x44 : 0x84);
                            BishopPtr bishop (new Bishop (is_white, pto, game_status, board));
                            c ? white_pieces.push_back(std::move(bishop)) : black_pieces.push_back(std::move(bishop));
                            bishop = nullptr;
                            break;
                        }
                        case 0x04: {
                            board[pto.x][pto.y] = (c ? 0x48 : 0x88);
                            RookPtr rook (new Rook (is_white, pto, game_status, board));
                            c ? white_pieces.push_back(std::move(rook)) : black_pieces.push_back(std::move(rook));
                            rook = nullptr;
                            break;
                        }
                        case 0x08: {
                            board[pto.x][pto.y] = (c ? 0x50 : 0x90);
                            QueenPtr queen (new Queen (is_white, pto, game_status, board));
                            c ? white_pieces.push_back(std::move(queen)) : black_pieces.push_back(std::move(queen));
                            queen = nullptr;
                            break;
                        }
                    }
                }
            }
            piece->valid_moves.clear();
        }
        rm_dlt (((game_status & 0x1) == 0 ? white_pieces : black_pieces), pto);
        return r;
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
