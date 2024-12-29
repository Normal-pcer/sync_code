/**
 * @link https://www.luogu.com.cn/problem/P3695
 */

#include "./lib_v3.hpp"

using namespace lib;

namespace IO {
#ifdef __linux__
#include <sys/stat.h>
#include <sys/mman.h>
#define IO_ENABLE_MMAP
#endif  // def __linux__
#if __cplusplus < 202002L
#define requires(...)
#endif
    struct EOFError: public std::exception {
        const char *what() const throw() {
            return "EOF when reading a char";
        }
    };
    struct IntegerOverflowError: public std::exception {
        const char *what() const throw() override {
            return "Integer Overflow";
        }
    };
    template <typename T>  struct is_integral_or_int128 { constexpr static bool value = std::is_integral<T>::value; };
    template <>  struct is_integral_or_int128<__int128> { constexpr static bool value = true; };
    template <>  struct is_integral_or_int128<unsigned __int128> { constexpr static bool value = true; };
    template <typename T>  struct is_floating_point_or_float128 { constexpr static bool value = std::is_floating_point<T>::value; };
    template <>  struct is_floating_point_or_float128<__float128> { constexpr static bool value = true; };
    template <typename T> struct is_number {
        constexpr static bool value = is_integral_or_int128<T>::value || is_floating_point_or_float128<T>::value;
    };

    bool addOverflow(auto &x, auto y) {
        return __builtin_add_overflow(x, y, &x);
    }
    bool subOverflow(auto &x, auto y) {
        return __builtin_sub_overflow(x, y, &x);
    }
    bool mulOverflow(auto &x, auto y) {
        return __builtin_mul_overflow(x, y, &x);
    }
    template <typename T, typename U>
    bool leftShiftOverflow(T &x, U y) {
        if (x == 0)  return false;
        return x <<= y, std::__lg(x) + y >= std::numeric_limits<T>::digits;
    }

    struct Scanner {
    private:
        char prev = '\0';
        bool ungetFlag = false;
        virtual char gc() = 0;
        static bool isDigit(char ch) { return '0' <= ch and ch <= '9'; }
        static bool isBlank(char ch) { return ch <= 32 or ch == 127; }
    public:
        char get() {
            if (ungetFlag) {
                ungetFlag = false;
                return prev;
            }
            return (prev = gc());
        }
        Scanner &unget() {
            if (ungetFlag)  throw std::logic_error("Cannot unget twice");
            ungetFlag = true;
            return *this;
        }
        template <typename T, typename std::enable_if<is_number<T>::value>::type* = nullptr, int base = 10>
        Scanner &read(T &x) {
            bool sign = false;  x = 0;  char ch = get();
            for (; not isDigit(ch); ch = get()) {
                if (ch == '-')  sign = true;
                if constexpr (is_floating_point_or_float128<T>::value) {
                    if (ch == '.')  break;
                }
            }
            if (sign) {
                for (; isDigit(ch); ch = get()) {
                    // Check overflow
                    if constexpr (is_integral_or_int128<T>::value) {
                        if (mulOverflow(x, 10))  throw IntegerOverflowError{};
                        if (subOverflow(x, ch ^ 48))  throw IntegerOverflowError{};
                    } else {
                        x = x * 10 - (ch ^ 48);
                    }
                }
                if constexpr (is_integral_or_int128<T>::value)  return unget(), *this;
                T tmp = 1;
                if (ch == '.') {
                    for (ch = get(); isDigit(ch); ch = get()) {
                        tmp *= 0.1, x -= tmp * (ch ^ 48);
                    }
                }
            } else {
                for (; isDigit(ch); ch = get()) {
                    // Check overflow
                    if constexpr (is_integral_or_int128<T>::value) {
                        if (mulOverflow(x, 10))  throw IntegerOverflowError{};
                        if (addOverflow(x, ch ^ 48))  throw IntegerOverflowError{};
                    } else {
                        x = x * 10 + (ch ^ 48);
                    }
                }
                if constexpr (is_integral_or_int128<T>::value)  return unget(), *this;
                T tmp = 1;
                if (ch == '.') {
                    for (ch = get(); isDigit(ch); ch = get()) {
                        tmp *= 0.1, x += tmp * (ch ^ 48);
                    }
                }
            }
            if (ch == 'e' or ch == 'E') {
                int y;  read(y);
                x *= pow(10, y);
            }
            return unget(), *this;
        }
        Scanner &read(char &x) {
            for (x = get(); isBlank(x); x = get());
            return *this;
        }
        Scanner &read(char *s) {
            char ch = get();
            for (; isBlank(ch); ch = get());
            for (; not isBlank(ch); ch = get())  *s++ = ch;
            *s = '\0';
            return unget(), *this;
        }
        Scanner &read(std::string &s, int reserve = 0) {
            char ch = get();
            s.clear(), s.reserve(reserve);
            for (; isBlank(ch); ch = get());
            for (; not isBlank(ch); ch = get())  s.push_back(ch); 
            return unget(), *this;
        }
        template <typename T, typename std::enable_if<
                is_number<T>::value || std::is_convertible<T, const char *>::value || std::is_convertible<T, std::string const &>::value
            >::type* = nullptr
        >
        Scanner &operator>> (T &x) {
            return read(x);
        }
        
    };
    template <size_t MaxSize>
    struct FileReadScanner: public Scanner {
    private:
        char buf[MaxSize], *p1, *p2;
        bool eofFlag = false;

