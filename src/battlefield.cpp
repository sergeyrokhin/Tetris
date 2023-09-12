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


//только для отладки
std::ostream& operator<<(std::ostream& out, const std::shared_ptr<BattleField> &field)
{
    out << ' ';
    auto size = field.get()->GetSize();
    for (size_t i = 0; i < size.w_; i++)
    {
                out << (i % 10);

    }

    out << std::endl;
    
    int k = 0;
    for (auto i = field.get()->field_.crbegin(); i != field.get()->field_.crend(); i++)
    {
        out << ( (size.h_ - ++k) % 10) << *i << std::endl;
    }
    return out;
}

void draw(std::shared_ptr<BattleField> field, const Tetris &tetris)
{
    field.get()->Clear();
    field.get()->Draw(tetris.ground_);
    field.get()->Draw(tetris.tetramino_, tetris.tetramino_position_);
}