/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=3399
 */
#include "./lib_v5.hpp"
#include "./libs/fixed_int.hpp"
// 是否支持 int128
// #define IO_ENABLE_INT128
#ifdef __linux__
#include <sys/stat.h>
#include <sys/mman.h>
#define IO_ENABLE_MMAP
#endif  // def __linux__
#if __cplusplus < 202002L
#define requires(...)
#endif
#ifdef IO_ENABLE_INT128
#ifdef __GNUC__
#define GCC_EXTENSION __extension__
#else  // not def __GCC__
#define GCC_EXTENSION
#endif  // def __GCC__
GCC_EXTENSION using int128_t = __int128;
GCC_EXTENSION using uint128_t = unsigned __int128;
GCC_EXTENSION using float128_t = __float128;
#endif  // def IO_ENABLE_INT128
namespace lib {
    using i16 = int16_t; using i32 = int32_t; using i64 = int64_t;
    using u16 = uint16_t; using u32 = uint32_t; using u64 = uint64_t; using uz = size_t;
    struct EOFError: public std::exception {
        const char *what() const throw() {
            return "EOF when reading a char";
        }
    }; 
    template <typename T>  struct is_integral_or_int128 { constexpr static bool value = std::is_integral<T>::value; };
    template <typename T>  struct is_floating_point_or_float128 { constexpr static bool value = std::is_floating_point<T>::value; };
    template <typename T>  struct make_unsigned { using type = std::make_unsigned_t<T>; };
#ifdef IO_ENABLE_INT128
    template <>  struct is_integral_or_int128<int128_t> { constexpr static bool value = true; };
    template <>  struct is_integral_or_int128<uint128_t> { constexpr static bool value = true; };
    template <>  struct is_floating_point_or_float128<float128_t> { constexpr static bool value = true; };
    template <>  struct make_unsigned<int128_t> { using type = uint128_t; };
#endif  // def IO_ENABLE_INT128
    template <typename T> struct is_number {
        constexpr static bool value = is_integral_or_int128<T>::value || is_floating_point_or_float128<T>::value;
    };

    struct Scanner {
        virtual char gc() = 0;
        static bool isDigit(char ch) { return '0' <= ch and ch <= '9'; }
        static bool isBlank(char ch) { return ch <= 32 or ch == 127; }
        template <typename T, typename std::enable_if<is_number<T>::value>::type* = nullptr>
        Scanner &read(T &x) {
            bool sign = false;  x = 0;  char ch = gc();
            for (; not isDigit(ch); ch = gc()) {
                if constexpr (not std::is_unsigned<T>::value) {
                    if (ch == '-')  sign = true;
                }
            }
            if (sign) {
                for (; isDigit(ch); ch = gc())  x = x * 10 - (ch ^ 48);
                if constexpr (is_integral_or_int128<T>::value)  return *this;
                double tmp = 1;
                if (ch == '.') {
                    for (ch = gc(); isDigit(ch); ch = gc()) tmp *= 0.1, x -= tmp * (ch ^ 48);
                }
            } else {
                for (; isDigit(ch); ch = gc())  x = x * 10 + (ch ^ 48);
                if constexpr (is_integral_or_int128<T>::value)  return *this;
                double tmp = 1;
                if (ch == '.') {
                    for (ch = gc(); isDigit(ch); ch = gc()) tmp *= 0.1, x += tmp * (ch ^ 48);
                }
            }
            if (ch == 'e' or ch == 'E') {
                int y;  read(y);
                x *= pow(10, y);
            }
            return *this;
        }
        Scanner &read(char &x) {
            for (x = gc(); isBlank(x); x = gc());
            return *this;
        }
        Scanner &read(char *s) {
            char ch = gc();
            for (; isBlank(ch); ch = gc());
            for (; not isBlank(ch); ch = gc())  *s++ = ch;
            *s = '\0';
            return *this;
        }
        Scanner &read(std::string &s, int reserve = 0) {
            char ch = gc();
            s.clear(), s.reserve(reserve);
            for (; isBlank(ch); ch = gc());
            for (; not isBlank(ch); ch = gc())  s.push_back(ch); 
            return *this;
        }
        Scanner &operator>> (auto &&x) {
            return read(x);
        }
        
    };
    template <size_t MaxSize>
    struct FileReadScanner: public Scanner {
        char buf[MaxSize], *p1, *p2;
        bool eofFlag = false;
        FileReadScanner(): p1(buf), p2(buf) {}

