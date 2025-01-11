/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=3357
 */

#include "./libs/debug_macros.hpp"

#include "./lib_v3.hpp"

#include "./libs/io.hpp"

using namespace lib;

namespace Solution_1773102751106496 {
    class BIT {
        std::vector<int> c;
        static constexpr auto lowbit(int x) -> int { return x & -x; }
    public:
        BIT(int N): c(N+1, inf) {}
        auto checkMinAt(int x, int val) -> void {
            x++;  assert(x > 0);
            while (x < static_cast<int>(c.size())) {
                chkMin(c[x], val);
                x += lowbit(x);
            }
        }
        auto minPrefix(int x) -> int {
            x++;  auto ans = inf;
            assert(x >= 0);
            while (x) {
                chkMin(ans, c[x]);
                x -= lowbit(x);
            }
            return ans;
        }
    };
    struct Element {
        int x, y, z;
    };
    using ElementList = std::vector<Element>;
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
        std::sort(ele.begin(), ele.end(), lam(x, y, x.x < y.x));
        auto ans = [&]() -> bool {
            BIT bit{static_cast<int>(values.size())};
            for (auto i = 0, j = 0; j != N; i = j) {
                // [i, j) 范围内的 x 值全相等
                while (j != N and ele[i].x == ele[j].x)  j++;
                for (auto k = i; k != j; k++) {
                    auto min_z = bit.minPrefix(ele[k].y - 1);  // ele[?].y < ele[k].y
                    if (min_z < ele[k].z) {  // exists ele[?].z < ele[k].z
                        return true;
                    }
                }
                for (auto k = i; k != j; k++) {
                    bit.checkMinAt(ele[k].y, ele[k].z);
                }
            }
            return false;
        }();
        if (ans)  io << "Yes" << endl;
        else  io << "No" << endl;
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_1773102751106496::solve();
    return 0;
}
