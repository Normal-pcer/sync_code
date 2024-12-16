#include "./lib"

using namespace lib;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    std::vector flat(1000, std::vector<int8_t>(1000));
    int N;  std::cin >> N;
    for (auto i = 0; i < N; i++) {
        int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;
        for (auto x = x1; x != x2; x++) {
            for (auto y = y1; y != y2; y++) {
                flat.at(x).at(y) = true;
            }
        }
    }
    auto cnt = 0LL;
    for (auto const &line: flat) {
        for (auto item: line) {
            cnt += !!item;
        }
    }
    std::cout << cnt << std::endl;
    return 0;
}