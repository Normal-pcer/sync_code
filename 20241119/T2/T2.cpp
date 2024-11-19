#include <bits/stdc++.h>
using ll = long long;
using ull = unsigned long long;

namespace Solution {

    // 检查一个序列是否是闪耀的
    template <typename T>
    bool check_shiny(T &&begin, T &&end) {
        std::set<int> source;  // 必选
        std::set<int> optional;  // 可选

        for (auto it = begin; it != end; it++) {
            auto item = *it;
            source.insert(item);
        }

        for (auto it1 = begin; it1 != end; it1++) {
            for (auto it2 = it1 + 1; it2 != end; it2++) {
                auto sum = *it1 + *it2;
                if (sum % 2 == 0) {
                    optional.insert(sum / 2);
                }
            }
        }

        auto min_element = *source.begin();
        auto max_element = *source.rbegin();
        for (auto i = min_element; i <= max_element; i++) {
            if (source.find(i) == source.end()) {  // source 中找不到
                // 在 optional 中找
                if (optional.find(i) == optional.end())  return false;
            }
        }
        return true;
    }

    void solve() {
        int N;
        std::cin >> N;

        std::vector<int> origin(N);
        for (auto &i: origin)  std::cin >> i;

    }
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr), std::cout.tie(nullptr);
#ifndef ONLINE_JUDGE
    std::freopen("T2.in", "r", stdin);
    std::freopen("T2.out", "w", stdout);
#endif
    int T;  std::cin >> T;
    while (T --> 0)
        Solution::solve();
    return 0;
}