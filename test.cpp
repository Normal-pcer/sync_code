#include "./lib"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    int N;  std::cin >> N;
    std::vector<std::pair<int, int>> kkk(N+1);
    for (auto &[x, y]: kkk | std::views::take(N))  std::cin >> x >> y;
    kkk.back() = {inf, inf};

    std::vector<int> F(N+1, 1);
    for (auto i = 0; i < N+1; i++) {
        for (auto j = 0; j < i; j++) {
            if (kkk.at(i).first > kkk.at(j).first and kkk.at(i).second > kkk.at(j).second) {
                chkMax(F.at(i), F.at(j) + 1);
            }
        }
    }

    std::cout << F.back() - 1 << std::endl;
    return 0;
}