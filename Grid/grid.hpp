#ifndef _GRID_H_
#define _GRID_H_
#include "bitset.hpp"
#include "point.hpp"

template<typename Point>
struct Grid {
private:
    Bitset _bit;
    
public:
    int H, W;

    Grid (int _H, int _W) : _bit(_H * _W), H(_H), W(_W) {
        Point::set_grid_size(H, W);
    }

    Point move(Point p, int d) {
        Point res = p.move(d);
        if (res.x == -1 || _bit[res.x * W + res.y] == 0) {
            return Point(-1, -1);
        }
        return res;
    }

    Point move_inv(Point p, int d) {
        Point res = p.move_inv(d);
        if (res.x == -1 || _bit[res.x * W + res.y] == 0) {
            return Point(-1, -1);
        }
        return res;
    }

    BitsetRangeref operator [](size_t i) { return BitsetRangeref(_bit, i * W); }
};
#endif