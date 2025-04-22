/**
 * @link
 */
 #include <bits/stdc++.h>
 bool DEBUG_MODE = false;
 #define debug if (DEBUG_MODE)
 #define never if constexpr (false)
 template <typename T> inline auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
 template <typename T> inline auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
 #define _lambda_1(expr) [&]() { return expr; }
 #define _lambda_2(a, expr) [&](auto a) { return expr; }
 #define _lambda_3(a, b, expr) [&](auto a, auto b) { return expr; }
 #define _lambda_4(a, b, c, expr) [&](auto a, auto b, auto c) { return expr; }
 #define _lambda_overload(a, b, c, d, e, ...) _lambda_##e
 #define lambda(...) _lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
 #define lam lambda
 namespace lib {
 #if __cplusplus > 201703LL
     namespace ranges = std::ranges;
     namespace views = std::views;
 #endif
 }
 char constexpr endl = '\n';
 using namespace lib;
 using i16 = std::int16_t; using i32 = std::int32_t; using i64 = std::int64_t;
 using u16 = std::uint16_t; using u32 = std::uint32_t; using u64 = std::uint64_t; using uz = std::size_t;
 using namespace lib;
 
 namespace Solution_6314180276493067 {
     auto solve() -> void {
         std::string s, match;
         std::cin >> s >> match;
 
         s.insert(s.begin(), '_');
         match.insert(match.begin(), '_');
         // s += '_', match += '_';
 
         auto n = static_cast<i32>(s.size()) - 1, m = static_cast<i32>(match.size()) - 1;
 
         std::vector<i32> next(m + 1);
 #if false
         for (i32 len = 1; len <= m; len++) {
             // 暴力匹配前后缀
             i32 i = 1, j = len;
             for (auto _ = len; _ --> 0; ) {
                 if (match.substr(1, i - 1) == match.substr(j + 1, len - j)) {
                     next[len] = i - 1;
                 }
                 i++, j--;
             }
         }
 #else
         i32 curMax = 0;  // 当前最长前缀的结尾
         for (i32 i = 1; i < m; i++) {
             if (curMax > 0 and match[curMax + 1] != match[i + 1]) {
                 curMax = next[curMax];
             }
             if (match[curMax + 1] == match[i + 1]) {
                 curMax++;
             }
             next[i + 1] = curMax;
         }
 #endif
         debug {
             std::cout << "next: " << std::endl;
             for (i32 i = 1; i <= m; i++) {
                 std::cout << "[" << i << "] " << next[i] << "  ";
             }
             std::cout << std::endl;
         }
 
         auto find = [&]() -> void {
             i32 i = 0, j = 0;
 
             for (; i <= n; i++) {
                 while (j > 0 and s[i + 1] != match[j + 1]) {
                     j = next[j];
                 }
                 if (s[i + 1] == match[j + 1]) {
                     j++;
                 }
                 if (j == m) {
                     std::cout << i - m + 2 << endl;
                     j = 0;
                     i -= m - 1;
                 }
             }
         };
         find();
 
         for (i32 i = 1; i <= m; i++) {
             std::cout << next[i] << " ";
         }
         std::cout << endl;
     }
 }
 
 auto main(int argc, char const *argv[]) -> int {
     DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
     std::ios::sync_with_stdio(false);
     std::cin.tie(nullptr), std::cout.tie(nullptr);
 
     Solution_6314180276493067::solve();
     return 0;
 }
 