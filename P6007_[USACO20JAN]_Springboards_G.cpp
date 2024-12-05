/**
 * @link https://www.luogu.com.cn/problem/P6007
 */

#include <bits/stdc++.h>
bool DEBUG_MODE=false;
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define from(i,b,e) for(auto i=(b);i<(e);i++)
#define rev(i,e,b) for(auto i=(e);i>(b);i--)

template <typename T> inline auto chkMax(T& base, const T& cmp) { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T& base, const T& cmp) { return (base = std::min(base, cmp)); }
#define never if constexpr(0)
#define always if constexpr(1)
const int inf = 0x3f3f3f3f;  const long long infLL = 0x3f3f3f3f3f3f3f3fLL; using ll = long long; using ull = unsigned long long;

#define __lambda_1(expr) [&](){return expr;}
#define __lambda_2(a, expr) [&](auto a){return expr;}
#define __lambda_3(a, b, expr) [&](auto a, auto b){return expr;}
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c){return expr;}
#define __lambda_overload(a, b, c, d, e, args...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib{}
#if __cplusplus > 201703LL
namespace ranges { using namespace std::ranges; }
namespace views { using namespace std::ranges::views; }
#endif

#include "./libs/range.hpp"

using namespace lib;
// #define int long long
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

    void cdq(decltype(points)::iterator begin, decltype(points)::iterator end) {
        if (begin + 1 == end)  return;

        auto bi = begin - points.begin(), ei = end - points.begin();
        debug std::cout << std::format("cdq({}, {})", bi, ei) << std::endl;

        auto mid = begin + (std::distance(begin, end) >> 1);
        cdq(begin, mid);
        std::sort(begin, mid, lam(x, y, x.y < y.y));
        std::sort(mid, end, lam(x, y, x.y < y.y));

        auto i = begin, j = mid;
        auto max = -inf;
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

        std::sort(mid, end, [](Point x, Point y) { return x.x == y.x? x.y < y.y: x.x < y.x; });
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
        F.resize(boards.size()+5);
        for (auto &[x0, y0, x1, y1, index]: boards) {
            x0 = getIndex(x0), y0 = getIndex(y0);
            x1 = getIndex(x1), y1 = getIndex(y1);
            points.push_back({Point::Outset, x0, y0, index});
            points.push_back({Point::Target, x1, y1, index});

            if (x0 == 0 and y0 == 0)  assert(boards.at(index).index == index), chkMax(F.at(index), boards.at(index).length());

            debug std::cout << '!' << x0 << ' ' << y0 << ' ' << x1 << ' ' << y1 << ' ' << index << std::endl;
        }
        auto indexN = getIndex(N);
        points.push_back({Point::Outset, indexN, indexN, (int)boards.size()});
        boards.emplace_back(indexN, indexN, indexN, indexN, boards.size());

        std::sort(points.begin(), points.end(), [](Point x, Point y) { return x.x == y.x? x.y < y.y: x.x < y.x; });
        cdq(points.begin(), points.end());

        auto max = *std::max_element(F.begin(), F.end());
        // auto max = F.at(boards.size()-1);
        debug for (auto i: F)  std::cout << i << ' ';
        debug std::cout << std::endl;
        std::cout << (N << 1) - max << std::endl;
    }
}

// #undef int
int main(int argc, char const *argv[]) {
    initDebug;
    Solution_2388584816228732::solve();
    return 0;
}