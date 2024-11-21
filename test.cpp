/**
 * 
 */

#include "./lib"

#include "./libs/io.hpp"

using namespace lib;

namespace Solution_1957976573789557 {
    
    int N;  const int _N = 32768;
    std::vector<int> values {-inf};
    int c[_N];

    inline auto getIndex(int x) {
        return std::lower_bound(values.begin(), values.end(), x) - values.begin();
    }

    inline auto lowbit(int x) { return x & -x; }

    int query(int x) {
        auto res = 0;
        while (x) {
            res += c[x];
            x -= lowbit(x);
        }
        return res;
    }

    void update(int x, int val) {
        while (x < _N) {
            c[x] += val;
            x += lowbit(x);
        }
    }

    void solve() {
        io >> N;
        std::vector<int> a(N);
        from(i, 0, N)  io >> a[i];

        values.resize(a.size()+1);
        std::copy(a.begin(), a.end(), values.begin()+1);  // values 中保留一个值，避免树状数组插入第 0 位
        std::sort(values.begin()+1, values.end());

        std::vector<int> less(N);  // i 左侧更小的值数量
        std::vector<int> greater(N);  // 右侧更大

        from(i, 0ULL, a.size()) {
            auto cur = a[i];
            less[i] = query(getIndex(cur) - 1);
            update(getIndex(cur), 1);
        }

        std::memset(c, 0, sizeof(c));

        for (auto i = a.size(); i > 0; i--) {
            auto tot = a.size() - i;  // 经历过的元素
            auto cur = a[i-1];
            auto this_leq = query(getIndex(cur));
            auto this_greater = tot - this_leq;
            greater[i-1] = this_greater;
            update(getIndex(cur), 1);
        }

        never {
            for (auto i: less)  io << i << ' ';
            io << endl;
            for (auto i: greater)  io << i << ' ';
            io << endl;
        }

        auto ans = 0LL;
        from(i, 0ULL, a.size()) {
            auto cur = (ll)less[i] * greater[i];
            ans += cur;
        }

        io << ans << endl;
    }
}

int main() {
    initDebug;
    Solution_1957976573789557::solve();
    return 0;
}