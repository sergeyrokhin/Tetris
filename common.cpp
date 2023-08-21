#include "common.h"

    Coordinates Coordinates::Shift(int h, int w) const
    {
        return {h_ + h, w_ + w};
    }

    Coordinates Coordinates::Shift(const Coordinates &pos) const
    {
        return {h_ + pos.h_, w_ + pos.w_};
    }

    Coordinates Coordinates::ToField(const Coordinates &pos) const
    {
        return {h_ + pos.h_ - MAX_INDEX_OF_TETRINO_SQUARES, w_ + pos.w_};
    }
