/**
 * @link https://www.luogu.com.cn/problem/P6007
 */

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;
#define int long long
namespace Solution_2388584816228732 {
    std::vector<int> values;

    struct Board {
        int x0, y0, x1, y1, index = 0;

        int length() const {
            assert(x1 >= x0 and y1 >= y0);
            return values.at(x1) - values.at(x0) + values.at(y1) - values.at(y0);
        }
    };

    struct Point {
        enum Type {Outset, Target} type;
        int x, y;
        int index;
    };
    std::vector<Board> boards;
    std::vector<Point> points;

    int getIndex(int x) {
        auto it = std::lower_bound(values.begin(), values.end(), x);
        assert(*it == x);
        return it - values.begin();
    }

    int N, P;
    std::vector<int> F;

    void cdq(std::vector<Point>::iterator begin, std::vector<Point>::iterator end) {
        if (begin + 1 == end)  return;

        auto mid = begin + (std::distance(begin, end) >> 1);
        cdq(begin, mid);
        std::sort(begin, mid, lam(x, y, x.y < y.y));
        std::sort(mid, end, lam(x, y, x.y < y.y));

        auto i = begin, j = mid;
        auto max = -infLL;
        while (i != mid or j != end) {
            if (j == end or (i != mid and i->y <= j->y)) {
                if (i->type == Point::Target) {
                    chkMax(max, F.at(i->index));
                }
                i++;
            } else {
                if (j->type == Point::Outset) {
                    chkMax(F.at(j->index), max + boards.at(j->index).length());
                }
                j++;
            }
        }

        std::sort(mid, end, lam(x, y, x.x < y.x));
        cdq(mid, end);
    }

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        std::cin >> N >> P;
        boards.resize(P);
        for (auto i: range(P)) {
            auto &[x0, y0, x1, y1, index] = boards.at(i);
            std::cin >> x0 >> y0 >> x1 >> y1, index = i;
            values.push_back(x0), values.push_back(y0);
            values.push_back(x1), values.push_back(y1);
        }

        values.push_back(0), values.push_back(N);
        std::sort(values.begin(), values.end());
        values.erase(std::unique(values.begin(), values.end()), values.end());

        points.push_back({Point::Target, 0, 0, (int)boards.size()+2});
        for (auto &[x0, y0, x1, y1, index]: boards) {
            x0 = getIndex(x0), y0 = getIndex(y0);
            x1 = getIndex(x1), y1 = getIndex(y1);
            points.push_back({Point::Outset, x0, y0, index});
            points.push_back({Point::Target, x1, y1, index});

            debug std::cout << '!' << x0 << ' ' << y0 << ' ' << x1 << ' ' << y1 << ' ' << index << std::endl;
        }
        auto indexN = getIndex(N);
        points.push_back({Point::Outset, indexN, indexN, (int)boards.size()});
        boards.emplace_back(indexN, indexN, indexN, indexN, boards.size());

        F.resize(boards.size()+2);
        std::sort(points.begin(), points.end(), lam(x, y, x.x < y.x));
        cdq(points.begin(), points.end());

        // auto max = *std::max_element(F.begin(), F.end());
        auto max = F.at(boards.size() - 1);
        debug for (auto i: F)  std::cout << i << ' ';
        debug std::cout << std::endl;
        std::cout << (N << 1) - max << std::endl;
    }
}

#undef int
int main() {
    initDebug;
    Solution_2388584816228732::solve();
    return 0;
}
