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
        if (h >= 0 && h < static_cast<int>(field_.size()) && w >= 0 && w < static_cast<int>(field_[0].size()))
        {
            field_[h][w] = '#';
        }
    }
    
}

void BattleField::SetNeedSent(bool ready)
{
    ready_sent_redraw_.store(ready, std::memory_order_relaxed);
}

bool BattleField::NeedSent()
{
    return ready_sent_redraw_.load(std::memory_order_relaxed);
}


void draw(std::shared_ptr<BattleField> field, const Tetris &tetris)
{
    field->Clear();
    field->Draw(tetris.ground_);
    field->Draw(tetris.tetramino_, tetris.tetramino_position_);
    field->SetNeedSent(true);

}


//только для отладки
std::ostream& operator<<(std::ostream& out, const std::shared_ptr<BattleField> &field)
{
    out << (*field.get());
    return out;
}

std::ostream &operator<<(std::ostream &out, BattleField &field)
{
    out << ' ';
    auto size = field.GetSize();
    for (int i = 0; i < size.w_; i++)
    {
                out << (i % 10);

    }

    out << std::endl;
    
    int k = 0;
    for (auto i = field.field_.crbegin(); i != field.field_.crend(); i++)
    {
        out << ( (size.h_ - ++k) % 10) << *i << std::endl;
    }
    return out;
}
