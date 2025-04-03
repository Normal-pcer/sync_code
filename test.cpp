#include <bits/stdc++.h>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);
    int x; std::cin >> x;

    auto ans = [&]() -> std::string_view {
        switch (x) {
        case 1: return "awu";
        case 2: return "ekhh";
        case 3: return "rnb abj";
        case 4: return "dggg udi uygy nkue";
        case 5: return "lkye iite";
        case 6: return "ewgi nnhy";
        default: return "qwq";
        }
    }();
    std::cout << ans << std::endl;
}