        char gc_fread() {
            if (p1 == p2) {
                if (eofFlag)  throw EOFError{};
                p1 = buf;
                p2 = buf + std::fread(buf, sizeof(char), sizeof(buf), stdin);
                if (std::feof(stdin)) eofFlag = true;
            }
            return p1 == p2? '\0': *p1++;
        }
        char gc() { return gc_fread(); }
    };
    struct GetCharScanner: public Scanner {
        char last = '\0'; 
        char gc_getchar() {
            char ch = getchar();
            if (ch != EOF)  return ch;
            else {
                throw EOFError{};
                return '\0';
            }
        }
        char gc() { return gc_getchar(); }
    };
#ifdef IO_ENABLE_MMAP
    struct MemoryMapScanner: public Scanner {
        struct stat s;
        char *c;
        MemoryMapScanner() {
            fstat(0, &s);
            c = (char *)mmap(nullptr, s.st_size, 1, 2, 0, 0);
        }
        char gc() { return *c++; }
    };
#endif  // def IO_ENABLE_MMAP
    struct Printer {
        virtual void put(char) = 0;
        template <typename T, typename std::enable_if<is_floating_point_or_float128<T>::value>::type* = nullptr>
        Printer &write(T x) {
            if (std::isnan(x))  return write("nan");
            static char st[std::numeric_limits<T>::max_exponent10+10];
            char *top = st;
            if (x < 0)  x = -x, put('-');
            if (std::isinf(x))  return write("Infinity");
            auto y = std::floor(x);
            while (y >= 1) {
                auto cur = std::fmod(y, 10);
                y = (y - cur) / 10;
                *top++ = (int)(cur) ^ 48;
            }
            if (top == st)  put('0');
            while (top != st)  put(*--top);
            x -= std::floor(x);
            put('.');
            for (auto i = 0; i < 6; i++) {
                x = x * 10;
                auto cur = std::floor(x);
                x -= cur;
                put((int)cur ^ 48);
            }
            return *this;
        }
        template <typename T, i32 x> struct pow10 { T static constexpr value = pow10<T, x - 1>::value * 10; };
        template <typename T> struct pow10<T, 0> { T static constexpr value = 1; };

        struct PrinterTable {
#if __cplusplus < 201703L
#define PRINTER_TABLE_CONSTEXPR
#else
#define PRINTER_TABLE_CONSTEXPR constexpr
#endif
            std::array<std::array<char, 4>, 10000> _numToString;
            std::array<char, 65536> _stringToNum;
            PRINTER_TABLE_CONSTEXPR PrinterTable() {
                std::memset(_stringToNum.begin(), -1, sizeof(_stringToNum));
                for (i32 i = 0; i <= 9; i++) {
                    for (i32 j = 0; j <= 9; j++) {
                        _stringToNum[(i + '0') << 8 | (j + '0')] = i * 10 + j;
                    }
                }
                for (i32 i = 0; i < 10000; i++) {
                    for (i32 j = 4, k = i; j--; ) {
                        _numToString[i][j] = k % 10 + 48, k /= 10;
                    }
                }
            }
            auto PRINTER_TABLE_CONSTEXPR numToString(i32 x) -> char const * {
                return _numToString[x].begin();
            }
            auto PRINTER_TABLE_CONSTEXPR stringToNum(u32 x) -> char {
                return _stringToNum[x];
            }
        } printerTable;

