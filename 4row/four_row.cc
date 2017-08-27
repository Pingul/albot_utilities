#include "four_row.h"

#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <list>
#include <stack>
#include <limits>

constexpr int score_max = 1000;
constexpr int score_min = -score_max;

Board::Board(const std::string& str)
    : mPos(), mHeight({0})
{
    std::stringstream ss(str);
    for (int c = 0; c < COLS; ++c) {
        bool hset = false;
        for (int r = 0; r < ROWS; ++r) {
            ss >> mPos.at(c, r);
            if (!hset && (mPos.at(c, r) != None)) {
                mHeight[c] = r - 1; // setting height to the next allowed spot
                hset = true;
            }
        }
        if (!hset) mHeight[c] = ROWS - 1;
    }
};

Board::Board(const Board& b, Mark m, const std::vector<int>& cols)
    : mPos(b.mPos), mHeight(b.mHeight)
{
    for (int c : cols) {
        if (mHeight[c] < 0)
            throw std::runtime_error("column is full");
        mPos.at(c, mHeight[c]--) = m;
        m = Mark(-int(m));
    }
}

void Board::print() const
{
    mPos.print();
    std::cout << "---" << std::endl;
    for (int c : mHeight)
        std::cout << std::setw(3) << c << " ";
    std::cout << "\n---" << std::endl;
    std::cout << "score : " << evaluate() << std::endl;
}

int Board::evaluate() const
{
    int score = 0;

    Matrix<1, 4> rt({1, 1, 1, 1});
    auto c_rt = mPos.convolve(rt);
    for (auto it = c_rt.iterator(); it(); ++it) {
        if (*it == 4) return score_max;
        else if (*it == -4) return score_min;
        else score += *it;
    }

    Matrix<4, 1> ct({1, 1, 1, 1});
    auto c_ct = mPos.convolve(ct);
    for (auto it = c_ct.iterator(); it(); ++it) {
        if (*it == 4) return score_max;
        else if (*it == -4) return score_min;
        else score += *it;
    }

    Matrix<4, 4> d1t({1, 0, 0, 0,
                      0, 1, 0, 0,
                      0, 0, 1, 0,
                      0, 0, 0, 1});
    auto c_d1t = mPos.convolve(d1t);
    for (auto it = c_d1t.iterator(); it(); ++it) {
        if (*it == 4) return score_max;
        else if (*it == -4) return score_min;
        else score += *it;
    }

    Matrix<4, 4> d2t({0, 0, 0, 1,
                      0, 0, 1, 0,
                      0, 1, 0, 0,
                      1, 0, 0, 0});
    auto c_d2t = mPos.convolve(d2t);
    for (auto it = c_d2t.iterator(); it(); ++it) {
        if (*it == 4) return score_max;
        else if (*it == -4) return score_min;
        else score += *it;
    }

    return score;
}


std::vector<int> Board::pmoves() const
{
    std::vector<int> m;
    for (int c = 0; c < COLS; ++c)
        if (mHeight[c] > -1) m.emplace_back(c);
    return m;
}

std::vector<int> operator+(const std::vector<int>& v, int i)
{
    std::vector<int> vv(v);
    vv.emplace_back(i);
    return vv;
}

template <typename T> T max(T a, T b) { return a > b ? a : b; }
template <typename T> T min(T a, T b) { return a < b ? a : b; }

Minmax::Minmax(const Board& b, Mark m)
{
    Node* root = new Node{0, 0, m == Mark::Mine, false, nullptr, {}};
    std::stack<Node*> stack;
    stack.push(root);
    
    int alpha = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();
    do {
        Node* c = stack.top();
        if (c->up) {
            if (!c->p->up ||
                (c->p->max && (c->p->score < c->score)) ||
               (!c->p->max && (c->p->score > c->score)) ||
               ((c->p->score == c->score) && (c->p->d > c->d))) { 
                    c->p->score = c->score;
                    c->p->moves = c->moves;
                    // Depth is now depth for best score
                    c->p->d = c->d;
            } 
            
            //if (!c->p->up) {
                //// just update
            //} else if (c->p->max && (c->p->score < c->score)) {
                //alpha = max(c->score, alpha);
            //} else if (!c->p->max && (c->p->score > c->score)) {
                //beta = min(c->score, beta);
            //} else {
                //goto skipupdate;
            //}
            //c->p->score = c->score;
            //c->p->moves = c->moves;
            //skipupdate:

            c->p->up = true;
            stack.pop();
            delete c;
            ++mSearched;
        } else {
            //search deeper
            Board cb(b, m, c->moves);
            for (int mov : cb.pmoves()) {
                Node* n = new Node{0, c->d + 1, !c->max, false, c, c->moves + mov};
                Board nb(b, m, n->moves);
                n->score = nb.evaluate();

                //if ((c->max && (n->score < beta)) ||
                   //(!c->max && (n->score > alpha))) {
                    //++mPruned;
                    //delete n;
                    //break;
                //}

                if (n->d == depth || (std::abs(n->score) == score_max)) { 
                    // we're in a leaf
                    n->up = true;
                }

                stack.push(n);
                ++mPushed;
            }
        }
        
        if (++mIter > maxiter) {
            print();
            throw std::runtime_error("maxiter reached"); // memory leak
        }
    } while (stack.size() > 1);
    // the last element of the stack should contain the moves we want
    if (stack.size() != 1) 
        throw std::runtime_error("something weird happened with Minmax -- not 1 element left");
    mBest.reset(stack.top()); // takes responsibility of the memory
}

void Minmax::print() const 
{
    std::cout << "---" << std::endl;
    std::cout << "Searched: " << mSearched << std::endl;
    std::cout << "Pushed:   " << mPushed << std::endl;
    std::cout << "Pruned:   " << mPruned << std::endl;
    std::cout << "Iterations: " << mIter << std::endl;
    std::cout << "Best moves ";
    for (int m : mBest->moves)
        std::cout << m << " ";
    std::cout << std::endl;
    std::cout << "Predicted score: " << mBest->score << std::endl;
}

