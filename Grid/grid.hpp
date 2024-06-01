#ifndef _GRID_H_
#define _GRID_H_
#include "bitset.hpp"


struct Grid {
private:
    int H, W;
    Bitset _bit;

public:
    Grid (int _H, int _W) : H(_H), W(_W), _bit(_H * _W) {}
    
    BitsetRangeref operator [](size_t i) { return BitsetRangeref(_bit, i * H); }
};
#endif