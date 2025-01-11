/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=3357
 */

#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/io.hpp"

using namespace lib;

namespace Solution_1069174936185286 {
    class BIT {
        struct Node {
            int val, time;
        };
        int current = 0;
        std::vector<Node> c;

        static constexpr auto lowbit(int x) noexcept -> int { return x & -x; }
    public:
        BIT(int N): c(N+1) {}
        auto addAt(int x, int val) -> void {
            x++;  assert(x > 0);
            while (x < static_cast<int>(c.size())) {
                if (c[x].time < current)  c[x] = {val, current};
                else  c[x].val += val;
                x += lowbit(x);
            }
        }
        auto sumPrefix(int x) -> int {
            x++;  auto res = 0;
            while (x) {
                if (c[x].time == current)  res += c[x].val;
                x -= lowbit(x);
            }
            return res;
        }
        auto tick() -> void {
            current++;
        }
    } bit(0);

    struct Element {
        int x, y, z;
    };
    using ElementList = std::vector<Element>;

    struct Operation {
        enum Type {
            Update, Query,
        } type;
        Element e;
    };
    using OperationList = std::vector<Operation>;

    // 是否存在 a.x < b.x，a.y < b.y，a.z < b.z
    auto cdq(OperationList::iterator begin, OperationList::iterator end) -> bool {
        if (std::distance(begin, end) == 1)  return false;
        auto mid = begin + (std::distance(begin, end) >> 1);
        if (cdq(begin, mid))  return true;
        bit.tick();
        if (cdq(mid, end))  return true;
        bit.tick();

        auto i = begin, j = mid;
        while (i != mid or j != end) {
            if (j == end or (i != mid and i->e.y <= j->e.y)) {
                if (i->type == Operation::Update)  bit.addAt(i->e.z, +1);
                i++;
            } else {
                if (j->type == Operation::Query) {
                    auto cnt = bit.sumPrefix(j->e.z);
                    if (cnt != 0)  return true;
                }
                j++;
            }
        }
        std::sort(begin, end, lam(x, y, x.e.y < y.e.y));
        return false;
    }
    void solve() {
        int N;  io >> N;
        ElementList ele(N);
        std::vector<int> values;
        for (auto &[x, y, z]: ele) {
            io >> x >> y >> z;
            values.push_back(x);
            values.push_back(y);
            values.push_back(z);
        }
        std::sort(values.begin(), values.end());
        values.erase(std::unique(values.begin(), values.end()), values.end());
        auto getIndex = [&](int x) -> int {
            auto it = std::lower_bound(values.begin(), values.end(), x);
            assert(*it == x);
            return std::distance(values.begin(), it);
        };
        for (auto &[x, y, z]: ele) {
            for (auto it: {&x, &y, &z})  *it = getIndex(*it);
            if (x > y)  std::swap(x, y);
            if (x > z)  std::swap(x, z);
            if (y > z)  std::swap(y, z);
        }
        std::vector<Operation> ops;
        for (auto [x, y, z]: ele) {
            ops.push_back({Operation::Update, {x, y, z}});
            ops.push_back({Operation::Query, {x-1, y-1, z-1}});
        }

        std::stable_sort(ops.begin(), ops.end(), [](Operation x, Operation y) {
            if (x.e.x != y.e.x)  return x.e.x < y.e.x;
            if (x.e.y != y.e.y)  return x.e.y < y.e.y;
            if (x.e.z != y.e.z)  return x.e.z < y.e.z;
            return static_cast<int>(x.type) < static_cast<int>(y.type);
        });
        bit = BIT(values.size());
        auto ans = cdq(ops.begin(), ops.end());
        if (ans)  io << "Yes" << endl;
        else  io << "No" << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1069174936185286::solve();
    return 0;
}
