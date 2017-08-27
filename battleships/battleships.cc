#include "battleships.hh"
#include <sstream>
#include <vector>

namespace battleship {

Board::Board(const std::string& inp) 
{
    std::array<int, XDIM*YDIM> data;
    std::transform(inp.begin(), inp.end(), data.begin(), [=](char c) {
            switch (c) {
                case '0': return int(Piece::Hidden); break;
                case '-': return int(Piece::Empty); break;
                case 'X': return int(Piece::Hit); break;
                case 'S': return int(Piece::Sunken); break;
                default:
                    throw std::runtime_error("Unknown char '" + std::to_string(c));

            }
    });
    mRep.reset(data);
}

std::string Board::asString() const
{
    std::stringstream ss;
    for (int x = 0; x < XDIM; ++x) {
        for (int y = 0; y < YDIM; ++y) {
            ss << std::to_string(mRep.at(x, y));
        }
    }
    return ss.str();
}

bool Board::is(Point p, Piece state) const
{
    return static_cast<Piece>(mRep.at(p.x, p.y)) == state;
}

void Logic::parseBoardState(const std::string& serverMessage)
{
    if (serverMessage == "GameOver")
        mState = State::GameOver;

    switch (mState) {
        case State::Search:
            mBoard = Board(serverMessage);
            if (mBoard.is(mLast, Board::Piece::Hit)) {
                mHit = mLast;
                mState = State::Destroy;
            }
            return;
        case State::Destroy:
            mBoard = Board(serverMessage);
            if (mBoard.is(mLast, Board::Piece::Sunken)) {
                mState = State::Search;
            }
            return;
        case State::InitBoats: 
        case State::GameOver:
            return;
    }
}

std::string Logic::nextAction() 
{
    Point p;
    switch (mState) {
        case State::InitBoats:
            mBoard = initBoats();
            mState = State::Search;
            return mBoard.asString();
        case State::Search:
            p = search();
            break;
        case State::Destroy:
            p = destroy();
            break;
        case State::GameOver:
        default:
            return "";
    }
    mLast = p;
    return std::to_string(coordinate(p));
}

void Logic::print() const 
{
    std::cout << "LOGIC" << std::endl;
    mBoard.print();
    std::cout << "State : " << int(mState) << std::endl;
    std::cout << "mLast : " << mLast << std::endl;
    std::cout << "mHit : " << mHit << std::endl;
};

Board Logic::initBoats()
{
    Board b;

    int x, y;
    bool flip, success;
    do {
        x = nextRandom();
        y = nextRandom();
        flip = nextRandom() < 5;
        if (flip)
            success = b.placeShip(Board::Piece::Scout, SHIP_SCOUT.transpose(), x, y);
        else
            success = b.placeShip(Board::Piece::Scout, SHIP_SCOUT, x, y);
    } while (!success);

    do {
        x = nextRandom();
        y = nextRandom();
        flip = nextRandom() < 5;
        if (flip)
            success = b.placeShip(Board::Piece::Transport, SHIP_TRANSPORT.transpose(), x, y);
        else
            success = b.placeShip(Board::Piece::Transport, SHIP_TRANSPORT, x, y);
    } while (!success);


    do {
        x = nextRandom();
        y = nextRandom();
        flip = nextRandom() < 5;
        if (flip)
            success = b.placeShip(Board::Piece::Submarine, SHIP_SUBMARINE.transpose(), x, y);
        else
            success = b.placeShip(Board::Piece::Submarine, SHIP_SUBMARINE, x, y);
    } while (!success);


    do {
        x = nextRandom();
        y = nextRandom();
        flip = nextRandom() < 5;
        if (flip)
            success = b.placeShip(Board::Piece::Battleship, SHIP_BATTLESHIP.transpose(), x, y);
        else
            success = b.placeShip(Board::Piece::Battleship, SHIP_BATTLESHIP, x, y);
    } while (!success);


    do {
        x = nextRandom();
        y = nextRandom();
        flip = nextRandom() < 5;
        if (flip)
            success = b.placeShip(Board::Piece::Carrier, SHIP_CARRIER.transpose(), x, y);
        else
            success = b.placeShip(Board::Piece::Carrier, SHIP_CARRIER, x, y);
    } while (!success);

    return b;
}

Point Logic::search()
{
    Point guess;
    do {
        guess.x = nextRandom();
        guess.y = nextRandom();
    } while (!mBoard.is(guess, Board::Piece::Hidden));
    return guess;
}

Point Logic::destroy()
{
    std::vector<Point> directions{
        Point{-1, 0},
        Point{1, 0},
        Point{0, -1},
        Point{0, 1}
    };
        
    for (auto& dir : directions) {
        Point p = mHit;
        while (mBoard.inside(p) && mBoard.is(p, Board::Piece::Hit))
            p += dir;
        
        std::cout << "Searching : " << p << std::endl;
        std::cout << "At : " << mBoard.at(p) << std::endl;

        if (!mBoard.inside(p)) continue; // we reached an edge
        else if (mBoard.is(p, Board::Piece::Empty)) continue; // We've searched in this direction before, and we did not hit anything
        else if (mBoard.is(p, Board::Piece::Hidden)) return p;
        else
            std::cout << "Found " << mBoard.at(p) << std::endl;
}
    throw std::runtime_error("Should not reach this code");
}

}; // namespace battleship
