/* Define Classes and Structs */
namespace chess {
    
    struct position {
        char x;
        char y;
    };
    
    // The pieces on the board
    class Piece {
    public:
        bool taken;
        position pos;
        std::string piece_type;
        Piece (bool taken, position pos, std::string piece_type) : taken(taken), pos(pos), piece_type(piece_type) {};
    protected:
    private:
    };
    
    // The game players
    class Player {
    public:
        std::vector<const Piece*> pieces;
        void is_white (bool wb) { white = wb; };
        bool check_colour () { return white; };
        std::string colour () { return white ? "White" : "Black"; };
        void add_piece (const Piece* cp) { pieces.push_back(cp); };
        void print_pieces (void) {
            for (auto& cp : pieces) {
                std::cout << colour() << " " << cp->piece_type << " at position " << cp->pos.x << cp->pos.y << "." << std::endl;
            }
            std::cout << std::endl;
        }
    protected:
        bool white;
    private:
    };
    
    // Player 1, extends Class Player
    class Player_One : public Player {
    public:
    protected:
    private:
    };
    
    // Player 2, extends Class Player
    class Player_Two : public Player {
    public:
        // Check if the player is a computer
        void is_computer (bool hc) { computer = hc; };
    protected:
        bool computer;
    private:
    };
    
    // The game itself
    class Game {
    public:
        void setup (Player_One& p1, Player_Two& p2) {
            // Add the Pawns to the Board
            for (char it = 'A'; it <= 'H'; ++it) {
                Piece* wp = new Piece (false, {it, '2'}, "Pawn");
                Piece* bp = new Piece (false, {it, '7'}, "Pawn");
                if (p1.check_colour()) {
                    p1.add_piece (wp); // Add White Piece to p1
                    p2.add_piece (bp); // Add Black Piece to p2
                } else {
                    p1.add_piece (bp); // Add Black Piece to p1
                    p2.add_piece (wp); // Add White Piece to p2
                }
            }
            // Add the Rooks, Knights and Bishops the Board
            char its[2] = {'A', 'H'};
            std::string rkb[3] = {"Rook", "Knight", "Bishop"};
            for (auto& s : rkb) {
                for (auto& it : its) {
                    Piece* wp = new Piece (false, {it, '1'}, s);
                    Piece* bp = new Piece (false, {it, '8'}, s);
                    if (p1.check_colour()) {
                        p1.add_piece (wp); // Add White Piece to p1
                        p2.add_piece (bp); // Add Black Piece to p2
                    } else {
                        p1.add_piece (bp); // Add Black Piece to p1
                        p2.add_piece (wp); // Add White Piece to p2
                    }
                }
                its[0]++;
                its[1]--;
            }
            // Add the Queen and the King to the Board
            std::string qk[2] = {"Queen", "King"};
            for (auto& s : qk) {
                Piece* wp = new Piece (false, {its[0], '1'}, s);
                Piece* bp = new Piece (false, {its[0], '8'}, s);
                if (p1.check_colour()) {
                    p1.add_piece (wp); // Add White Piece to p1
                    p2.add_piece (bp); // Add Black Piece to p2
                } else {
                    p1.add_piece (bp); // Add Black Piece to p1
                    p2.add_piece (wp); // Add White Piece to p2
                }
                its[0]++;
            }
        }
    protected:
    private:
    };
    
    // A Board Cell
    class Square {
    public:
        position coordinates () { return cell_num; };
        char colour () { return white ? 'W' : 'B'; };
        void is_occupied (const Player_One& p1, const Player_Two& p2) {
            for (auto& cp : p1.pieces) {
                if (cp->pos.x == cell_num.x && cp->pos.y == cell_num.y) {
                    occupied = true;
                    return;
                }
            }
            for (auto& cp : p2.pieces) {
                if (cp->pos.x == cell_num.x && cp->pos.y == cell_num.y) {
                    occupied = true;
                    return;
                }
            }
            occupied = false;
        }
        Square (position cell_num, bool occupied, bool white) : cell_num (cell_num), occupied (occupied), white (white) {};
    protected:
        bool occupied;
        position cell_num;
    private:
        bool white;
    };
    
    // The Chess Board
    class Board {
    public:
        std::vector<Square*> board;
        void board_init (void) {
            bool is_sq_white = true;
            for (char it1 = '8'; it1 >= '1'; --it1) {
                for (char it2 = 'A'; it2 <= 'H'; ++it2) {
                    Square* sq = new Square ({it2,it1}, (it1 <= '2' || it1 >= '7'), is_sq_white);
                    board.push_back(sq);
                    is_sq_white = !is_sq_white;
                }
                is_sq_white = !is_sq_white;
            }
        }
        void print_board (void) {
            for (auto& sq : board) {
                position p = sq->coordinates();
                std::cout << p.x << p.y << '{' << sq->colour() << '}';
                p.x == 'H' ? std::cout << std::endl : std::cout << ' ';
            }
            std::cout << std::endl;
        }
    protected:
    private:
    };
};

