#ifndef _DIR_H_
#define _DIR_H_

#include <array>

template<int id>
struct PointDir4 {
    static constexpr size_t K = 4;
    // 右->下->左->上
    static constexpr std::array<int8_t, K> dx = {0, 1, 0, -1};
    static constexpr std::array<int8_t, K> dy = {1, 0, -1, 0};

    static size_t H, W; // グリッドサイズ

    static void set_grid_size(size_t _H, size_t _W) {
        H = _H;
        W = _W;
    }

    int x, y;
    PointDir4(int _x, int _y) : x(_x), y(_y) {}
    
    // 方向dに移動 範囲外になる場合は {-1, -1} を返す
    PointDir4 move(int d) {
        assert(0 <= d && d < K);
        assert(0 <= x && x < H);
        assert(0 <= y && y < W);
        PointDir4 res(*this);
        res.x += dx[d];
        res.y += dy[d];
        if (res.x < 0 || res.y < 0 || res.x >= H || res.y >= W) {
            return PointDir4(-1, -1);
        }
        return res;
    }

    // 方向dに移動して {x, y} になる点 ない場合は {-1, -1}
    PointDir4 move_inv(int d) {
        return move(K - 1 - d);
    }
    
    bool operator == (const PointDir4 &p) const { return x == p.x && y == p.y; }
};

template<int id>
struct PointDir8{
    static constexpr size_t K = 8;
    // 右->右下->下->左下->左->左上->上->右上
    static constexpr std::array<int8_t, K> dx = {0, 1, 1, 1, 0, -1, -1, -1};
    static constexpr std::array<int8_t, K> dy = {1, 1, 0, -1, -1, -1, 0, 1};

    static size_t H, W; // グリッドサイズ

    static void set_grid_size(size_t _H, size_t _W) {
        H = _H;
        W = _W;
    }

    int x, y;
    PointDir8(int _x, int _y) : x(_x), y(_y) {}

    // 方向dに移動 範囲外になる場合は {-1, -1} を返す
    PointDir8 move(int d) {
        assert(0 <= d && d < K);
        assert(0 <= x && x < H);
        assert(0 <= y && y < W);
        PointDir8 res(*this);
        res.x += dx[d];
        res.y += dy[d];
        if (res.x < 0 || res.y < 0 || res.x >= H || res.y >= W) {
            return PointDir8(-1, -1);
        }
        return res;
    }

    // 方向dに移動して {x, y} になる点 ない場合は {-1, -1}
    PointDir8 move_inv(int d) {
        return move(K - 1 - d);
    }

    bool operator == (const PointDir8 &p) const { return x == p.x && y == p.y; }
};

template<int id>
struct PointDir4torus {
    static constexpr size_t K = 4;
    // 右->下->左->上
    static constexpr std::array<int8_t, K> dx = {0, 1, 0, -1};
    static constexpr std::array<int8_t, K> dy = {1, 0, -1, 0};

    static size_t H, W; // グリッドサイズ

    static void set_grid_size(size_t _H, size_t _W) {
        H = _H;
        W = _W;
    }

    int x, y;
    PointDir4torus(int _x, int _y) : x(_x), y(_y) {}

    // 方向dに移動
    PointDir4torus move(int d) {
        assert(0 <= d && d < K);
        assert(0 <= x && x < H);
        assert(0 <= y && y < W);
        PointDir4torus res(*this);
        res.x += dx[d];
        res.y += dy[d];
        if (res.x < 0) res.x += H;
        if (res.x >= H) res.x -= H;
        if (res.y < 0) res.y += W;
        if (res.y >= W) res.y -= W;
        return res;
    }

    // 方向dに移動して {x, y} になる点
    PointDir4torus move_inv(int d) {
        return move(K - 1 - d);
    }

    bool operator == (const PointDir4torus &p) const { return x == p.x && y == p.y; }
};

template<int id>
struct PointDir8torus {
    static constexpr size_t K = 8;
    // 右->右下->下->左下->左->左上->上->右上
    static constexpr std::array<int8_t, K> dx = {0, 1, 1, 1, 0, -1, -1, -1};
    static constexpr std::array<int8_t, K> dy = {1, 1, 0, -1, -1, -1, 0, 1};

    static size_t H, W; // グリッドサイズ

    static void set_grid_size(size_t _H, size_t _W) {
        H = _H;
        W = _W;
    }

    int x, y;
    PointDir8torus(int _x, int _y) : x(_x), y(_y) {}

    // 方向dに移動
    PointDir8torus move(int d) {
        assert(0 <= d && d < K);
        assert(0 <= x && x < H);
        assert(0 <= y && y < W);
        PointDir8torus res(*this);
        res.x += dx[d];
        res.y += dy[d];
        if (res.x < 0) res.x += H;
        if (res.x >= H) res.x -= H;
        if (res.y < 0) res.y += W;
        if (res.y >= W) res.y -= W;
        return res;
    }

    // 方向dに移動して {x, y} になる点
    PointDir8torus move_inv(int d) {
        return move(K - 1 - d);
    }

    bool operator == (const PointDir8torus &p) const { return x == p.x && y == p.y; }
};


template<int id>
size_t PointDir4<id>::H = 0;
template<int id>
size_t PointDir4<id>::W = 0;

template<int id>
size_t PointDir8<id>::H = 0;
template<int id>
size_t PointDir8<id>::W = 0;

template<int id>
size_t PointDir4torus<id>::H = 0;
template<int id>
size_t PointDir4torus<id>::W = 0;

template<int id>
size_t PointDir8torus<id>::H = 0;
template<int id>
size_t PointDir8torus<id>::W = 0;
#endif