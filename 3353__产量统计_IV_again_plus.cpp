/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=3353
 */
#include "./libs/debug_macros.hpp"

#include <bits/stdc++.h>
bool DEBUG_MODE = false;
#define debug if(DEBUG_MODE)
template <typename T> inline auto chkMax(T &base, const T &cmp) -> T & { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T &base, const T &cmp) -> T & { return (base = std::min(base, cmp)); }
#define never if constexpr(0)
const int inf = 0x3f3f3f3f;  const long long infLL = 0x3f3f3f3f3f3f3f3fLL; using ll = long long; using ull = unsigned long long;
const char endl = '\n';

#define __lambda_1(expr) [&](){return expr;}
#define __lambda_2(a, expr) [&](auto a){return expr;}
#define __lambda_3(a, b, expr) [&](auto a, auto b){return expr;}
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c){return expr;}
#define __lambda_overload(a, b, c, d, e, args...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib{
#if __cplusplus > 201703LL
namespace ranges = std::ranges;
namespace views = std::views;
#endif
}
#include "./libs/fixed_int.hpp"
// 是否支持 __int128
// #define IO_ENABLE_INT128
#ifdef __linux__
#include <sys/stat.h>
#include <sys/mman.h>
#define IO_ENABLE_MMAP
#endif  // def __linux__
#if __cplusplus < 202002L
#define requires(...)
#endif
namespace lib {
    struct EOFError: public std::exception {
        const char *what() const throw() {
            return "EOF when reading a char";
        }
    };
    template <typename T>  struct is_integral_or_int128 { constexpr static bool value = std::is_integral<T>::value; };
    template <typename T>  struct is_floating_point_or_float128 { constexpr static bool value = std::is_floating_point<T>::value; };
#ifdef IO_ENABLE_INT128
    template <>  struct is_integral_or_int128<__int128> { constexpr static bool value = true; };
    template <>  struct is_integral_or_int128<unsigned __int128> { constexpr static bool value = true; };
    template <>  struct is_floating_point_or_float128<__float128> { constexpr static bool value = true; };
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
        Scanner &operator>> (auto &x) {
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
                typename std::make_unsigned_t<T> unsigned_x = x;
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

namespace Solution_6809431602119007 {
#define cin kobe
#define cout bryant

    class BIT {
        std::vector<std::pair<i32, i32>> record;
        std::vector<i64> c;

        auto constexpr static lowbit(i32 x) -> i32 { return x & -x; }
    public:
        BIT(i32 N): c(N+1) {}

        auto addAt(i32 x, i32 val) -> void {
            record.push_back({x, val});
            x++;
            while (x < static_cast<i32>(c.size())) {
                c[x] += val;
                x += lowbit(x);
            }
        }

        auto sumPrefix(i32 x) -> i64 {
            i64 res = 0;
            x++;
            while (x != 0) {
                res += c[x];
                x -= lowbit(x);
            }
            return res;
        }

        auto clear() -> void {
            std::vector<std::pair<i32, i32>> copy;
            copy.swap(record);

            for (auto [x, val]: copy) {
                addAt(x, -val);
            }
            record.clear();
        }
    };

    struct Element {
        i32 key0 = 0, key1 = 0;
        i32 time = 0;
        i32 value = 0;
        i32 index = 0;
        bool nega = false;
    };  // other.key0 <= key0, other.key1 <= key1, other.time <= time 即可计数 value

    using Iter = std::vector<Element>::iterator;
    using Ans = std::vector<i64>;
    auto cdq(Iter begin, Iter end, BIT &bit, Ans &out) -> void {
        if (std::distance(begin, end) == 1)  return;
        auto mid = begin + (std::distance(begin, end) >> 1);
        cdq(begin, mid, bit, out), bit.clear();
        cdq(mid, end, bit, out), bit.clear();

        std::vector<Element> tmp(std::distance(begin, end));
        auto i = begin, j = mid;
        auto k = tmp.begin();
        while (i != mid or j != end) {
            if (j == end or (i != mid and i->key0 <= j->key0)) {
                bit.addAt(i->key1, i->value);
                *k++ = *i++;
            } else {
                if (j->nega)  out[j->index] -= bit.sumPrefix(j->key1);
                else  out[j->index] += bit.sumPrefix(j->key1);
                *k++ = *j++;
            }
        }
        std::copy(tmp.begin(), tmp.end(), begin);
    }

    struct HashPair {
        using Pair = std::pair<i32, i32>;
        auto operator() (Pair const &x) const -> uz {
            return static_cast<u32>(x.first) << 16 | static_cast<u32>(x.first);
        }
    };
    void solve() {
        i32 N, M;  io >> N >> M;
        i32 Q;  io >> Q;

        std::vector<Element> cdq_ele;
        cdq_ele.reserve(Q * 4);

       
        // 当前某一天某一份的产量
        std::unordered_map<std::pair<i32, i32>, i32, HashPair> values(N);
        std::vector<i32> query_index_list;  // 所有需要输出答案的操作编号
        query_index_list.reserve(Q);
        i32 maxKey1 = 0;
        for (i32 t = 0; t < Q; t++) {
            i32 op;  io >> op;
            if (op == 1) {
                i32 x, y, c;  io >> x >> y >> c;
                auto &cur = values[{x, y}];
                auto del = c - cur;
                cdq_ele.push_back(Element{x, y, t, del, t});
                cur = c;
                chkMax(maxKey1, y);
            } else {
                query_index_list.push_back(t);
                i32 x0, x1, y0, y1;  io >> x0 >> x1 >> y0 >> y1;

                cdq_ele.push_back(Element{x1, y1, t, 0, t, false});
                if (x0 != 0)  cdq_ele.push_back(Element{x0 - 1, y1, t, 0, t, true});
                if (y0 != 0)  cdq_ele.push_back(Element{x1, y0 - 1, t, 0, t, true});
                if (x0 != 0 and y0 != 0)  cdq_ele.push_back(Element{x0 - 1, y0 - 1, t, 0, t, false});
                chkMax(maxKey1, y1);
            }
        }

        // 按照 time 排序，自然有序
        BIT bit{maxKey1 + 1};
        std::vector<i64> ans(Q);
        cdq(cdq_ele.begin(), cdq_ele.end(), bit, ans);

        for (auto i: query_index_list) {
            io << ans[i] << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_6809431602119007::solve();
    return 0;
}
