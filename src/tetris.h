#pragma once
#include <stdint.h>
#include <vector>
#include <memory>
#include <algorithm>

#include "common.h"

#define TETRINO_STENSIL_SIZE 4

enum class TetraminoType;

class Figure
{
public:
    std::vector<Coordinates> squares_; // с низу с лева на право (0,0) нижняя левая
                                        //инвариант: не допускается одинаковых
                                        // set  не будем для этого привлекать.
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
    void CorrectPositionW(Coordinates &pos, int w_size) const;
};

class Ground : public Figure
{   public:
    void Wrap(int width);

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
    inline bool GameIsStart() {return game_is_start_;}
    void Start();
    inline void Stop() {game_is_start_ = false;}

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
