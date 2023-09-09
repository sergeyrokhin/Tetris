#pragma once

#include <random>

enum class TetraminoType;

constexpr auto MAX_INDEX_OF_TETRINO_SQUARES = 3;

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

class RandomMashine
{
    static std::random_device rd; // non-deterministic generator
    std::mt19937 gen;             // to seed mersenne twister.
    std::uniform_int_distribution<> dist_tetramino;
    std::uniform_int_distribution<> dist_rotate;

public:
    RandomMashine() : gen(rd()), dist_tetramino(0, 6), dist_rotate(-1, 2)
    {
    }
    TetraminoType GetRandomTetraminoType()
    {
        return static_cast<TetraminoType>(dist_tetramino(gen));
    }
    int GetRandomRotateType()
    {
        return dist_rotate(gen);
    }
};
