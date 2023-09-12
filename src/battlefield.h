#pragma once

#include "tetris.h"
#include <iostream>
#include <string>


// холст Это относится уже к реализации отображения, а не к математической модели тетриса
class Canvas
{
    Coordinates size_;     // в минимальной еденице экрана (пиксели или символы для символьного экрана)
    Coordinates position_; // тоже
    Canvas(Coordinates size, Coordinates position) : size_(size), position_(position) {}
};


class BattleField {
    Coordinates size_;

    public:

    std::vector<std::string> field_;
    BattleField(Coordinates size) : size_(size), field_(size_.h_, std::string(size_.w_, ' ')) {}
    Coordinates GetSize() { return size_; }

    void Clear();
    void Draw(const Figure& figure, Coordinates position = { 0, 0 });

    friend void draw(std::shared_ptr<BattleField> field, const Tetris& tetris);
    friend std::ostream& operator<<(std::ostream& out, const BattleField& field);
};