        char gc_fread() {
            if (p1 == p2) {
                if (eofFlag)  throw EOFError{};
                p1 = buf;
                p2 = buf + std::fread(buf, sizeof(char), sizeof(buf), stdin);
                if (std::feof(stdin)) eofFlag = true;
            }
            return p1 == p2? '\0': *p1++;
        }
    protected:
        char gc() { return gc_fread(); }
    public:
        FileReadScanner(): p1(buf), p2(buf) {}
    };
    struct GetCharScanner: public Scanner {
    private:
        char gc_getchar() {
            char ch = getchar();
            if (ch != EOF)  return ch;
            else {
                throw EOFError{};
                return '\0';
            }
        }
    protected:
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
            x += 5e-7;
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
        template <typename T, typename std::enable_if<
                is_number<T>::value || std::is_convertible<T, const char *>::value || std::is_convertible<T, std::string const &>::value
            >::type* = nullptr>
        Printer &operator<< (const T &x) {
            if constexpr (
                is_number<T>::value || std::is_convertible<T, const char *>::value || std::is_convertible<T, std::string const &>::value
            ) {
                return write(x);
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

#ifdef IO_ENABLE_MMAP
    template <size_t MaxSize>
    struct DefaultIO: public MemoryMapScanner, FileWritePrinter<MaxSize> {};
    DefaultIO<1<<20> io;
#else  // not def IO_ENABLE_MMAP
    template <size_t MaxSize>
    struct DefaultIO: public FileReadScanner<MaxSize>, FileWritePrinter<MaxSize> {};
    DefaultIO<1<<20> io;
#endif  // def IO_ENABLE_MMAP
}
using IO::io;

using i32 = int32_t;  using i64 = int64_t;  using u32 = uint32_t;  using u64 = uint64_t;
using i128 = __int128;  using u128 = unsigned __int128;
using f32 = float;  using f64 = double;

// CYaRon Programming Language
namespace CYaRonLang {
    template <size_t MaxSize>
    struct FileWritePrinterStdandardError: public IO::Printer {
        char pbuf[MaxSize], *pp;
        FileWritePrinterStdandardError(): pp(pbuf) {}
        ~FileWritePrinterStdandardError() {
            std::fwrite(pbuf, 1, pp - pbuf, stderr);
        }
        void flush() {
            std::fwrite(pbuf, 1, MaxSize, stderr);
            pp = pbuf;
        }
        void put(char ch) override {
            if (pp - pbuf == MaxSize)  flush();
            *pp++ = ch;
        }
    };
    FileWritePrinterStdandardError<1<<20> ioError;
    struct StringScanner: public IO::Scanner {
        std::string &s;
        size_t index;
        bool eofFlag = false;
        StringScanner(std::string &s): s(s), index(0) {}
        char gc() override {
            if (index < s.size())  eofFlag = false;
            if (index == s.size()) {
                if (eofFlag)  throw IO::EOFError{};
                else  eofFlag = true;
                return '\0';
            }
            return index == s.size()? '\0': s[index++];
        }
    };
    namespace Compiler {
        void compileError(std::string const &s) {
            ioError << s << endl;
            std::exit(-1);
        }
        bool isBlank(char ch) {
            return ch <= 32 or ch == 127;
        }
        bool isIdentifierStart(char ch) {
            return ('A' <= ch and ch <= 'Z') or ('a' <= ch and ch <= 'z') or ch == '_';
        }
        bool isDigit(char ch) {
            return '0' <= ch and ch <= '9';
        }
        struct ParseAble {
            virtual void parse(IO::Scanner &io) = 0;
            friend IO::Scanner &operator>>(IO::Scanner &io, ParseAble &pa) {
                return pa.parse(io), io;
            }
        };
        struct DumpAble {
            virtual void dump(IO::Printer &io) = 0;
            friend IO::Printer &operator<<(IO::Printer &io, DumpAble &da) {
                return da.dump(io), io;
            }
        };
        
        struct Identifier: public ParseAble, public DumpAble {
            std::string name;
            Identifier(): name() {}
            Identifier(std::string name): name(name) {}

            void parse(IO::Scanner &io) {
                name.clear();
                // 读取一个标识符
                // 包括字母、数字、下划线
                // 不能以数字开头
                char ch = io.get();
                for (; not isIdentifierStart(ch); ch = io.get());
                for (; isIdentifierStart(ch) or isDigit(ch); ch = io.get()) {
                    name.push_back(ch);
                }
                io.unget();
            }
            void dump(IO::Printer &io) {
                io << name;
            }
        };
        // 整数字面量，允许带有后缀运算符
        struct Integer: public ParseAble, public DumpAble {
            u64 value;
            std::vector<std::string> suffixOperators;  // 后缀运算符
            Integer(): value(0) {}
            Integer(u64 value): value(value) {}
            
            void parse(IO::Scanner &io) {
                value = 0;
                suffixOperators.clear();
                // 读取一个整数字面量
                // 支持十进制、十六进制、八进制、二进制
                // 支持后缀 u, U, l, L, ll, LL
                char ch = io.get();
                try {
                    if (ch == '0') {
                        ch = io.get();
                        if (ch == 'x' or ch == 'X') {
                            // 十六进制
                            for (ch = io.get(); isDigit(ch) or ('A' <= ch and ch <= 'F') or ('a' <= ch and ch <= 'f') or ch == '\''; ch = io.get()) {
                                if (ch == '\'')  continue;
                                if (IO::leftShiftOverflow(value, 4))  throw IO::IntegerOverflowError{};
                                auto cur = ch <= '9'? ch ^ 48: (ch & 15) + 9;
                                if (IO::addOverflow(value, cur))  throw IO::IntegerOverflowError{};
                            }
                            io.unget();
                        } else if (ch == 'b' or ch == 'B') {
                            // 二进制
                            for (ch = io.get(); ch == '0' or ch == '1' or ch == '\''; ch = io.get()) {
                                if (ch == '\'')  continue;
                                if (IO::leftShiftOverflow(value, 1))  throw IO::IntegerOverflowError{};
                                if (IO::addOverflow(value, ch ^ 48))  throw IO::IntegerOverflowError{};
                            }
                            io.unget();
                        } else if (ch == 'o' or ch == 'O') {
                            // 八进制
                            for (ch = io.get(); ('0' <= ch and ch <= '7') or ch == '\''; ch = io.get()) {
                                if (ch == '\'')  continue;
                                if (IO::leftShiftOverflow(value, 3))  throw IO::IntegerOverflowError{};
                                if (IO::addOverflow(value, ch ^ 48))  throw IO::IntegerOverflowError{};
                            }
                            io.unget();
                        } else {
                            goto dec;
                        }
                    } else {
                    dec:
                        // 十进制
                        for (; isDigit(ch) or ch == '\''; ch = io.get()) {
                            if (ch == '\'')  continue;
                            if (IO::mulOverflow(value, 10))  throw IO::IntegerOverflowError{};
                            if (IO::addOverflow(value, ch ^ 48))  throw IO::IntegerOverflowError{};
                        }
                        io.unget();
                    }
                } catch (IO::IntegerOverflowError &) {
                    compileError("Number is too large");
                }
                // 后缀，用分隔符（"'"） 隔开的若干个标识符，表示文本运算符
                Identifier id;
                // 特别地，如果第一个字符是 'e' 或 'E'，则识别为科学计数法
                // 如果存在名为 e 的文本运算符，可以写作 123'e
                if (ch = io.get(); isIdentifierStart(ch) and ch != 'E' and ch != 'e') {
                    io.unget();
                    io >> id;
                    suffixOperators.push_back(id.name);
                } else {
                    io.unget();
                }
                while (io.get() == '\'') {
                    io >> id;
                    suffixOperators.push_back(id.name);
                }
                io.unget();
            }
            void dump(IO::Printer &io) {
                io << value;
                for (auto &op: suffixOperators) {
                    io << "'" << op;
                }
            }
        };
        // 字符串字面量，允许带有后缀运算符
        struct String: public ParseAble, public DumpAble {
            std::string value;
            std::vector<std::string> suffixOperators;
            String(): value() {}
            String(std::string value): value(std::move(value)) {}
            String(std::string &&value): value(std::move(value)) {}
            void parse(IO::Scanner &io) {
                value.clear();
                suffixOperators.clear();
                // 读取一个字符串字面量
                // 用双引号括起来的若干个字符
                // 支持转义字符
                // 对于未知的转义，直接去掉反斜杠
                char ch = io.get();
                if (ch != '"')  throw "String literal should starts with '\"'";
                for (ch = io.get(); ch != '"'; ch = io.get()) {
                    if (ch == '\\') {
                        ch = io.get();
                        switch (ch) {
                        case 'a': value.push_back('\a'); break;  // 响铃
                        case 'b': value.push_back('\b'); break;  // 退格
                        case 'f': value.push_back('\f'); break;  // 换页
                        case 'n': value.push_back('\n'); break;  // 换行
                        case 'r': value.push_back('\r'); break;  // 回车
                        case 't': value.push_back('\t'); break;  // 制表
                        case 'v': value.push_back('\v'); break;  // 垂直制表
                        case '\\': value.push_back('\\'); break;  // 反斜杠
                        case '\'': value.push_back('\''); break;  // 单引号
                        case '"': value.push_back('"'); break;  // 双引号
                        case '?': value.push_back('\?'); break;  // 问号
                        case 'x': {
                            // 十六进制
                            int x = 0;
                            for (int i = 0; i < 2; i++) {
                                ch = io.get();
                                if ('0' <= ch and ch <= '9')  x = x * 16 + (ch ^ 48);
                                else if ('A' <= ch and ch <= 'F')  x = x * 16 + (ch & 15) + 9;
                                else if ('a' <= ch and ch <= 'f')  x = x * 16 + (ch & 15) + 9;
                                else  compileError("Invalid hex digit");
                            }
                            value.push_back(x);
                        } break;
                        case 0:  [[fallthrough]];
                        case 1:  [[fallthrough]];
                        case 2:  [[fallthrough]];
                        case 3:  [[fallthrough]];
                        case 4:  [[fallthrough]];
                        case 5:  [[fallthrough]];
                        case 6:  [[fallthrough]];
                        case 7: {
                            // 八进制
                            int x = ch ^ 48;
                            for (int i = 0; i < 2; i++) {
                                ch = io.get();
                                if ('0' <= ch and ch <= '7')  x = x * 8 + (ch ^ 48);
                                else  compileError("Invalid oct digit");
                            }
                            value.push_back(x);
                        } break;
                        default: value.push_back(ch); break;  // 未知
                        }
                    } else {
                        value.push_back(ch);
                    }
                }
                // 后缀，用分隔符（"'"） 隔开的若干个标识符，表示文本运算符
                Identifier id;
                if (ch = io.get(); isIdentifierStart(ch)) {
                    io.unget();
                    io >> id;
                    suffixOperators.push_back(id.name);
                } else {
                    io.unget();
                }
                while (io.get() == '\'') {
                    io >> id;
                    suffixOperators.push_back(id.name);
                }
                io.unget();
            }
            void dump(IO::Printer &io) {
                io << '"';
                for (auto x: value) {
                    switch (x) {
                    case '\n':  io << "\\n"; break;
                    case '\t':  io << "\\t"; break;
                    case '\r':  io << "\\r"; break;
                    case '\0':  io << "\\0"; break;
                    case '\\':  io << "\\\\"; break;
                    case '\"':  io << "\\\""; break;
                    case ' ':  io << ' '; break;
                    [[likely]] default: 
                        if (isBlank(x))  io << std::format("\\x{:02X}", x);
                        else  io << x;
                    }
                }
                io << '"';
                if (not suffixOperators.empty()) {
                    io << suffixOperators.front();
                }
                if (suffixOperators.size() > 1) {
                    for (auto op: suffixOperators | views::drop(1)) {
                        io << '\'' << op;
                    }
                }
            }
        };
        struct Symbol: public ParseAble, public DumpAble {
            char value;
            Symbol(): value(0) {}
            Symbol(char value): value(value) {}
            void parse(IO::Scanner &io) {
                io >> value;
            }
            void dump(IO::Printer &io) {
                io << value;
            }
        };
        
        struct FloatingPointNumber: public ParseAble, public DumpAble {
            double value;
            std::vector<std::string> suffixOperators;
            FloatingPointNumber(): value(0) {}
            FloatingPointNumber(double value): value(value) {}
            void parse(IO::Scanner &io) {
                double tmp = 1;
                char ch = io.get();
                value = 0;
                for (; isDigit(ch); ch = io.get())  value = value * 10 + (ch ^ 48);
                if (ch == '.') {
                    for (ch = io.get(); not isDigit(ch); ch = io.get())  tmp *= 0.1, value += tmp * (ch ^ 48);
                }
                Identifier id;
                if  (ch = io.get(); isIdentifierStart(ch)) {
                    io.unget();
                    io >> id;
                    suffixOperators.push_back(id.name);
                } else {
                    io.unget();
                }
                while (io.get() == '\'') {
                    io >> id;
                    suffixOperators.push_back(id.name);
                }
                io.unget();
            }
            void dump(IO::Printer &io) {
                io << value;
            }
        };
        struct Token {
            enum Tag {
                NoneTag, IdentifierTag, SymbolTag, KeywordTag, IntegerTag, StringTag, EndOfLineTag, FloatingPointTag
            } tag = NoneTag;
            std::variant<int, Identifier, Integer, String, Symbol, FloatingPointNumber> value = 0;
        };
        
        std::vector<Token> tokenize(IO::Scanner &io) {
            std::vector<Token> tokens;
            try {
                while (true) {
                    char ch = io.get();
                    io.unget();
                    if (ch == '\0') {
                        break;
                    } if (ch == '\n') {
                        io.get();
                        tokens.push_back({Token::EndOfLineTag});
                    } else if (isBlank(ch)) {
                        io.get();
                    } else if (ch == '"') {
                        String str;
                        io >> str;
                        tokens.push_back({Token::StringTag, str});
                    } else if (isDigit(ch)) {
                        Integer integer;
                        FloatingPointNumber fp;
                        bool isInteger = true;
                        io >> integer;
                        // 已经有了后缀运算符，不应视为浮点数
                        if (integer.suffixOperators.empty()) {
                            if (io.get() == '.') {
                                io.unget();  // 读到小数点，即 0.123 的形式
                                isInteger = false;
                                io >> fp;
                                fp.value += integer.value;
                            } else {
                                io.unget();
                            }
                            if (ch = io.get(); ch == 'e' or ch == 'E') {
                                // 科学计数法
                                if (isInteger)  fp.value = integer.value;
                                isInteger = false;
                                ch = io.get();
                                bool expSigned = false;
                                if (ch == '+')  ch = io.get();
                                else if (ch == '-')  ch = io.get(), expSigned = true;
                                if (isDigit(ch)) {
                                    io.unget(), io >> integer;
                                    if (expSigned)  fp.value *= std::pow(10.0, -(double)integer.value);
                                    else  fp.value *= std::pow(10, integer.value);
                                    fp.suffixOperators = std::move(integer.suffixOperators);
                                } else {
                                    compileError("Invalid decimal literal");
                                }
                            } else {
                                io.unget();
                            }
                        }
                        if (isInteger)  tokens.push_back({Token::IntegerTag, integer});
                        else  tokens.push_back({Token::FloatingPointTag, fp});
                    } else if (isIdentifierStart(ch)) {
                        Identifier identifier;
                        io >> identifier;
                        tokens.push_back({Token::IdentifierTag, identifier});
                    } else {
                        Symbol symbol;
                        io >> symbol;
                        tokens.push_back({Token::SymbolTag, symbol});
                    }
                }
            } catch (IO::EOFError &) {}
            return tokens;
        }
    }
    void test() {
        using namespace Compiler;
//         std::string s = R"aaa(
// "test"
// 5e-6;
// print("test");
// return 0;
// )aaa";
//         StringScanner scan(s);
        auto res = tokenize(io);
        for (auto token: res) {
            switch (token.tag) {
            case Token::IdentifierTag:
                io << "Identifier: " << std::get<Identifier>(token.value) << endl;
                break;
            case Token::IntegerTag:
                io << "Integer: " << std::get<Integer>(token.value) << endl;
                break;
            case Token::StringTag:
                io << "String: " << std::get<String>(token.value) << endl;
                break;
            case Token::SymbolTag:
                io << "Symbol: " << std::get<Symbol>(token.value) << endl;
                break;
            case Token::EndOfLineTag:
                io << "End of line" << endl;
                break;
            case Token::FloatingPointTag:
                io << "Floating point: " << std::get<FloatingPointNumber>(token.value) << endl;
                break;
            default:
                io << "Unknown token" << endl;
                break;
            }
        }
    }
    void solve() {
        test();
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    CYaRonLang::solve();
    return 0;
}
