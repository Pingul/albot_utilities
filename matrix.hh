#ifndef BOARD_MATRIX
#define BOARD_MATRIX

#include <iostream>
#include <iomanip>
#include <array>

template <int xDim, int yDim, typename T = int, bool ColMajor = false>
class Matrix
{
public:
    static const int X = xDim;
    static const int Y = yDim;

    using Type = T;
    using ArrType = std::array<Type, X*Y>;
    using SizeType = typename ArrType::size_type;


    Matrix() : mData({0}) {}
    Matrix(const std::array<int, X*Y>& inp) { reset(inp); } // Column major due to legacy

    SizeType index(int x, int y) const
    {
        if (ColMajor) return y*X + x;
        return x*Y + y;
    }

    void reset(const std::array<Type, X*Y>& data) { mData = data; }

    Type at(int x, int y) const { return mData[index(x, y)]; }
    Type& at(int x, int y) { return mData[index(x, y)]; }

    // currently assuming that all dimensions are valid
    template <typename CM, // convolve matrix
              typename OutMat = Matrix<
                X - (CM::X - 1),
                Y - (CM::Y - 1),
                Type,
                ColMajor>>
    OutMat convolve(const CM& c) const
    {
        OutMat result;
        for (int xo = 0; xo < OutMat::X; ++xo) {
            for (int yo = 0; yo < OutMat::Y; ++yo) {
                for (int xc = 0; xc < CM::X; ++xc) {
                    for (int yc = 0; yc < CM::Y; ++yc) {
                        result.at(xo, yo) += at(xo + xc, yo + yc)*c.at(xc, yc);
                    }
                }
            }
        }
        return result;
    }

    template <typename OutMat = Matrix<Y, X, Type, ColMajor>>
    OutMat transpose() const 
    {
        OutMat result;
        for (int x = 0; x < X; ++x) {
            for (int y = 0; y < X; ++y) {
                result.at(y, x) = at(x, y);
            }
        }
        return result;
    }

    void print() const 
    {
        int sep = ColMajor ? X : Y;
        for (int i = 0; i < mData.size(); ++i) {
            std::cout << std::setw(3) << mData[i] << " ";
            if ((i + 1) % sep == 0) std::cout << std::endl;
        }

        //for (int y = 0; y < Y; ++y) {
            //for (int x = 0; x < X; ++x) {
                //std::cout << std::setw(3) << at(x, y) << " ";
            //}
            //std::cout << std::endl;
        //}
    }

    // Column major iterator
    struct Iterator 
    {
        const Matrix* mat;
        int x;
        int y;
        
        Iterator(const Matrix& mat) : mat(&mat), x(0), y(0) {}
        Iterator& operator++() { if (++y % Matrix::Y == 0) { y = 0; ++x; } return *this; }
        Type operator*() { return mat->at(x, y); } 
        bool operator()() { return (x < Matrix::X) && (y < Matrix::Y); }
    };
    Iterator iterator() const { return Iterator(*this); } 

private:
    //std::array<std::array<Type, yDim>, xDim> mData;
    std::array<Type, yDim*xDim> mData;
};

#endif 
