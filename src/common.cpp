#include "common.h"

    Coordinates Coordinates::Shift1(int h, int w) const
    {
        return {h_ + h, w_ + w};
    }

    Coordinates Coordinates::Shift2(const Coordinates &pos) const
    {
        return {h_ + pos.h_, w_ + pos.w_};
    }