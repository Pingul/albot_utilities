#ifndef BATTLESHIPS
#define BATTLESHIPS

#include "matrix.hh"
#include <string>
#include <random>
#include <iostream>

namespace battleship {

const auto SHIP_SCOUT = Matrix<1, 2>({1, 1});
const auto SHIP_TRANSPORT = Matrix<1, 3>({2, 2, 2});
const auto SHIP_SUBMARINE = Matrix<1, 3>({3, 3, 3});
const auto SHIP_BATTLESHIP = Matrix<1, 4>({4, 4, 4, 4});
const auto SHIP_CARRIER = Matrix<1, 5>({5, 5, 5, 5, 5});

struct Point { 
    int x, y; 
    void operator+=(const Point& p) { x += p.x; y += p.y; }
    friend std::ostream& operator<<(std::ostream& os, const Point& p) 
    { 
        os << "(" << p.x << ", " << p.y << ")"; 
        return os;
    }
};
//Point operator+(const Point& p1, const Point& p2) { return Point{p1.x + p2.x, p1.y + p2.y}; }

class Board {
public:
    static const int XDIM = 10;
    static const int YDIM = 10;

    enum class Piece : int {
        Empty,
        Scout,
        Transport,
        Submarine,
        Battleship,
        Carrier,
        Hit,
        Sunken,
        Hidden,
    };

    Board() = default;
    Board(const std::string&);

    void print() const { mRep.print(); }
    std::string asString() const;

    // returns true if the ship was placed, false otherwise
    template <typename Matrix>
    bool placeShip(Piece shipType, Matrix ship, int x, int y)
    {
        int SX = Matrix::X + x;
        int SY = Matrix::Y + y;
        if (SX > XDIM ||
            SY > YDIM) 
            return false;

        // Is the space free?
        for (int i = x; i < SX; ++i) {
            for (int j = y; j < SY; ++j) {
                if (static_cast<Piece>(mRep.at(i, j)) != Piece::Empty)
                    return false;
            }
        }

        // Lets place the ship
        for (int i = x; i < SX; ++i) {
            for (int j = y; j < SY; ++j) {
                mRep.at(i, j) = int(shipType);
            }
        }
        return true;
    }

    bool is(Point p, Piece state) const; 
    int at(Point p) { return mRep.at(p.x, p.y); }
    bool inside(Point p) const { return p.x < XDIM && p.y < YDIM; }

private:
    Matrix<XDIM, YDIM, int> mRep;

};

class Logic {
public:
    enum class State : int {
        InitBoats,
        Search,
        Destroy,
        GameOver,
    };

    Logic() 
        : mState(State::InitBoats), 
          mBoard(),
          mRandDevice(), 
          mGenerator(mRandDevice()) ,
          mDist(0, Board::XDIM)
    {}

    void parseBoardState(const std::string&);
    std::string nextAction();
    void print() const;
    void reset() { mState = State::InitBoats; mBoard = Board(); }

private:
    Board initBoats();
    Point search();
    Point destroy();
    int coordinate(const Point& c) { return c.x*Board::YDIM + c.y; }

    int nextRandom() { return mDist(mGenerator); }

    Board mBoard;

    // Keeping the state
    State mState;
    Point mLast, mHit;

    std::random_device mRandDevice;
    std::mt19937 mGenerator;
    std::uniform_int_distribution<> mDist;
};

}; // namespace battleship

#endif
