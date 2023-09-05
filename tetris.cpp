#include <cassert>
#include "tetris.h"

    std::random_device RandomMashine::rd;

    Coordinates Figure::GetSize()
    {
        Coordinates size;
        for (const auto &i : squares_)
        {
            if (size.h_ < i.h_)
            {
                size.h_ = i.h_;
            }
            if (size.w_ < i.w_)
            {
                size.w_ = i.w_;
            }
        }
        return size;
    }

    //inline size_t abs_(int a) {return a > 0 ? a : -a;}

    void Tetramino::RotateLeft()
    {
        for (auto &i : squares_)
        {
            auto n = i.w_;
            i.w_ = MAX_INDEX_OF_TETRINO_SQUARES - i.h_;
            i.h_ = n;
        }
        ShiftTop();
    }

    void Tetramino::RotateRight()
    {
        for (auto &i : squares_)
        {
            auto n = i.h_;
            i.h_ = MAX_INDEX_OF_TETRINO_SQUARES - i.w_;
            i.w_ = n;
        };
        ShiftTop();
    }

    void Tetramino::Create(TetraminoType type, int rotate)
    {
        squares_.clear();
        switch (type)
        {
        case TetraminoType::O:
            squares_.emplace_back(3, 2); // 3 .XX.
            squares_.emplace_back(2, 2); // 2 .XX.
            squares_.emplace_back(3, 1); // 1 ....
            squares_.emplace_back(2, 1); // 0 ....
            break;
        case TetraminoType::I:
            squares_.emplace_back(3, 0); // 3 XXXX
            squares_.emplace_back(3, 1); // 2 ....
            squares_.emplace_back(3, 2); // 1 ....
            squares_.emplace_back(3, 3); // 0 ....
            break;
        case TetraminoType::L1:
            squares_.emplace_back(3, 1); // 3 .XX.
            squares_.emplace_back(3, 2); // 2 ..X.
            squares_.emplace_back(2, 2); // 1 ..X.
            squares_.emplace_back(1, 2); // 0 ....
            break;
        case TetraminoType::L2:
            squares_.emplace_back(3, 1); // 3 .XX.
            squares_.emplace_back(3, 2); // 2 .X..
            squares_.emplace_back(2, 1); // 1 .X..
            squares_.emplace_back(1, 1); // 0 ....
            break;
        case TetraminoType::Z1:
            squares_.emplace_back(3, 2); // 3 ..XX
            squares_.emplace_back(3, 3); // 2 .XX.
            squares_.emplace_back(2, 2); // 1 ....
            squares_.emplace_back(2, 1); // 0 ....
            break;
        case TetraminoType::Z2:
            squares_.emplace_back(3, 2); // 3 .XX.
            squares_.emplace_back(3, 1); // 2 ..XX
            squares_.emplace_back(2, 2); // 1 ....
            squares_.emplace_back(2, 3); // 0 ....
            break;
        case TetraminoType::T:
            squares_.emplace_back(3, 1); // 3 .XXX
            squares_.emplace_back(3, 2); // 2 ..X.
            squares_.emplace_back(3, 3); // 1 ....
            squares_.emplace_back(2, 2); // 0 ....
            break;
        default:
            break;
        }
        if (rotate == -1)
        {
            RotateLeft();
        }
        else
            while (rotate-- > 0)
            {
                RotateRight();
            }
    }

    bool Tetris::GameOverCheck()
    {
        if (game_is_start_ && GetIntersection(tetramino_, tetramino_position_) != IntersectionType::Free)
        {
            game_is_start_ = false;
        }
        return game_is_start_;
    }

    void Tetris::Start()
    {
        ground_.squares_.clear();
        NewTetramino();
        game_is_start_ = true;
    }

    void Tetris::NewTetramino(TetraminoType type, int rotate) {
        tetramino_.Create(type, rotate);
        tetramino_position_ = {size_.h_  - MAX_INDEX_OF_TETRINO_SQUARES - 1, ((size_.w_ - MAX_INDEX_OF_TETRINO_SQUARES) / 2)};
        GameOverCheck();
    }

    void Tetris::NewTetramino()
    {
        NewTetramino(random_.GetRandomTetraminoType(), random_.GetRandomRotateType());
    }

    void Tetris::Move(MoveType move)
    {
        // после соприкосновения ContactBottom тетрамино ещё живой
        //
        if (move == MoveType::Drop)
        {
            auto h_drop = DropHeight();
            // передаем на грунт кубики тетрамино
            for (const auto &i : tetramino_.squares_)
            {
                int h = (tetramino_position_.h_ - h_drop) + i.h_;
                int w = tetramino_position_.w_ + i.w_;
                assert(h >= 0);
                assert(w >= 0);
                ground_.squares_.emplace_back( h, w);
            }
            NewTetramino();
            ground_.Wrap(size_.w_);
        }
        else if (move == MoveType::Down) // попытка спуститься на уровень, если некуда (пересечение), тогда сброс
        {
            auto pos_new = tetramino_position_.Shift(-1, 0);
            auto intersection = GetIntersection(tetramino_, pos_new);
            if (intersection == IntersectionType::Intersect) // если спуститься, и будет пересечение
            {
                Move(MoveType::Drop);
                return;
            }
            tetramino_position_ = pos_new;
            // return;
        }
        else if (move == MoveType::Left || move == MoveType::Right)
        {
            auto pos_new = tetramino_position_.Shift(0, move == MoveType::Left ? -1 : 1);
            auto intersection = GetIntersection(tetramino_, pos_new);
            if (intersection == IntersectionType::Free)
            {
                tetramino_position_ = pos_new;
            }
        }
        else
        {
            auto tetramino = tetramino_; // компилятор справится с конструктором копирования?
            if (move == MoveType::RotateL)
            {
                tetramino.RotateLeft();
            }
            else
            {
                tetramino.RotateRight();
            }
            auto intersection = GetIntersection(tetramino, tetramino_position_);
            if (intersection == IntersectionType::Free)
            {
                tetramino_ = tetramino; // а с копированием справится?
            }
        }
    }

    IntersectionType Tetris::GetIntersection(const Tetramino &tetramino, const Coordinates &position) const
    {
        IntersectionType result = IntersectionType::Free;
        for (const auto &i : tetramino.squares_)
        {
            auto i_abs = i.Shift(position); // в абсолютных координатах
            if (i_abs.h_ < 0)
            {
                return IntersectionType::Intersect; // да, пересечение, те. опустился на дно
            }

            if (i_abs.w_ < 0 || i_abs.w_ >= size_.w_)
            {
                return IntersectionType::Out; // За боковой стенкой
            }

            for (const auto &j : ground_.squares_)
            {
                if (i_abs == j)
                {
                    return IntersectionType::Intersect;
                }
            }
        }
        return result;
    }

    /// @brief
    /// @return на сколько позиций можно упасть (т.е. если соприкасается снизу, то 0)
    int Tetris::DropHeight() const
    {
        std::vector<int> max_ground(size_.w_, -1); //вектор рассояний верх упавших 
        for (const auto& j : ground_.squares_)
        {
            //
            if (j.h_ > max_ground[j.w_])
            {
                max_ground[j.w_] = j.h_;
            }
        }
        auto tetramino_hight_flight = tetramino_position_.h_; // высота от дна до начала координат тетрамина
        std::vector<int> min_tetramino(size_.w_, size_.h_); //вектор рассояний падения по каждой вертикали
        for (const auto &i : tetramino_.squares_)
        {
            auto tetramino_w = tetramino_position_.w_ + i.w_;
            auto h_sq_bottom = (tetramino_hight_flight + i.h_ - (1 + max_ground[tetramino_w])); //от дна трафарета + до квадратика - упавшие
            if (min_tetramino[tetramino_w] > h_sq_bottom)
            {
                min_tetramino[tetramino_w] = h_sq_bottom;
            }
        }
        return (*std::min_element(min_tetramino.begin(), min_tetramino.end()));
    }

    void Tetramino::ShiftTop() //на самый верх сдвинуть внутри трафарета 4х4. т.е. самый верх = MAX_INDEX_OF_TETRINO_SQUARES
    {
        int h = 0; //найдем самый верхний
        int w1 = MAX_INDEX_OF_TETRINO_SQUARES, w2 = 0;

        for (const auto &i : squares_)
        {
            if (h < i.h_) //самая верхняя
            {
                h = i.h_;
            }
            if (w1 > i.w_) //самая левая
            {
                w1 = i.w_;
            }
            if (w2 < i.w_) //самая правая
            {
                w2 = i.w_;
            }
        }
        auto w = ((MAX_INDEX_OF_TETRINO_SQUARES - w2) - w1) / 2; //сдвинуть на
        h = MAX_INDEX_OF_TETRINO_SQUARES - h;
        if (h != 0 || w != 0)
        {
            for (auto &i : squares_)
            { 
                i.h_ += h;
                i.w_ += w;
            }
        }
    }

    void Ground::Wrap(int width)
    {
        std::vector<int> line;
        for (auto i : squares_)
        {
            if (line.size() <= i.h_)
            {
                line.resize(i.h_ + 1);
            }
            ++line[i.h_]; //посчитаем, сколько их, но их не может быть больше width
        }
        std::vector<int> line_wrap(line.size());
        int full_line_count = 0;
        for (size_t i = 0; i < line.size(); i++) {
            assert(line[i] <= width);
            if (line[i] == width)
            {
                ++full_line_count;
            }
            line_wrap[i] = full_line_count;
        }
        if (full_line_count == 0)
        {
            return;
        }
        std::vector<Coordinates> squares_new;
        squares_new.reserve(squares_.size() - static_cast<size_t>(width) * full_line_count);
        for (auto s : squares_)
        {
            if (line[s.h_] != width)
            {
                squares_new.emplace_back(s.h_ - line_wrap[s.h_], s.w_);
            }
        }
        squares_.swap(squares_new);
    }
