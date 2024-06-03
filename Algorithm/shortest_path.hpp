#ifndef _SHORTEST_PATH_H_
#define _SHORTEST_PATH_H_

#include <queue>
#include "../Grid/grid.hpp"

// O(N)
template<typename Point>
struct shortest_path_bfs {
  private:
    std::vector<int> mindist, hist;
  
  public:  
    static constexpr int inf = 1 << 30;
    int H, W;

    void build(Grid<Point> &g, Point s) {
        H = g.H;
        W = g.W;
        mindist = std::vector<int>(H * W, inf);
        hist = std::vector<int>(H * W, -1);
        std::queue<Point> q;
        q.push(s);
        mindist[s.x * W + s.y] = 0;

        while (!q.empty()) {
            Point p = q.front();
            q.pop();
            int idx = p.x * W + p.y;
            for (int i = 0; i < Point::K; i++) {
                Point p2 = g.move(p, i);
                int idx2 = p2.x * W + p2.y;
                if (p2.x != -1 && mindist[idx2] > mindist[idx] + 1) {
                    mindist[idx2] = mindist[idx] + 1;
                    hist[idx2] = i;
                    q.push(p2);
                }
            }
        }
    }

    // tへの最短距離 到達不可能な場合はinf O(1)
    int dist(Point t) {
        assert(0 <= t.x && t.x < H);
        assert(0 <= t.y && t.y < W);
        return mindist[t.x * W + t.y];
    }

    // 最短経路の復元(移動の操作列) O(パス長)
    // 到達不可能な場合は{-1}を返す
    std::vector<int> path(Point t) {
        assert(0 <= t.x && t.x < H);
        assert(0 <= t.y && t.y < W);
        int idx = t.x * W + t.y;
        if (mindist[idx] == inf) return {-1};
        std::vector<int> res;
        while (hist[idx] != -1) {
            res.push_back(hist[idx]);
            t = t.move_inv(hist[idx]);
            idx = t.x * W + t.y;
        }
        std::reverse(res.begin(), res.end());
        return res;
    }
};

// 辺の重みを指定する get_dist(p, d) := pから方向dへの距離を取得する関数
// O(N)
template<typename Point, typename Dist>
struct shortest_path_dijkstra {
    private:
    std::vector<Dist> mindist;
    std::vector<int> hist;
  
  public:
    static constexpr Dist inf = std::numeric_limits<Dist>() / 2;
    int H, W;

    void build(Grid<Point> &g, Point s, std::function<Dist(Point, int)> get_dist) {
        H = g.H;
        W = g.W;
        mindist = std::vector<Dist>(H * W, inf);
        hist = std::vector<int>(H * W, -1);

        using pr = std::pair<Dist, Point>;
        struct Cmp {
            bool operator () (const pr &A, const pr &B) {
                return A.first > B.first;
            }
        };
        std::priority_queue<pr, std::vector<pr>, Cmp> q;
        q.push({0, s});
        mindist[s.x * W + s.y] = 0;

        while (!q.empty()) {
            auto [d, p] = q.top();
            q.pop();
            int idx = p.x * W + p.y;
            if (d != mindist[idx]) continue;
            for (int i = 0; i < Point::K; i++) {
                Point p2 = g.move(p, i);
                int idx2 = p2.x * W + p2.y;
                Dist d2 = get_dist(p, i);
                if (p2.x != -1 && mindist[idx2] > d + d2) {
                    mindist[idx2] = d + d2;
                    hist[idx2] = i;
                    q.push({mindist[idx2], p2});
                }
            }
        }

    }

    // tへの最短距離 到達不可能な場合はinf O(1)
    Dist dist(Point t) {
        assert(0 <= t.x && t.x < H);
        assert(0 <= t.y && t.y < W);
        return mindist[t.x * W + t.y];
    }

    // 最短経路の復元(移動の操作列) O(パス長)
    // 到達不可能な場合は{-1}を返す
    std::vector<int> path(Point t) {
        assert(0 <= t.x && t.x < H);
        assert(0 <= t.y && t.y < W);
        int idx = t.x * W + t.y;
        if (mindist[idx] == inf) return {-1};
        std::vector<int> res;
        while (hist[idx] != -1) {
            res.push_back(hist[idx]);
            t = t.move_inv(hist[idx]);
            idx = t.x * W + t.y;
        }
        std::reverse(res.begin(), res.end());
        return res;
    }
};

#endif