        // 保证 1 <= n <= 4
        auto putStringWithLength(char const *first, size_t n) -> char const * {
            switch (n) {
            case 4:  put(*first++);  [[fallthrough]];
            case 3:  put(*first++);  [[fallthrough]];
            case 2:  put(*first++);  [[fallthrough]];
            default:   put(*first++);
            }
            return first;
        }

        template <typename T, i32 digits>
        auto writeIntegerPow2(T x) -> void {  // x 的十进制位数为 2 的幂次
            if constexpr(digits == 4) {
                putStringWithLength(printerTable.numToString(x), 4);
            } else {
                writeIntegerPow2<T, digits / 2>(x / pow10<T, digits / 2>::value);
                writeIntegerPow2<T, digits / 2>(x % pow10<T, digits / 2>::value);
            }
        }

        template <typename T, i32 digits>
        auto writeIntegerAny(T x) -> void {  // x 的十进制位数不大于 digits
            if constexpr(digits <= 4) {
                i32 d = (digits >= 4 and x >= 1000) + (digits >= 3 and x >= 100) + (digits >= 2 and x >= 10) + 1;
                putStringWithLength(printerTable.numToString(x) + (4 - (d)), (d));
            } else {
                i32 constexpr near = 1 << std::__lg(digits - 1);  // 取一个 2 的若干次方
                if (x >= pow10<T, near>::value) {
                    writeIntegerAny<T, digits - near>(x / pow10<T, near>::value);
                    writeIntegerPow2<T, near>(x % pow10<T, near>::value);
                } else {
                    writeIntegerAny<T, near>(x);
                }
            }
        }

        template <typename T>
        auto writeUnsignedInteger(T x) -> void {
            return writeIntegerAny<T, std::numeric_limits<T>::digits10 + 1>(x);
        }

        template <typename T>
        auto writeSignedInteger(T x) -> void {
            if (x < 0) {
                typename make_unsigned<T>::type unsigned_x = x;
                unsigned_x = -unsigned_x;
                put('-'), writeUnsignedInteger(unsigned_x);
            } else {
                writeUnsignedInteger(x);
            }
        }
        template <typename T, typename std::enable_if<is_integral_or_int128<T>::value>::type* = nullptr>
        Printer &write(T x) {
            if constexpr (std::is_unsigned<T>::value) {
                writeUnsignedInteger(x);
            } else {
                writeSignedInteger(x);
            }
            return *this;
        }
        Printer &write(char ch) {
            put(ch);
            return *this;
        }
        Printer &write(const char *s) {
            for (; *s; s++)  put(*s);
            return *this;
        }
        Printer &write(std::string const &s) {
            for (auto ch: s)  put(ch);
            return *this;
        }
        template <typename T>
        Printer &operator<< (const T &x) {
            if constexpr (
                is_number<T>::value || std::is_convertible<T, const char *>::value || std::is_convertible<T, std::string const &>::value
            ) {
                return write(x);
            } else {
                std::stringstream stream;
                stream << x;
                return write(stream.str());
            }
        }
    };
    struct PutCharPrinter: public Printer {
        void put(char ch) {
            std::putchar(ch);
        }
    };
    template <size_t MaxSize>
    struct FileWritePrinter: public Printer {
        char pbuf[MaxSize], *pp;
        FileWritePrinter(): pp(pbuf) {}
        ~FileWritePrinter() {
            std::fwrite(pbuf, 1, pp - pbuf, stdout);
        }
        void flush() {
            std::fwrite(pbuf, 1, MaxSize, stdout);
            pp = pbuf;
        }
        void put(char ch) {
            if (pp - pbuf == MaxSize)  flush();
            *pp++ = ch;
        }
    };

#ifdef __linux__
    template <size_t MaxSize>
    struct DefaultIO: public MemoryMapScanner, FileWritePrinter<MaxSize> {};
    DefaultIO<1<<20> io;
#else  // not def __linux__
    template <size_t MaxSize>
    struct DefaultIO: public FileReadScanner<MaxSize>, FileWritePrinter<MaxSize> {};
    DefaultIO<1<<20> io;
#endif  // def __linux__
}
using namespace lib;

