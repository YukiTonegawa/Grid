#ifndef _RANDOM_PATH_H_
#define _RANDOM_PATH_H_

#include <random>
#include <array>
#include <numeric>
#include <algorithm>
#include "../Grid/grid.hpp"
#include "../Random/random.hpp"

// 始点と終点を固定したランダムなパスを生成
// 生成方法
// ・始点からdfsを行う
// ・どの方向に移動するかランダム
// ・同じ頂点は訪れない(始点 = 終点の場合に限りサイクルになる)
// ・終点にたどり着いたら終了

// 長さ1以上のパスが存在しない場合空の配列を返す
template<typename Point>
std::vector<int> random_path(Grid<Point> &g, Point s, Point t) {
    static std::bitset<1000000> used(0); // 訪れた頂点
    std::vector<int> res;
    bool ok = false;

    std::array<int, Point::K> idx;
    std::iota(idx.begin(), idx.end(), 0);

    auto dfs = [&](auto &&dfs, Point p) -> void {
        used[p.x * g.W + p.y] = 1;
        std::shuffle(idx.begin(), idx.end(), rng.mt);

        for (int i = 0; i < Point::K && !ok; i++) {
            Point q = g.move(p, idx[i]);
            if (q.x == -1 || used[q.x * g.W + q.y]) continue;
            res.push_back(idx[i]);
            if (q.x == t.x && q.y == t.y) {
                ok = true;
                return;
            }
            dfs(dfs, q);
            if (!ok) res.pop_back();
        }
        used[p.x * g.W + p.y] = 0;
    };

    dfs(dfs, s);
    // usedを初期化
    for (int d : res) {
        used[s.x * g.W + s.y] = 0;
        s = g.move(s, d);
    }
    return res;
}


#endif