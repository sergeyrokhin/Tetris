#pragma once
#include <stdint.h>

#define MAX_INDEX_OF_TETRINO_SQUARES 3

struct Coordinates
{
    int h_ = 0, w_ = 0;
    Coordinates() {}
    Coordinates(int h, int w) : h_(h), w_(w) {}
    bool operator==(const Coordinates &other) const
    {
        return other.h_ == h_ && other.w_ == w_;
    }
    Coordinates Shift(int h, int w) const;
    Coordinates Shift(const Coordinates &pos) const;
    Coordinates Shift(const Coordinates &pos) const;
};