namespace Solution_3593834255407005 {
    i32 constexpr maxVal = 3;
    i32 constexpr maxRevPairIndex = 3;
    using i8 = std::int8_t;
    template <typename T>
    auto copy(T const &x) -> T { return x; }
    class SegTree {
        struct Node {
            i32 begin = 0, end = 0;
            std::array<i32, maxVal> cnt;
            std::array<i8, maxVal> replace_tag;  // tag 以 -1 为无效值
            std::array<std::array<i64, maxVal>, maxVal> pair_cnt;

            Node() {
                cnt.fill(0), replace_tag.fill(-1);

                for (auto &line: pair_cnt) {
                    for (auto &item: line) {
                        item = 0;
                    }
                }
            }

            auto empty() const -> bool { return begin == end; }

            auto replace(i8 from, i8 to) -> void {
                auto new_pair_cnt = pair_cnt;  // 重新计数各种对的数量
                for (i32 other = 0; other < maxVal; other++) {
                    new_pair_cnt[other][from] = 0;
                    new_pair_cnt[from][other] = 0;
                    new_pair_cnt[other][to] += pair_cnt[other][from];
                    new_pair_cnt[to][other] += pair_cnt[from][other];
                }
                pair_cnt = new_pair_cnt;

                cnt[to] += cnt[from];
                cnt[from] = 0;
                replace_tag[from] = to;
            }

            auto multipleReplace(std::array<i8, maxVal> targets) -> void {
                auto new_tag = replace_tag;
                for (i32 i = 0; i < maxVal; i++) {
                    if (new_tag[i] == -1) {
                        new_tag[i] = targets[i];
                    } else {
                        new_tag[i] = targets[new_tag[i]];
                    }
                }
                decltype(pair_cnt) new_pair_cnt;
                std::memset(new_pair_cnt.data(), 0, sizeof(new_pair_cnt));
                for (i32 i = 0; i < maxVal; i++) {
                    for (i32 j = 0; j < maxVal; j++) {
                        new_pair_cnt[targets[i]][targets[j]] += pair_cnt[i][j];
                    }
                }
                pair_cnt = new_pair_cnt;

                std::array<i32, maxVal> new_cnt;
                new_cnt.fill(0);
                for (i32 from = 0; from < maxVal; from++) {
                    auto to = targets[from];
                    new_cnt[to] += cnt[from];
                }
                cnt = new_cnt;

                replace_tag = new_tag;
            }

            auto countRevPair() const -> i64 {
                i64 ans = 0;
                ans += pair_cnt[1][0];
                ans += pair_cnt[2][0];
                ans += pair_cnt[2][1];
                return ans;
            }

            auto merge_from(Node const &lhs, Node const &rhs) -> Node & {
                if (lhs.empty()) return *this = rhs;
                if (rhs.empty()) return *this = lhs;
                for (i32 i = 0; i < maxVal; i++) {
                    cnt[i] = lhs.cnt[i] + rhs.cnt[i];
                }
                for (i32 i = 0; i < maxVal; i++) {
                    for (i32 j = 0; j < maxVal; j++) {
                        pair_cnt[i][j] = lhs.pair_cnt[i][j] + rhs.pair_cnt[i][j];  // 各自贡献
                        pair_cnt[i][j] += static_cast<i64>(lhs.cnt[i]) * rhs.cnt[j];  // 中间贡献
                    }
                }
                for (i32 i = 0; i < maxVal; i++) {
                    replace_tag[i] = -1;
                }
                return *this;
            }
        };
        i32 N;
        std::vector<Node> tree;

        auto static constexpr lson(i32 p) -> i32 { return p << 1; }
        auto static constexpr rson(i32 p) -> i32 { return p << 1 | 1; }

