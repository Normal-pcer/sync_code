/**
 * @link https://neooj.com:8082/oldoj/problem.php?id=3361
 */
#include "./libs/debug_macros.hpp"

#include "./lib_v4.hpp"

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
    using i16 = int16_t; using i32 = int32_t; using i64 = int64_t;
    using u16 = uint16_t; using u32 = uint32_t; using u64 = uint64_t; using uz = size_t;
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
                if (ch == '-')  sign = true;

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
            std::array<std::array<char, 4>, 10000> _numToString;
            std::array<char, 65536> _stringToNum;
            PrinterTable() {
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
            auto numToString(i32 x) -> char const * {
                return _numToString[x].begin();
            }
            auto stringToNum(u32 x) -> char {
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
                putchar('-'), writeUnsignedInteger(unsigned_x);
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

/*
考虑求多少个区间的 gcd 为 x
固定区间的左端点为 l，区间右端点向右移动。
这个过程中，区间 gcd 一定不减。
可以二分查找最小和最大的区间。

对于查询的每个 gcd 都可以这么做，40% 的数据 gcd 的种类不会太多，40% 的数据查询次数不会太多。
单次复杂度是 O(N*logN*logN)
加个记忆化感觉可以获得 80 分。
经过验证，手动构造方案，N = 3000 还是可以过的。

选定了一个右端点，接下来向右最多会有 log 种区间 gcd（每一次至少减半）
可以直接预处理，复杂度为 O(N*logN*logN)（gcd 越来越小，均摊复杂度 O(1)）
*/
namespace Solution_7409812912917923 {
    template <typename T>  T gcd(T x, T y) { return std::__gcd(x, y); }
    template <typename T, typename U, typename Func, typename Comp = std::less<>>
    auto lower_bound_mapping(T begin, T end, U val, Func &&mapping, Comp cmp = {}) -> T {
        while (end - begin >= 1) {
            auto mid = begin + ((end - begin) >> 1);
            if (cmp(mapping(mid), val)) {
                begin = mid + 1;
            } else {
                end = mid;
            }
        }
        return begin;
    }
    template <typename T, typename U, typename Func, typename Comp = std::less<>>
    auto upper_bound_mapping(T begin, T end, U val, Func &&mapping, Comp cmp = {}) -> T {
        while (end - begin >= 1) {
            auto mid = begin + ((end - begin) >> 1);
            if (not cmp(val, mapping(mid))) {
                begin = mid + 1;
            } else {
                end = mid;
            }
        }
        return begin;
    }
#define cin kobe
#define cout bryant
    auto solve() -> void {
        i32 N;  io >> N;
        std::vector<i32> a(N);
        for (auto &x: a)  io >> x;

        // F[j][i] 表示 [i, i + 1 << j) 个数的区间 gcd
        auto log_N = std::__lg(N);
        std::vector<std::vector<i32>> F(log_N + 1, std::vector<i32>(N));
        F[0] = a;
        for (i32 j = 1; j <= log_N; j++) {
            for (i32 i = 0; i < N; i++) {
                if (i + (1 << j) > N)  break;
                F[j][i] = gcd(F[j - 1][i], F[j - 1][i + (1 << (j - 1))]);
            }
        }

        auto query = [&](i32 begin, i32 end) -> i32 {
            if (begin == end)  return 0x3f3f3f3f;
            auto dis = end - begin;
            auto log_dis = std::__lg(dis);

            return gcd(F[log_dis][begin], F[log_dis][end - (1 << log_dis)]);
        };

        std::unordered_map<i32, i64> cnt;  // gcd 为 i 的区间数量
        {
            // 枚举左端点
            for (i32 l = 0; l < N; l++) {
                auto f = [&](i32 x) { return x == N+1? 1: query(l, x); };
                auto cur = query(l, l + 1);
                auto begin = l + 1;
                while (cur != 1) {
                    // [begin, end) 为右端点，区间 gcd 为 cur
                    auto end = upper_bound_mapping(l + 1, N + 1, cur, f, std::greater<>{});
                    auto len = end - begin;
                    cnt[cur] += len;
                    begin = end, cur = f(end);
                }
                cnt[1] += N - begin + 1;  // 最后一个区间为 1
            }
        }

        i32 Q;  io >> Q;
        while (Q --> 0) {
            i32 l, r;  io >> l >> r;
            l--, r--;  // 下标从 0 开始
            auto x = query(l, r + 1);
            io << x << " " << cnt[x] << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    Solution_7409812912917923::solve();
    return 0;
}
