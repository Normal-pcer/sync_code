// Do not expand includes
#include <bits/stdc++.h>

int main() {
    auto count = [&](auto x) {
        std::set<typename decltype(x)::value_type> set;
        for (auto i: x) {
            set.insert(i);
        }
        return set.size();
    };
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    int N;  std::cin >> N;
    std::vector<int> a(N);
    for (auto &i: a)  std::cin >> i;

    auto ans = 0LL;
    for (auto i = 0; i < N; i++) {
        for (auto j = i+1; j <= N; j++) {
            auto cnt = count(std::span{a.begin() + i, a.begin() + j});
            ans += cnt * cnt;
        }
    }

    std::cout << ans << std::endl;
    return 0;
}