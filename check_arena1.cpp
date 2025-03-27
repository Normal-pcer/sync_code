#include "./lib_v5.hpp"
#include "./libs/fixed_int.hpp"
using namespace lib;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    i32 N, Q; std::cin >> N >> Q;
    std::vector<i32> a(N);
    for (auto &x: a) std::cin >> x;

    for (auto q = Q; q --> 0; ) {
        i32 op, l, r; std::cin >> op >> l >> r;
        l--, r--;
        if (op == 1) {
            i64 ans = 0;
            for (i32 i = l; i <= r; i++) {
                for (i32 j = l; j <= r; j++) {
                    if (i > j and a[i] < a[j]) {
                        ans++;
                    }
                }
            }
            std::cout << ans << endl;
        } else {
            std::array<i32, 3> rp;
            for (auto &x: rp) std::cin >> x;
            auto cp = a;
            for (i32 i = l; i <= r; i++) {
                cp[i] = rp[a[i]];
            }
            a = cp;
        }
    }
}
