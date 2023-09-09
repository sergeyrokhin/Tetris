#include "battlefield.h"

void BattleField::Clear() {
    for (auto &i : field_)
    {
        i = {std::string(size_.w_, '.')};
    }
    
}

void BattleField::Draw(const Figure &figure, Coordinates position)
{
    for (const auto& i : figure.squares_)
    {
        auto h = size_.h_ - 1 - position.h_ + (figure.h_offset_ - i.h_);
        auto w = position.w_ + i.w_;
        if (h >= 0 && h < field_.size() && w >= 0 && w < field_[0].size())
        {
            field_[h][w] = '#';
        }
    }
    
}

std::ostream& operator<<(std::ostream &out, const BattleField & field )
{
    out << ' ';
    for (size_t i = 0; i < field.size_.w_; i++)
    {
                out << (i % 10);

    }

    out << std::endl;
    
    int k = 0;
    for (auto i = field.field_.crbegin(); i != field.field_.crend(); i++)
    {
        out << ( (field.size_.h_ - ++k) % 10) << *i << std::endl;
    }
    return out;
}

void draw(BattleField &field, const Tetris &tetris)
{
    field.Clear();
    field.Draw(tetris.ground_);
    field.Draw(tetris.tetramino_, tetris.tetramino_position_);
}