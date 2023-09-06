#pragma once
#define MAX_INDEX_OF_TETRINO_SQUARES 3

class Coordinates
{
    public :

    int h_ = 0, w_ = 0;
    Coordinates() {}
    Coordinates(int h, int w) : h_(h), w_(w) {}
    bool operator==(const Coordinates &other) const
    {
        return other.h_ == h_ && other.w_ == w_;
    }
    Coordinates Shift1(int h, int w) const;
    Coordinates Shift2(const Coordinates &pos) const;
};
