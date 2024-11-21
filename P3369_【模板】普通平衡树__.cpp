/**
 * @link https://www.luogu.com.cn/problem/P3369
 */

#include "./lib"

using namespace lib;

namespace Solution_1637954082994790 {

    const int _N = 1e5+5;
    int N;

    std::vector<int> val = {-inf};
    auto getIndex(int x) {
        return std::lower_bound(val.begin(), val.end(), x) - val.begin();
    }

    inline int lowbit(int x) {
        return x & -x;
    }

    int c[_N];  // 树状数组
    void add(int x, int delta) {
        while (x < _N) {
            c[x] += delta;
            x += lowbit(x);
        }
    }

    void insert(int x) {
        add(x, 1);
    }

    void erase(int x) {
        add(x, -1);
    }

    // 小于等于 x 的数量
    int query(int x) {
        auto res = 0;
        while (x) {
            res += c[x];
            x -= lowbit(x);
        }
        return res;
    }

    int rank(int x) {
        return query(x-1) + 1;
    }

    int nth_element(int n) {
        int max = std::__lg(_N);
        auto idx = 0;
        for (auto i = max; i >= 0; i--) {
            auto detect = idx + (1<<i);
            if (detect < _N and c[detect] < n) {
                // 找右面
                idx += 1<<i;
                n -= c[idx];
            }
        }

        return idx + 1;
    }

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        std::cin >> N;
        int op, x;

        std::vector<std::pair<int, int>> ops;
        while (N --> 0) {
            std::cin >> op >> x;
            ops.push_back({op, x});
            val.push_back(x);
        }

        std::sort(val.begin(), val.end());
        val.erase(std::unique(val.begin(), val.end()), val.end());

        auto size = 0;
        for (auto [op, x]: ops) {
            if (op == 1)  insert(getIndex(x)), size++;
            else if (op == 2)  erase(getIndex(x)), size--;
            else if (op == 3)  std::cout << rank(getIndex(x)) << std::endl;
            else if (op == 4)  std::cout << val[nth_element(x)] << std::endl;
            else if (op == 5)  std::cout << val[nth_element(rank(getIndex(x)-1))] << std::endl;
            else  std::cout << val[nth_element(rank(getIndex(x+1)))] << std::endl;

            debug {
                std::cout << "!!! ";
                for (auto i = 1; i <= size; i++)  std::cout << val[nth_element(i)] << " ";
                std::cout << std::endl;
            }
        }
    }
}

int main() {
    initDebug;
    Solution_1637954082994790::solve();
    return 0;
}