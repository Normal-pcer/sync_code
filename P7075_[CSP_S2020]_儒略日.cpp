/**
 * @link https://www.luogu.com.cn/problem/P7075
 */

#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=(n);i++)
#define from(i,b,e) for(int i=(b);i<=(e);i++)
#define rev(i,e,b) for(int i=(e);i>=(b);i--)

#define optimizeIO std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);
#define chkMax(base,cmp...) (base=std::max({(base),##cmp}))
#define chkMin(base,cmp...) (base=std::min({(base),##cmp}))
#define chkMaxEx(base,exchange,other,cmp...) {auto __b__=base;if(__b__!=chkMax(base,##cmp)){exchange;} else other;}
#define chkMinEx(base,exchange,other,cmp...) {auto __b__=base;if(__b__!=chkMin(base,##cmp)){exchange;} else other;}
#define ensure(v, con, otw) (((v) con)? (v): (otw))
#define never if constexpr(0)
#define always if constexpr(1)
#define bitOr(x,y) (((x)&(y))^(((x)^(y))|(~(x)&(y))))
#define Infinity 2147483647
#define compare(x,y,g,e,l) (((x)>(y))?(g):(((x)<(y))?(l):(e)))
bool DEBUG_MODE=false;
typedef long long ll; typedef unsigned long long ull;
inline void batchOutput(int *begin, int n, const char *format){upto(i, n)printf(format, begin[i]);printf("\n");} inline void batchOutput(int*begin, int n) {batchOutput(begin,n,"%3d ");}
#define batchOutput2d(b, r, c, fmt) upto(i,r){upto(j,c)printf(fmt,b[i][j]);printf("\n");}
namespace lib{}
#include <bits/stdc++.h>
// 是否支持 __int128
#define IO_ENABLE_INT128
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
        virtual void flush() {}
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
        template <typename T, typename std::enable_if<is_integral_or_int128<T>::value>::type* = nullptr>
        Printer &write(T x) {
            static char st[std::numeric_limits<T>::digits10+1];
            char *top = st;
            if (x < 0) {
                put('-');
                *top++ = -(x % 10) ^ 48, x = -(x / 10);
                if (x == 0) {
                    put(*--top);
                    return *this;
                }
            }
            do {
                *top++ = x % 10 ^ 48, x /= 10;
            } while (x);
            while (top != st)  put(*--top);
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
    struct DefaultIO: public GetCharScanner, PutCharPrinter {};
    DefaultIO io;
#endif  // def __linux__
}

using namespace lib;

namespace Solution {

    int Q;

    const int AverageYearsMonth_negative[] = {-365, -334, -304, -273, -243, -212, -181, -151, -120, -90, -59, -31, 0};
    const int LeapYearsMonth_negative[] = {-366, -335, -305, -274, -244, -213, -182, -152, -121, -91, -60, -31, 0};
    
    bool isLeap(int year, bool usingGregorian=true) {
        if (year<0)  year++;
        if (not usingGregorian)
            return (year%4 == 0);
        else
            return (year%400 == 0) or (year%100!=0 and year%4==0);
    }

    void init() {

    }

    // 计算年内的某个日期
    // 传入：从某年的 1/1 到目标日期的天数，包含两个端点
    // 返回：月，日
    std::pair<int, int> calculateDateInYear(ll days, bool leap=false) {
        const int *YearMonth = leap? LeapYearsMonth_negative: AverageYearsMonth_negative;
        auto monthToDecember = std::upper_bound(YearMonth, YearMonth+13, -days) - YearMonth;  // 和 12 月的差距
        auto month = 13 - monthToDecember;  // 月份标号
        
        auto preventMonthPassedDays = -YearMonth[monthToDecember];  // 先前的月份经过的天数
        auto currentDay = days - preventMonthPassedDays;  // 本月经过的天数

        return {month, currentDay};
    }

    void BC(ll julian) {
        auto daysNegativeCount = 1721424 - julian;  // 到 1/1/1 的天数
        auto termsCount = daysNegativeCount / 1461;  // 周期（4 年）数量
        auto yearAbs = termsCount * 4;  // 年份的绝对值
        daysNegativeCount %= 1461;  // 向前拨回若干周期后剩余的天数；到某年的 1 月 1 日，该年份绝对值是 4 的倍数
        auto newDays = daysNegativeCount;  // 再拨回一年，这一年一定是一个闰年
        while (newDays > 0) {
            newDays = newDays - 365 - isLeap(-yearAbs-1, false);
            yearAbs++;
        }

        int dayIndex = 0 - newDays;  // 目标日期到那年 1 月 1 日的天数
        dayIndex++;  // 包含那年的一月一日

        auto [month, day] = calculateDateInYear(dayIndex, isLeap(-yearAbs, false));
        printf("%d %d %lld BC\n", day, month, yearAbs);
    }

