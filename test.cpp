/**
 * 
 */

#include "./lib"

#include "./libs/range.hpp"

using namespace lib;

namespace Solution_9178170785134090 {

    const int _N = 1'048'576;
    int N, Q;

    std::vector<int> F, size;

    std::deque<std::pair<int, int>> st;
    int find(int x) {
        if (F.at(x) == x)  return x;
        else  return find(F[x]);
    }

    void merge(int x, int y) {
        auto a = find(x), b = find(y);
        if (a == b)  return st.push_back({0, 0});
        if (size.at(a) < size.at(b))  return merge(y, x);
        F[b] = a;
        size[a] += size[b];
        st.push_back({a, b});
    }

    void solve() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr), std::cout.tie(nullptr);

        std::cin >> N >> Q;
        F.resize(N+1), rgs::copy(range(0, N+1), F.begin());
        size.resize(N+1), rgs::fill(size, 1);

        for (auto _: range(Q)) {
            int op, x, y;
            std::cin >> op >> x >> y;

            // 连边
            if (op == 0) {
                merge(x, y);
            } else if (op == 1) {
                // 撤销
                if (st.empty())  continue;
                auto [super, sub] = st.back();  st.pop_back();
                if (super == sub and sub == 0)  continue;
                size[super] -= size[sub];
                F[sub] = sub;
            } else {
                std::cout << find(x) << ' ' << find(y) << std::endl;
                std::cout << (find(x) == find(y)? "Yes": "No") << std::endl;
            }
        }
    }
}

int main() {
    initDebug;
    Solution_9178170785134090::solve();
    return 0;
}