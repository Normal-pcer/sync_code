#include <bits/stdc++.h>

namespace ranges = std::ranges;

int main() {
    std::fstream fs{"move14.in", std::ios::in};
    std::fstream fs2{"1.in", std::ios::out | std::ios::binary};

    std::string line;
    std::vector<long long> cs;
    std::vector<int> diffs;
    for (int i = 0; fs; i++) {
        std::getline(fs, line);
        if (i > 2 and i < 80005) {
            std::stringstream ss{line};
            long long a, b, c;  ss >> a >> b >> c;
            cs.push_back(c);
            diffs.push_back(b - a);
            fs2 << line << std::endl;
        }
    }

    std::map<int, int> diff_cnt;
    for (auto i: diffs)  diff_cnt[i / 1000 * 1000]++;

    for (auto [i, cnt]: diff_cnt)  std::cout << i << " " << cnt << std::endl;
    std::cout << "--- cs_cnt" << std::endl;
    std::map<int, int> cs_cnt;
    for (auto i: cs)  cs_cnt[int(std::log2(i))]++;
    for (auto [i, cnt]: cs_cnt)  std::cout << i << " " << cnt << std::endl;

    std::ranges::sort(cs);
    // std::ranges::sort(diffs, ranges::greater{});
    // for (auto x: diffs | std::views::take(100))  std::cout << x << std::endl;
    std::cout << "Sum: " << ranges::fold_left(diffs, 0LL, std::plus{}) << std::endl;
    return 0;
}