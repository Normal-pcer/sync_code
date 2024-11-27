#include <bits/stdc++.h>
#define FILE_NAME "A"
#define DEBUG_MODE false
#define debug if (DEBUG_MODE)
#if false
#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#endif  // if false
using ll = long long;
using ull = unsigned long long;

namespace Solution {
    void egg() {
        std::cout << "No" << std::endl;
    }

    std::vector<int> fa, size;
    int find(int x) {
        if (x == fa.at(x))  return x;
        else  return (fa.at(x) = find(fa.at(x)));
    }

    void connect(int x, int y) {
        auto a = find(x), b = find(y);
        if (size.at(a) < size.at(b))  std::swap(a, b), std::swap(x, y);
        size.at(a) += size.at(b), fa.at(b) = a;
    }

    void solve() {
        int N, M;  std::cin >> N >> M;
        std::vector<int> origin(N), target(N);
        std::vector<std::pair<int, int>> ranges(M);
        for (auto &i: origin)  std::cin >> i;
        for (auto &i: target)  std::cin >> i;
        for (auto &[x, y]: ranges)  std::cin >> x >> y, x--, y--;

        fa.resize(N), size.resize(N);
        for (auto i = 0; i < N; i++)  fa.at(i) = i, size.at(i) = 1;

        for (auto [x, y]: ranges) {
            for (auto i = x; i < y; i++) {
                connect(i, i+1);
            }
        }

        std::vector<int> origin_copy(origin.begin(), origin.end()), 
            target_copy(target.begin(), target.end());
        
        std::sort(origin_copy.begin(), origin_copy.end());
        std::sort(target_copy.begin(), target_copy.end());
        if (origin_copy != target_copy)  return egg();

        auto prev_fa = 0;
        std::vector<std::pair<int, int>> continuousRanges;  // 连续区间，左闭右开
        for (auto i = 0; i < N; i++) {
            if (find(i) != find(prev_fa)) {
                continuousRanges.push_back({prev_fa, i});
                prev_fa = i;
            }
        }

        for (auto [begin, end]: continuousRanges) {
            std::vector<int> from(origin.begin() + begin, origin.begin() + end);
            std::vector<int> to(target.begin() + begin, target.begin() + end);

            debug {
                std::cout << begin << ", " << end << ":" << std::endl;
                std::cout << "from: ";
                for (auto i: from)  std::cout << i << ' ';
                std::cout << std::endl << "to: ";
                for (auto i: to)  std::cout << i << ' ';
                std::cout << std::endl;
            }

            std::sort(from.begin(), from.end());
            std::sort(to.begin(), to.end());

            if (from != to)  return egg();
       }

       std::cout << "Yes" << std::endl;
    }
}

int main() {
#ifndef ONLINE_JUDGE
    std::freopen(FILE_NAME ".in", "r", stdin);
    std::freopen(FILE_NAME ".out", "w", stdout);
#endif

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    int T;  std::cin >> T;

    while (T --> 0)  Solution::solve();

    return 0;
}