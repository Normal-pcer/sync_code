/**
 * @link https://www.luogu.com.cn/problem/P9016
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
 
 /*
 用节点 t[i][ch] 表示第 i 次替换之后的字母 ch。
 那么一次操作就可以转化为若干次连边，例如第一次操作是 把字母 b 替换为 "ac"，就可以转化为依次进行以下连边：
 t[0][b] -> t[1][a]
         -> t[1][c]
 
 再比如把字母 a 替换为 "aaab"：
 t[0][a] -> t[1][a]
         -> t[1][a]
         -> t[1][a]
         -> t[1][b]
 
 接下来，对于每一个节点，可以统计出来它的实际 size。
 注意 size 可能会爆 double。size 足够大的时候就不用管它了。
 
 接下来，统计答案。需要找到从左到右数第 l 个叶子节点并开始遍历。
 这一步可以用类似线段树上二分的方式。
 */
 namespace Solution_5242390942628612 {
     auto stringReplace(std::string const &s, std::string const &from, std::string const &to) -> std::string {
         std::string result;
         std::boyer_moore_searcher searcher{from.begin(), from.end()};
         auto it = s.begin();
         while (it != s.end()) {
             auto found = std::search(it, s.end(), searcher);
             ranges::copy(it, found, std::back_inserter(result));
             it = found;
             if (found != s.end()) {
                 ranges::copy(to, std::back_inserter(result));
                 it += from.end() - from.begin();
             }
         }
         return result;
     }
     auto solveForce() -> void {
         i32 l, r, opCount;
         std::cin >> l >> r >> opCount;
 
         std::vector<std::pair<std::string, std::string>> ops(opCount);
         for (auto &[from, to] : ops) {
             std::cin >> from >> to;
         }
 
         std::string s("a");
         for (auto const &[from, to]: ops) {
             s = stringReplace(s, from, to);
         }
         std::cout << s.substr(l - 1, r - l + 1) << endl;
     }
 
     i32 constexpr charsetSize = 26;
     auto constexpr ord(char ch) -> i32 { return ch - 'a'; }
     auto constexpr chr(i32 index) -> char { return static_cast<char>(index + 'a'); }
 
     // 某次操作之后的某个字母
     struct PositionedLetter {
         i32 pos;
         char ch;
 
         auto getIndex() const -> i32 {
             return pos * charsetSize + ord(ch);
         }
     };
 
     class Tree {
         struct Node {
             PositionedLetter data{};
             std::vector<i32> children{};
             i64 size = 0;  // 叶子节点数量
             i64 static constexpr maxSize = 0x3f3f3f3f3f3f3f3fLL;  // 大于这个 size 的将会被视为这个值
         };
 
         std::vector<Node> nodes;
 
         auto getSizeRecursion(i32 p, std::vector<char> &vis) -> void {
             if (vis[p]) return;
             vis[p] = true;
             // std::cerr << "getSizeRecursion " << p << std::endl;
             i64 size = 0;
             bool isLeaf = true;
             for (auto child: nodes[p].children) {
                 isLeaf = false;
                 getSizeRecursion(child, vis);
                 size += nodes[child].size;
                 if (size >= Node::maxSize) {
                     size = Node::maxSize;
                     break;
                 }
             }
             nodes[p].size = size + isLeaf;
         }
 
         auto walkRecursion(i64 begin, i64 end, i32 p, i64 node_begin, i64 node_end, std::string &res) -> void {
             // std::cerr << "walk" << p << " " << node_begin << " " << node_end << std::endl;
             if (node_end - node_begin == 1) {
                 // 叶子节点
                 assert(begin <= node_begin and node_end <= end);
                 res.push_back(nodes[p].data.ch);
                 return;
             }
 
             i64 it = node_begin;
             for (auto child: nodes[p].children) {
                 auto last = it + nodes[child].size;
                 if (last >= Node::maxSize) last = Node::maxSize;
                 if (last > begin and it < end) {
                     walkRecursion(begin, end, child, it, last, res);
                 }
                 if (last >= Node::maxSize) break;
                 it = last;
             }
         }
     public:
         Tree(i32 maxPos): nodes(static_cast<i32>(charsetSize * (maxPos + 1))) {
             for (i32 p = 0; p <= maxPos; p++) {
                 for (i32 chIndex = 0; chIndex < charsetSize; chIndex++) {
                     PositionedLetter letter{.pos = p, .ch = chr(chIndex)};
                     nodes[letter.getIndex()].data = letter;
                 }
             }
         }
 
         // 执行替换逻辑
         // 需要保证 pos 没有在之前出现过
         auto replace(i32 pos, char from, std::string const &to) -> void {
             for (i32 chIndex = 0; chIndex < charsetSize; chIndex++) {
                 PositionedLetter letter{.pos = pos, .ch = chr(chIndex)};
                 if (letter.ch == from) {
                     // 逐个字符处理
                     nodes[letter.getIndex()].children.reserve(to.size());
                     for (auto ch: to) {
                         PositionedLetter next{.pos = pos + 1, .ch = ch};
                         nodes[letter.getIndex()].children.push_back(next.getIndex());
                     }
                 } else {
                     // 直接向下连
                     PositionedLetter next{.pos = pos + 1, .ch = chr(chIndex)};
                     nodes[letter.getIndex()].children.push_back(next.getIndex());
                 }
             }
         }
 
         // 为 (0, ch) 及其子树获取正确的 size
         auto getSize(char ch) -> void {
             PositionedLetter letter{.pos = 0, .ch = ch};
             std::vector<char> vis(nodes.size(), false);
             getSizeRecursion(letter.getIndex(), vis);
         }
 
         // 获取 ch 经过若干次替换后的 [begin, end) 区间
         auto walk(char ch, i64 begin, i64 end) -> std::string {
             std::string res;
             PositionedLetter letter{.pos = 0, .ch = ch};
             getSize(ch);
             walkRecursion(begin, end, letter.getIndex(), 0, nodes[letter.getIndex()].size, res);
             return res;
         }
     };
 
     auto solve() -> void {
         i64 l, r;
         i32 opCount;
         std::cin >> l >> r >> opCount;
 
         Tree tr{opCount};
         for (i32 pos = 0; pos < opCount; pos++) {
             char from;
             std::string to;
 
             std::cin >> from >> to;
             tr.replace(pos, from, to);
         }
 
         std::cout << tr.walk('a', l - 1, r) << endl;
     }
 }
 
 auto main(int argc, char const *argv[]) -> int {
     DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
     std::ios::sync_with_stdio(false);
     std::cin.tie(nullptr), std::cout.tie(nullptr);
 
     Solution_5242390942628612::solve();
     return 0;
 }
 