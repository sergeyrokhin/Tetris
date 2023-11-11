#include "common.h"
#include <chrono>
#include <thread>

    Coordinates Coordinates::Shift1(int h, int w) const
    {
        return {h_ + h, w_ + w};
    }

    Coordinates Coordinates::Shift2(const Coordinates &pos) const
    {
        return {h_ + pos.h_, w_ + pos.w_};
    }

    void wait() {
        using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
        std::this_thread::sleep_for(100ms);
    }