        auto pushDown(i32 p) -> void {
            auto targets = tree[p].replace_tag;
            if (std::any_of(targets.begin(), targets.end(), lam(x, x != -1))) {
                for (i32 i = 0; i < maxVal; i++) {
                    if (targets[i] == -1) targets[i] = static_cast<i8>(i);
                }
                tree[lson(p)].multipleReplace(targets);
                tree[rson(p)].multipleReplace(targets);
                tree[p].replace_tag.fill(-1);
            }
        }
        auto pushUp(i32 p) -> void {
            tree[p].merge_from(tree[lson(p)], tree[rson(p)]);
        }
        auto build(i32 begin, i32 end, i32 p, std::vector<i8> const &init) -> void {
            tree[p].begin = begin, tree[p].end = end;
            if (end - begin == 1) {
                tree[p].cnt[init[begin]]++;
                return;
            }
            auto mid = begin + ((end - begin) >> 1);
            build(begin, mid, lson(p), init);
            build(mid, end, rson(p), init);
            pushUp(p);
        }
    public:
        SegTree(i32 N, std::vector<i8> const &init): N(N), tree(N << 2) {
            build(0, N, 1, init);
        }
        auto queryRange(i32 begin, i32 end, i32 p = 1) -> Node {
            if (tree[p].begin >= begin and tree[p].end <= end) {
                return tree[p];
            }
            pushDown(p);
            Node res{};
            if (tree[lson(p)].end > begin) res.merge_from(copy(res), queryRange(begin, end, lson(p)));
            if (tree[rson(p)].begin < end) res.merge_from(copy(res), queryRange(begin, end, rson(p)));
            return res;
        }
        auto replaceRange(i32 begin, i32 end, i8 from, i8 to, i32 p = 1) -> void {
            if (tree[p].begin >= begin and tree[p].end <= end) {
                tree[p].replace(from, to);
                return;
            }
            pushDown(p);
            if (tree[lson(p)].end > begin) replaceRange(begin, end, from, to, lson(p));
            if (tree[rson(p)].begin < end) replaceRange(begin, end, from, to, rson(p));
            pushUp(p);
        }
        auto multipleReplaceRange(i32 begin, i32 end, std::array<i8, maxVal> targets, i32 p = 1) -> void {
            if (tree[p].begin >= begin and tree[p].end <= end) {
                tree[p].multipleReplace(targets);
                return;
            }
            pushDown(p);
            if (tree[lson(p)].end > begin) multipleReplaceRange(begin, end, targets, lson(p));
            if (tree[rson(p)].begin < end) multipleReplaceRange(begin, end, targets, rson(p));
            pushUp(p);
        }
        auto getAt(i32 pos, i32 p = 1) -> i8 {
            if (tree[p].end - tree[p].begin == 1) {
                return std::distance(
                    tree[p].cnt.begin(),
                    std::max_element(tree[p].cnt.begin(), tree[p].cnt.end())
                );
            }
            pushDown(p);
            if (tree[lson(p)].end > pos) return getAt(pos, lson(p));
            else return getAt(pos, rson(p));
        }
        auto print() -> void {
            for (i32 i = 0; i < N; i++) {
                std::cout << static_cast<i32>(getAt(i)) << ' ';
            }
            std::cout << std::endl;
        }
    };
    auto solve() -> void {
        i32 N, Q; io >> N >> Q;
        std::vector<i8> init(N);
        for (auto &x: init) {
            i32 tmp; io >> tmp;
            x = static_cast<i8>(tmp);
        }

        SegTree sgt{N, init};
        for (auto q = Q; q --> 0; ) {
            i32 op, l, r;
            io >> op >> l >> r;
            l--, r--;

            if (op == 1) {
                auto ans = sgt.queryRange(l, r + 1).countRevPair();
                io << ans << endl;
            } else {
                i32 x, y, z;
                io >> x >> y >> z;

                sgt.multipleReplaceRange(l, r + 1, {{static_cast<i8>(x), static_cast<i8>(y), static_cast<i8>(z)}});
            }
            // sgt.print();
        }
    }
}

auto main(int argc, char const *argv[]) -> int {
    DEBUG_MODE = (argc != 1) and (std::strcmp("-d", argv[1]) == 0);
    Solution_3593834255407005::solve();
    return 0;
}
