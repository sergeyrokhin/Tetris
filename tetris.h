#pragma once
#include <stdint.h>
#include <vector>
#include <memory>
#include <algorithm>
#include <random>

#include "common.h"

#define TETRINO_STENSIL_SIZE 4
enum class TetraminoType
{
    O,
    I,
    L1,
    L2,
    Z1,
    Z2,
    T
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

class Figure
{
public:
    std::vector<Coordinates> squares_; // с низу с лева на право (0,0) нижняя левая
    int h_offset_ = 0;    // тоже
    Coordinates GetSize();
};

class Tetramino : public Figure
{
    void ShiftTop();
public:
    void RotateLeft();
    void RotateRight();
    void Create(TetraminoType type, int rotate);
};

class Ground : public Figure
{
};

enum class MoveType
{
    Down,
    Left,
    Right,
    RotateL,
    RotateR,
    Drop
};
enum class IntersectionType
{
    Free,
    Intersect,
    Out
}; // ContactBottom
// Инварианты
//  Тетрамино - в пределах стакана
//  не может пересечься с грунтом, т.е. высота любого кубика до кубика грунта по вертикали >= 1
class Tetris
{
    Coordinates size_;
    bool game_is_start_ = false;
    RandomMashine random_;

    public:
    Ground ground_;
    Tetramino tetramino_;
    Coordinates tetramino_position_;

    Tetris(Coordinates size) : size_(size) {}


    bool GameOverCheck();
    bool GameIsStart() {return game_is_start_;}
    void Start() {game_is_start_ = true;}
    void Stop() {game_is_start_ = false;}

    void NewTetramino();
    void NewTetramino(TetraminoType type, int rotate);

    void Move(MoveType move);
    
    /// @brief //Получить тип пересечения тетрамино с грунтом
    /// @param terramino
    /// @param position //какие координаты
    /// @return //тип соприкосновения тетрамино и поверхности
    IntersectionType GetIntersection(const Tetramino &tetramino, const Coordinates &position) const;

    /// @brief
    /// @return на сколько позиций можно упасть (т.е. если соприкасается снизу, то 0)
    int DropHeight() const;
};
