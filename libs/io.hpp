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
            static char st[std::numeric_limits<T>::max_exponent10+1];
            char *top = st;
            if (x < 0)  x = -x, put('-');
            if (std::isinf(x))  return write("Infinity");
            auto y = std::floor(x);
            while (y >= 1) {
                auto cur = y - (std::floor(y / 10) * 10);
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
    template <size_t MaxSize>
    struct DefaultIO: public FileReadScanner<MaxSize>, FileWritePrinter<MaxSize> {};
    DefaultIO<1<<20> io;
#endif  // def __linux__
}
