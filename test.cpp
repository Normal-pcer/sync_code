#include <bits/stdc++.h>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    int N, M, K;  std::cin >> N >> M >> K;
    struct Point {
        int x, y, cnt;

        auto operator<=> (const Point &) const = default;
    };

    std::vector<Point> luo;
    std::vector<int> values;
    auto get_index = [&](int x) -> int {
        auto it = std::lower_bound(values.begin(), values.end(), x);
        return std::distance(values.begin(), it);
    };
    for (auto &[x, y, cnt]: luo)  std::cin >> x >> y >> cnt, values.push_back(x), values.push_back(y);
    std::sort(values.begin(), values.end()), values.erase(std::unique(values.begin(), values.end()), values.end());
    for (auto &[x, y, cnt]: luo) {
        x = get_index(x), y = get_index(y);
    }
    std::sort(luo.begin(), luo.end());
    std::vector<int> F(luo.size());
    for (auto i = 0; i < (int)luo.size(); i++) {
        for (auto j = 0; j < i; j++) {
            if (luo.at(j).y <= luo.at(i).y) {
                F.at(i) = std::max(F.at(i), F.at(j) + luo.at(i).cnt);
            }
        }
    }
    std::cout << F.back() << std::endl;
    return 0;
}