    void DC_WithJulianCalender(ll julian) {
        auto daysCount = julian - 1721424;  // 到 1/1/1 的天数
        auto termsCount = daysCount / 1461;  // 四年周期
        auto years = termsCount * 4 + 1;  // 此时是这一年的 1/1
        daysCount %= 1461;
        auto newDays = daysCount + 1;
        // 向后拨若干年，直到这个数大于等于 0 时的最小值
        while (newDays - 365 - isLeap(years, false) > 0) {
            newDays = newDays - 365 - isLeap(years, false);
            years++;
        }
        auto [month, day] = calculateDateInYear(newDays, isLeap(years, false));
        printf("%d %d %lld\n", day, month, years);
    }

    void DC_WithGregorianCalender(ll julian) {
        // auto daysFromBreaking = 2299160;
        const auto newYearJulianDay = 2299249-10;  // 1583/1/1 的儒略日
        if (julian < newYearJulianDay) {
            DC_WithJulianCalender(julian+10);
            return;
        }
        auto daysAfterNewYear = julian - newYearJulianDay + 1;  // 从 1583/1/1 开始
        // 先暴力推 17 年
        auto year = 1583LL;  // 当前处于 year/1/1
        while (daysAfterNewYear - 365 - isLeap(year) > 0) {
            daysAfterNewYear = daysAfterNewYear - 365 - isLeap(year);
            year++;
            if (year == 1601)  break;
        }
        if (year > 1600) {
            daysAfterNewYear--;  // ! 不包含 1 月 1 日
            // 对于一个 400 年的大周期
            const auto bigTermLength = 146097;
            auto bigTermCount = daysAfterNewYear / bigTermLength;
            daysAfterNewYear %= bigTermLength;
            year += bigTermCount * 400; assert(year%400==1);

            // 直接特判 400 年的倍数
            if (daysAfterNewYear >= bigTermLength - 366) {
                year += 399;
                auto [month, day] = calculateDateInYear(daysAfterNewYear - bigTermLength + 366 + 1, 1);
                printf("%d %d %lld\n", day, month, year);
                return;
            }

            // 100 年的中等周期
            const auto midTermLength = 36524;
            auto midTermCount = daysAfterNewYear / midTermLength;
            daysAfterNewYear %= midTermLength;
            year += midTermCount * 100;

            // 特判 100 的倍数
            if (daysAfterNewYear >= midTermLength - 365) {
                year += 99;
                auto [month, day] = calculateDateInYear(daysAfterNewYear - midTermLength + 365 + 1, 0);
                printf("%d %d %lld\n", day, month, year);
                return;
            }

            // 4 年
            const auto littleTermLength = 1461;
            auto littleTermCount = daysAfterNewYear / littleTermLength;
            daysAfterNewYear %= littleTermLength;
            year += littleTermCount * 4;
            while (daysAfterNewYear - 365 - isLeap(year) >= 0) {
                daysAfterNewYear = daysAfterNewYear - 365 - isLeap(year);
                year++;
            }
            daysAfterNewYear++;
            auto [month, day] = calculateDateInYear(daysAfterNewYear, isLeap(year));
            printf("%d %d %lld\n", day, month, year);
        } else {
            auto [month, day] = calculateDateInYear(daysAfterNewYear, isLeap(year));
            printf("%d %d %lld\n", day, month, year);
        }
    }

    void solve() {
        init();
        io >> Q;
        ll x;

        // auto test_ret = calculateDateInYear(15, true);
        // log("%d %d\n", test_ret.first, test_ret.second);
        from(_, 1, Q) {
            io >> x;
            
            // 1/1/1 BC 及以前
            if (x < 1721424) {
                BC(x);
            } else if (x <= 2299160LL) {
                DC_WithJulianCalender(x);
            } else {
                DC_WithGregorianCalender(x);
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    never {
        freopen("julian3.in", "r", stdin);
        freopen("julian3.out", "w", stdout);
    }
    Solution::solve();
    return 0;
}