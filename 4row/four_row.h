#ifndef FOUR_ROW
#define FOUR_ROW

#include <array>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#include "matrix.hh"

class Board
{
public:
    enum Mark {
        Mine = 1,
        None = 0,
        Opp = -1,
    };

    using T = int;

    Board(const std::string&); // parse new Board
    Board(const Board&, Mark, const std::vector<int>&); // placing many markers, starting with Mark type
    void print() const;
    int evaluate() const;
    std::vector<int> pmoves() const;
    
private:
    static const int ROWS = 6;
    static const int COLS = 7;
    Matrix<COLS, ROWS, T> mPos;
    std::array<int, COLS> mHeight; 
};

struct Minmax
{
    using Mark = Board::Mark;

    int depth = 6;
    int maxiter = 500000;

    Minmax(const Board&, Mark);
    int bestMove() const { 
        if (mBest && mBest->moves.size() > 0) return mBest->moves[0];
        return 3;
    }
    void print() const;

private:
    struct Node
    {
        int score;
        int d;
        bool max; // maximize or minimize
        bool up;
        Node* p;
        std::vector<int> moves;
        friend std::ostream& operator<<(std::ostream& os, const Node& n) 
        {
            std::cout << (n.max ? "+" : "-") << "d: " << n.d << ", s: " << n.score << std::endl;
            std::cout << "mov: ";
            for (int m : n.moves) std::cout << m << " ";
            return os;
        }
    };

    int mSearched{0};
    int mPushed{0};
    int mPruned{0};
    int mIter{0};
    std::shared_ptr<Node> mBest;
};

#endif
