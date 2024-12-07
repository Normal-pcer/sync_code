#include "./lib"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    int N, K;  std::cin >> N >> K;
    std::vector<std::tuple<int, int, int>> bots(N);

    for (auto &[x, r, q]: bots)  std::cin >> x >> r >> q;
    auto cnt = 0LL;
    for (auto i = bots.begin(); i != bots.end(); i++) {
        for (auto j = bots.begin(); j != i; j++) {
            auto &[xi, ri, qi] = *i;
            auto &[xj, rj, qj] = *j;

            if (xi - ri <= xj and xj <= xi + ri) {
                if (xj - rj <= xi and xi <= xj + rj) {
                    if (qi - qj <= K and qj - qi <= K) {
                        cnt++;
                    }
                }
            }
        }
    }

    std::cout << cnt << endl;
    return 0;
}