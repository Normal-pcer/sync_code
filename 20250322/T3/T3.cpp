#include <bits/stdc++.h>
#define FILE_NAME "T3"
#ifdef LOCAL
#define debug if (true)
#else  // not def LOCAL
#define debug if (false)
#endif  // def LOCAL

using i16 = std::int16_t; using i32 = std::int32_t; using i64 = std::int64_t;
using u16 = std::int16_t; using u32 = std::uint32_t; using u64 = std::uint64_t; using uz = std::size_t;

template <typename T, typename U>  auto chkMin(T &base, U const &cmp) -> T & { return (base = std::min<T>(base, cmp)); }
template <typename T, typename U>  auto chkMax(T &base, U const &cmp) -> T & { return (base = std::max<T>(base, cmp)); }

/*
*/
namespace Solution {
    i32 constexpr inf = 0x3f3f3f3f;
    auto solve() -> void {
        i32 N; std::cin >> N;
        std::string s; std::cin >> s;
        std::vector<i32> a(N + 1);
        for (i32 i = 1; i <= N; i++) std::cin >> a[i];

        s.insert(s.begin(), '_');
        std::vector<i32> prev(N + 1);
        {
            std::vector<i32> st;
            for (i32 i = 1; i <= N; i++) {
                auto ch = s[i];
                if (ch == '(') {
                    st.push_back(i);
                } else {
                    if (st.empty()) {
                        prev[i] = 0;
                    } else {
                        prev[i] = st.back();
                        st.pop_back();
                    }
                }
            }
        }

        std::vector<i64> F(N + 1, -inf);
        F[0] = 0;
        for (i32 i = 1; i <= N; i++) {
            auto ch = s[i];
            if (ch == ')') {
                chkMax(F[i], F[i - 1]);
                chkMax(F[i], F[prev[i]] + (a[i] - a[prev[i]]));
            } else {
                chkMax(F[i], F[i - 1]);
            }
        }
        std::cout << F[N] << '\n';
    }
}

auto main() -> int {
    std::freopen(FILE_NAME ".in", "r", stdin);
    std::freopen(FILE_NAME ".out", "w", stdout);

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    Solution::solve();

    return 0;
}