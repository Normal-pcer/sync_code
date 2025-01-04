/**
 * @link https://www.luogu.com.cn/problem/P3695
 */
#include "./libs/debug_macros.hpp"

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
        bool flag = std::__lg(x) + y >= std::numeric_limits<T>::digits;
        return x <<= y, flag;
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
    struct DefaultIO: public GetCharScanner, PutCharPrinter {};
    DefaultIO io;
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
    namespace Trie {
        class Trie {
        public:
            struct Node {
                std::array<Node *, 96> next;
                int match;
            };
            std::deque<Node> nodes;
            Node *root;
            Trie(): nodes(), root(nullptr) {
                nodes.push_back({});
                root = &nodes.back();
            }
            Trie(std::initializer_list<std::string> &&init): nodes(), root(nullptr) {
                nodes.push_back({});
                root = &nodes.back();
                for (const auto &s: init)  insert(s);
            }
            
            int match(const std::string &s) const {
                Node *cur = root;
                for (auto ch: s) {
                    assert(ch > 32), ch -= 32;
                    if (cur->next[ch])  cur = cur->next[ch];
                    else  return 0;
                }
                return cur->match;
            }
            void insert(const std::string &s) {
                Node *cur = root;
                for (auto ch: s) {
                    assert(ch > 32), ch -= 32;
                    cur->match++;
                    if (cur->next[ch])  cur = cur->next[ch];
                    else  nodes.push_back({}), cur = cur->next[ch] = &nodes.back();
                }
                cur->match++;
            }
        };
    }
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

            std::strong_ordering operator<=> (Identifier const &other) const {
                return name <=> other.name;
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
                            goto egg;  // 解析十进制
                        }
                    } else {
                    egg:
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
                        case '0':  [[fallthrough]];
                        case '1':  [[fallthrough]];
                        case '2':  [[fallthrough]];
                        case '3':  [[fallthrough]];
                        case '4':  [[fallthrough]];
                        case '5':  [[fallthrough]];
                        case '6':  [[fallthrough]];
                        case '7': {
                            // 八进制
                            int x = ch ^ 48;
                            for (int i = 0; i < 2; i++) {
                                ch = io.get();
                                if ('0' <= ch and ch <= '7')  x = x * 8 + (ch ^ 48);
                                else {
                                    io.unget();
                                    break;
                                }
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
                    case '\x20':  io << '\x20'; break;
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
            std::string value;
            Symbol(): value({}) {}
            Symbol(std::string value): value(value) {}
            void parse(IO::Scanner &);
            void dump(IO::Printer &io) {
                io << value;
            }
        };
        Trie::Trie symbols {
            "<=", ">=", "!=", "==", "<<", ">>", "<=>", "&&", "||", "+=", "-=", "*=", "/=", "%=", "|=", "&=", "^=", "->",
            "++", "--"
        };
        void Symbol::parse(IO::Scanner &io) {
            value.clear();
            char ch = io.get();
            value.push_back(ch);
            auto *cur = symbols.root->next[ch - 32];
            bool flag = false;
            while (cur) {
                ch = io.get(), flag = true;
                if (isBlank(ch) or isDigit(ch) or isIdentifierStart(ch))  break;
                if (cur->next[ch - 32])  cur = cur->next[ch - 32];
                else  break;
                value.push_back(ch);
            }
            if (flag)  io.unget();
        }
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
                    for (ch = io.get(); isDigit(ch); ch = io.get())  tmp *= 0.1, value += tmp * (ch ^ 48);
                }
                Identifier id;
                if (isIdentifierStart(ch) and ch != 'E' and ch != 'e') {
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
                for (auto i: suffixOperators)  io << '\'' << i;
            }
        };
        struct Token {
            enum Tag {
                NoneTag, IdentifierTag, SymbolTag, KeywordTag, IntegerTag, StringTag, EndOfLineTag, FloatingPointTag
            } tag = NoneTag;
            std::variant<int, Identifier, Integer, String, Symbol, FloatingPointNumber> value = 0;
            friend IO::Printer &operator<<(IO::Printer &io, Token &token) {
                switch (token.tag) {
                case Token::IdentifierTag:
                    return io << "Identifier_" << std::get<Identifier>(token.value);
                case Token::IntegerTag:
                    return io << "Integer" << std::get<Integer>(token.value);
                case Token::StringTag:
                    return io << "String" << std::get<String>(token.value);
                case Token::SymbolTag:
                    return io << "Symbol" << std::get<Symbol>(token.value);
                case Token::EndOfLineTag:
                    return io << "EOL";
                case Token::FloatingPointTag:
                    return io << "FloatingPoint" << std::get<FloatingPointNumber>(token.value);
                default:
                    return io << "Unknown";
                }
            }
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
        // AST
        // 抽象语法树
        namespace AST {
            using TokensType = const std::vector<Token>;
            using TokenIterator = TokensType::const_iterator;
            using TokensSubrange = std::ranges::subrange<TokenIterator>;
            struct Node {
                Node() = default;
                Node(const Node &) = delete;
                virtual ~Node() = default;
                Node &operator= (const Node &) = delete;
            };
            template <typename T>
            struct ParseResult {
                T *node;
                TokenIterator it;
            };
            struct StatementNode: public Node {
                enum Type {
                    NoneStatement,
                    ExpressionEvaluateStatement,
                    VariableDeclareStatement,
                    RunBlockStatement,
                } type = NoneStatement;
                StatementNode(Type type = NoneStatement): type(type) {}
                virtual ~StatementNode() = default;
            };
            struct BlockNode: public Node {
                enum Type {
                    NoneBlock,  // 常规代码块
                    IfBlock,  // 条件代码块
                    WhileBlock,  // 条件循环代码块
                    VarsBlock,  // 变量声明代码块
                } type = NoneBlock;
                std::vector<StatementNode *> statements;
                BlockNode() = default;
                BlockNode(Type type): type(type), statements() {}
                virtual ~BlockNode() {
                    for (auto &statement: statements)  delete statement;
                }
                template <typename T>
                static ParseResult<BlockNode> parse(const T &src);
            };
            struct ExpressionNode: public Node {
                enum Operator {
                    // 特殊运算符
                    NoneOp,
                    Bracket,    // 括号（仅解析时使用）
                    Call,       // 函数调用
                    FunctionArgsBracket,  // 函数参数括号（仅解析时使用）
                    Subscript,  // 下标访问
                    SubscriptBracket,  // 方括号（仅解析时使用）
                    SplitComma,  // 作为分隔符的逗号
                    // 算术运算符
                    UnaryAdd,  // 正
                    UnarySub,  // 负
                    Add,  // 加
                    Sub,  // 减
                    Mul,  // 乘
                    Div,  // 除
                    Mod,  // 模
                    // 比较运算符
                    Less,  // 小于
                    LessEqual,  // 小于等于
                    Greater,  // 大于
                    GreaterEqual,  // 大于等于
                    Equal,  // 等于
                    NotEqual,  // 不等于
                    // 逻辑运算符
                    And,  // 与
                    Or,  // 或
                    Not,  // 非
                    // 位运算符
                    BitAnd,  // 与
                    BitOr,  // 或
                    BitXor,  // 异或
                    BitNot,  // 非
                    BitShiftLeft,  // 左移
                    BitShiftRight,  // 右移
                    // 赋值运算符
                    Assign,  // 赋值
                } op = NoneOp;
                static constexpr const char *opNames[] = {"NoneOp", "Bracket", "Call", "FunctionArgsBracket", "Subscript", "SubscriptBracket", "SplitComma", "UnaryAdd", "UnarySub", "Add", "Sub", "Mul", "Div", "Mod", "Less", "LessEqual", "Greater", "GreaterEqual", "Equal", "NotEqual", "And", "Or", "Not", "BitAnd", "BitOr", "BitXor", "BitNot", "BitShiftLeft", "BitShiftRight", "Assign"};
                // 操作数；特别地，单目运算符只有 left
                ExpressionNode *left = nullptr, *right = nullptr;
                ExpressionNode(Operator op = NoneOp): op(op) {}
                virtual ~ExpressionNode();

                struct OperatorInfo {
                    static constexpr const int priority_max = 0x3f3f3f3f;
                    int priority = 0;  // 优先级；越小越先算
                    bool leftAssociative = false;  // 从右到左结合
                };
                static constexpr OperatorInfo infoOf(Operator op) {
                    switch (op) {
                    case Call:  return {2, false};
                    case Subscript:  return {2, false};
                    case SplitComma:  return {17, false};
                    case UnaryAdd:  return {3, true};
                    case UnarySub:  return {3, true};
                    case Add:  return {6, false};
                    case Sub:  return {6, false};
                    case Mul:  return {5, false};
                    case Div:  return {5, false};
                    case Mod:  return {5, false};
                    case Less:  return {9, false};
                    case LessEqual:  return {9, false};
                    case Greater:  return {9, false};
                    case GreaterEqual:  return {9, false};
                    case Equal:  return {10, false};
                    case NotEqual:  return {10, false};
                    case And:  return {14, false};
                    case Or:  return {15, false};
                    case Not:  return {3, true};
                    case BitAnd:  return {11, false};
                    case BitOr:  return {13, false};
                    case BitXor:  return {12, false};
                    case BitNot:  return {3, true};
                    case BitShiftLeft:  return {7, false};
                    case BitShiftRight:  return {7, false};
                    case Assign:  return {16, true};
                    default:  return {OperatorInfo::priority_max, false};
                    }
                }
                template <typename T>
                static ParseResult<ExpressionNode> parse(const T &);
                void walk(IO::Printer &io);
            };
            struct ValueNode: public ExpressionNode {
                enum Type {
                    NoneValue,
                    Integer,
                    FloatingPoint,
                    String,
                    Identifier
                } type = NoneValue;
                Token token;

                ValueNode(Type type, Token token): 
                    ExpressionNode({ExpressionNode::NoneOp}), type(type), token(token) {}
                ValueNode(const ValueNode &other): 
                    ExpressionNode({ExpressionNode::NoneOp}), type(other.type), token(other.token) {}
                ValueNode(Token token): ExpressionNode({ExpressionNode::NoneOp}), type(NoneValue), token(token) {
                    type = [&]() {
                        switch (token.tag) {
                        case Token::IntegerTag:  return Integer;
                        case Token::FloatingPointTag:  return FloatingPoint;
                        case Token::StringTag:  return String;
                        case Token::IdentifierTag:  return Identifier;
                        default:  return assert(false), NoneValue;
                        }
                    }();
                }
            };
            struct ExpressionEvaluateStatementNode: public StatementNode {
                ExpressionNode *expr;
                ExpressionEvaluateStatementNode(ExpressionNode *expr):
                    StatementNode(StatementNode::ExpressionEvaluateStatement), expr(expr){}
                ~ExpressionEvaluateStatementNode() {
                    delete expr;
                }
            };
            struct VariableDeclareStatementNode: public StatementNode {
                Identifier name;
                ExpressionNode *type;
                VariableDeclareStatementNode(): StatementNode(StatementNode::VariableDeclareStatement) {}
                ~VariableDeclareStatementNode() {
                    delete type;
                }
                template <typename T>
                static ParseResult<VariableDeclareStatementNode> parse(const T &src);
            };
            struct RunBlockStatementNode: public StatementNode {
                // 执行一个代码块
                BlockNode *block;
                RunBlockStatementNode(BlockNode *block): StatementNode(StatementNode::RunBlockStatement), block(block) {}
                ~RunBlockStatementNode() {
                    delete block;
                }
            };
            struct ConditionalBlockNode: public BlockNode {
                // 带条件语句块（if, while）
                enum Condition {
                    None, Less, LessEqual, Greater, GreaterEqual, Equal, NotEqual
                } condition = None;
                ExpressionNode *left, *right;  // 满足 *left (condition) *right 时才执行 statements
                ConditionalBlockNode(BlockNode::Type type): BlockNode(type) {}
                ~ConditionalBlockNode() {
                    delete left;
                    delete right;
                }
                template <typename T>
                auto parseCondition(const T &src) {
                    // 类似 op, a, b
                    auto it = src.begin();
                    assert(it->tag == Token::IdentifierTag);
                    auto opName = std::get<Identifier>(it->value).name;
                    condition = [&]() {
                        if (opName == "lt")  return Less;
                        if (opName == "le")  return LessEqual;
                        if (opName == "gt")  return Greater;
                        if (opName == "ge")  return GreaterEqual;
                        if (opName == "eq")  return Equal;
                        if (opName == "ne")  return NotEqual;
                        return assert(false), None;
                    }();
                    it++;
                    // 期待一个逗号
                    assert(it->tag == Token::SymbolTag and std::get<Symbol>(it->value).value == ","), it++;
                    // 读取两个表达式
                    auto [left_expr, right_begin] = ExpressionNode::parse(TokensSubrange{it, src.end()});
                    auto [right_expr, end] = ExpressionNode::parse(TokensSubrange{right_begin, src.end()});
                    it = end;
                    left = left_expr;
                    right = right_expr;
                    return it;
                }
            };
            struct IfBlockNode: public ConditionalBlockNode {
                IfBlockNode(): ConditionalBlockNode(BlockNode::IfBlock) {}
            };
            struct WhileBlockNode: public ConditionalBlockNode {
                WhileBlockNode(): ConditionalBlockNode(BlockNode::WhileBlock) {}
            };
            template <typename T>
            ParseResult<VariableDeclareStatementNode> VariableDeclareStatementNode::parse(const T &src) {
                // 解析一行变量定义语句
                auto res = new VariableDeclareStatementNode;
                auto it = src.begin();
                // 类似 name: type[EOL] 的形式
                // 读取标识符
                assert(it->tag == Token::IdentifierTag), res->name = std::get<Identifier>(it->value), it++;
                assert(it->tag == Token::SymbolTag and std::get<Symbol>(it->value).value == ":"), it++;
                // 读取类型
                // 类型为表达式
                auto [type, end] = ExpressionNode::parse(TokensSubrange{it, src.end()});
                res->type = type;
                it = end;
                return {res, it};
            }
            template <typename T>
            ParseResult<BlockNode> BlockNode::parse(T const &src) {
                // 解析一个语句块
                // 语句块用花括号包裹，包含若干条语句
                auto res = new BlockNode;
                auto it = src.begin();
                for (; it != src.end();) {
                    if (it->tag == Token::EndOfLineTag) {
                        it++;
                        continue;
                    }
                    if (it->tag == Token::SymbolTag and std::get<Symbol>(it->value).value == "}")  return {res, ++it};
                    // 读取一个子块
                    if (it->tag == Token::SymbolTag and std::get<Symbol>(it->value).value == "{") {
                        auto [sub_block, next] = BlockNode::parse(TokensSubrange{++it, src.end()});
                        res->statements.push_back(new RunBlockStatementNode{sub_block}), it = next;
                        continue;
                    }
                    // vars 关键字
                    // 把当前块变成声明块
                    if (it->tag == Token::IdentifierTag and std::get<Identifier>(it->value).name == "vars") {
                        res->type = VarsBlock;
                        it++;
                        continue;
                    }
                    // ihu 关键字（if）
                    if (it->tag == Token::IdentifierTag and std::get<Identifier>(it->value).name == "ihu") {
                        delete res, res = new IfBlockNode;
                        it = dynamic_cast<IfBlockNode *>(res)->parseCondition(TokensSubrange{it+1, src.end()});
                        continue;
                    }
                    // while 关键字
                    if (it->tag == Token::IdentifierTag and std::get<Identifier>(it->value).name == "while") {
                        delete res, res = new WhileBlockNode;
                        it = dynamic_cast<WhileBlockNode *>(res)->parseCondition(TokensSubrange{it+1, src.end()});
                        continue;
                    }
                    // todo: for 循环

                    // 特殊语法：
                    // 冒号开头的 :f x, y 等价于函数调用 f(x, y)
                    if (it->tag == Token::SymbolTag and std::get<Symbol>(it->value).value == ":") {
                        it++;
                        // 匹配一个函数名
                        assert(it->tag == Token::IdentifierTag);
                        auto funcToken = *it++;
                        std::vector<Token> tmp;
                        // 匹配直到一个 EOL
                        while (it != src.end() and it->tag != Token::EndOfLineTag)  tmp.push_back(*it++);
                        // 补全成正常函数调用
                        tmp.insert(tmp.begin(), {Token::SymbolTag, Symbol("(")});
                        tmp.insert(tmp.begin(), funcToken);
                        tmp.push_back({Token::SymbolTag, Symbol(")")});
                        assert(it != src.end()), it++, tmp.push_back({Token::EndOfLineTag});
                        // 解析 tmp
                        auto [expr, next] = ExpressionNode::parse(tmp);
                        assert(expr->op == ExpressionNode::Call and next == tmp.end());
                        res->statements.push_back(new ExpressionEvaluateStatementNode{expr});
                        continue;
                    }
                    if (res->type == VarsBlock) {
                        // 解析一个变量声明
                        auto [decl, next] = VariableDeclareStatementNode::parse(TokensSubrange{it, src.end()});
                        res->statements.push_back(decl);
                        it = next;
                        continue;
                    }
                    // 表达式求值
                    auto [expr, next] = ExpressionNode::parse(TokensSubrange{it, src.end()});
                    res->statements.push_back(new ExpressionEvaluateStatementNode{expr});
                    it = next;
                }
                return {res, src.end()};
            }
            ExpressionNode::~ExpressionNode() {
                if (left)  delete left;
                if (right)  delete right;
            }
            template <typename T>
            ParseResult<ExpressionNode> ExpressionNode::parse(const T &src) {
                auto [postfix, it] = [&]() {                
                    const auto inf = OperatorInfo::priority_max;
                    struct StackValueType {
                        Operator op;
                        int args_remains;  // 剩余操作数
                    };
                    std::vector<StackValueType> ops {{NoneOp, 1}};  // 运算符栈
                    auto it = src.begin();
                    // 转成后缀表达式
                    struct PostfixValueType {
                        bool symbol = false;
                        std::variant<Operator, ValueNode> item;
                    };
                    std::vector<PostfixValueType> postfix;
                    auto add = [&](Operator type, int args_remains) {
                        if (infoOf(type).leftAssociative) {
                            while (not ops.empty() and infoOf(ops.back().op).priority < infoOf(type).priority)  postfix.push_back({true, {ops.back().op}}), ops.pop_back();
                        } else {
                            while (not ops.empty() and infoOf(ops.back().op).priority <= infoOf(type).priority)  postfix.push_back({true, {ops.back().op}}), ops.pop_back();
                        }
                        ops.push_back({type, args_remains});
                    };
                    for (; it != src.end(); it++) {
                        auto &token = *it;
                        if (token.tag == Token::SymbolTag or token.tag == Token::EndOfLineTag) {
                            auto op = token.tag == Token::EndOfLineTag? "\n": std::get<Symbol>(token.value).value;
                            if (op == "(") {
                                // 函数调用
                                // 如果左侧是一个完整结果，视为函数调用
                                if (not ops.empty() and ops.back().args_remains == 0) {
                                    add(Call, 0);
                                    ops.push_back({FunctionArgsBracket, 1});
                                } else {
                                    ops.back().args_remains--;
                                    ops.push_back({Bracket, 1});
                                }
                            } else if (op == ")") {
                                while (infoOf(ops.back().op).priority != inf) {
                                    postfix.push_back({true, ops.back().op}), ops.pop_back();
                                }
                                // 结束函数调用括号
                                if (ops.back().op == FunctionArgsBracket) {
                                    bool flag = ops.back().args_remains == 1;
                                    ops.pop_back();
                                    // 如果没有参数，填充一个空值
                                    if (flag)  postfix.push_back({false, ValueNode{ValueNode::NoneValue, Token{Token::NoneTag}}});
                                } else {
                                    // 结束常规括号
                                    assert(ops.back().op == Bracket and ops.back().args_remains == 0);
                                    ops.pop_back();
                                }
                            } else if (op == ",") {
                                while (not ops.empty() and infoOf(ops.back().op).priority < infoOf(SplitComma).priority) {
                                    postfix.push_back({true, {ops.back().op}}), ops.pop_back();
                                }
                                if (ops.empty() or ops.back().op == NoneOp) {
                                    // 结束表达式
                                    it++;
                                    break;
                                }
                                ops.push_back({SplitComma, 1});
                            } else if (op == "[") {
                                add(Subscript, 0);
                                ops.push_back({SubscriptBracket, 1});
                            } else if (op == "]") {
                                // 中括号匹配
                                while (infoOf(ops.back().op).priority != inf) {
                                    postfix.push_back({true, ops.back().op}), ops.pop_back();
                                }
                                assert(ops.back().op == SubscriptBracket);
                                bool flag = ops.back().args_remains == 1;
                                // postfix.push_back({true, ops.back().op}), ops.pop_back();
                                ops.pop_back();
                                if (flag)  postfix.push_back({false, ValueNode{ValueNode::NoneValue, Token{Token::NoneTag}}});
                            } else if (op == "+") {
                                // 判断为一元或者二元
                                // 如果左侧为一个期待其他操作数的符号，视为一元运算符
                                if (not ops.empty() and ops.back().args_remains != 0) {
                                    ops.back().args_remains--;
                                    add(UnaryAdd, 1);
                                } else {
                                    add(Add, 1);
                                }
                            } else if (op == "-") {
                                if (not ops.empty() and ops.back().args_remains != 0) {
                                    ops.back().args_remains--;
                                    add(UnarySub, 1);
                                } else {
                                    add(Sub, 1);
                                }
                            } else if (op == ";" or op == "\n") {
                                // 结束表达式
                                it++;
                                break;
                            }
    #define JOIN_BINARY_OP(op_type, op_str) else if (op == op_str)  add(op_type, 1);
                            JOIN_BINARY_OP(Mul, "*")
                            JOIN_BINARY_OP(Div, "/")
                            JOIN_BINARY_OP(Mod, "%")
                            JOIN_BINARY_OP(Less, "<")
                            JOIN_BINARY_OP(Greater, ">")
                            JOIN_BINARY_OP(LessEqual, "<=")
                            JOIN_BINARY_OP(GreaterEqual, ">=")
                            JOIN_BINARY_OP(Equal, "==")
                            JOIN_BINARY_OP(NotEqual, "!=")
                            JOIN_BINARY_OP(And, "&&")
                            JOIN_BINARY_OP(Or, "||")
                            JOIN_BINARY_OP(BitAnd, "&")
                            JOIN_BINARY_OP(BitOr, "|")
                            JOIN_BINARY_OP(BitXor, "^")
                            JOIN_BINARY_OP(BitShiftLeft, "<<")
                            JOIN_BINARY_OP(BitShiftRight, ">>")
                            JOIN_BINARY_OP(Assign, "=")
    #undef JOIN_BINARY_OP
                            else if (op == "!") {
                                ops.back().args_remains--;
                                add(Not, 1);
                            } else if (op == "~") {
                                ops.back().args_remains--;
                                add(BitNot, 1);
                            } else {
                                ioError << "Unknown symbol: " << op << endl;
                                throw -1;
                            }
                        } else {
                            // 直接压入答案
                            if (token.tag == Token::IdentifierTag) {
                                postfix.push_back({false, ValueNode{ValueNode::Identifier, token}});
                            } else if (token.tag == Token::IntegerTag) {
                                postfix.push_back({false, ValueNode{ValueNode::Integer, token}});
                            } else if (token.tag == Token::FloatingPointTag) {
                                postfix.push_back({false, ValueNode{ValueNode::FloatingPoint, token}});
                            } else if (token.tag == Token::StringTag) {
                            postfix.push_back({false, ValueNode{ValueNode::String, token}});
                            }
                            ops.back().args_remains--;
                            assert(ops.back().args_remains == 0);
                        }
                    }
                    // 清空剩余操作符
                    while (not ops.empty() and ops.size() != (size_t)1) {
                        postfix.push_back({true, ops.back().op});
                        ops.pop_back();
                    }
                    return std::pair{postfix, it};
                }();
                // 测试，输出后缀表达式
                debug for (auto &x: postfix) {
                    if (x.symbol) {
                        io << "Operator: " << (int)std::get<Operator>(x.item) << '\x20' << opNames[(int)std::get<Operator>(x.item)] << endl;
                    } else {
                        io << "Value: ";
                        auto token = std::get<ValueNode>(x.item).token;
                        io << token << endl;
                    }
                }

                // 建立表达式树
                // 对于所有非运算符，节点压入栈中
                // 对于所有运算符，弹出对应数量的节点作为儿子，建立运算符节点，然后压入栈中
                std::vector<AST::ExpressionNode *> nodes_stack;
                auto countOf = [&](Operator op) {
                    switch (op) {
                    case UnaryAdd:  return 1;
                    case UnarySub:  return 1;
                    case Not:  return 1;
                    case BitNot:  return 1;
                    default:  return 2;
                    }
                };  // 运算数的数量
                for (auto &x: postfix) {
                    if (x.symbol) {
                        auto count = countOf(std::get<Operator>(x.item));
                        ExpressionNode *l_son = nullptr, *r_son = nullptr;
                        if (count == 2) {
                            r_son = nodes_stack.back();
                            nodes_stack.pop_back();
                            l_son = nodes_stack.back();
                            nodes_stack.pop_back();
                        } else {
                            l_son = nodes_stack.back();
                            nodes_stack.pop_back();
                        }
                        auto *node = new ExpressionNode{std::get<Operator>(x.item)};
                        node->left = l_son, node->right = r_son;
                        nodes_stack.push_back(node);
                    } else {
                        auto *node = new ValueNode{std::get<ValueNode>(x.item)};
                        nodes_stack.push_back(node);
                    }
                }
                // 当前元素即为栈中唯一的元素
                assert(nodes_stack.size() == (size_t)1);
                return {nodes_stack.back(), it};
            }
        }
    }
    namespace Interpreter {
        using Compiler::Identifier;
        namespace AST = Compiler::AST;
        struct Object {
            enum Type {
                Struct, Int, Function, String, None
            } type;
            std::variant<std::map<Identifier, Object>, std::shared_ptr<int>, std::shared_ptr<std::string>, Identifier, std::nullptr_t> value;
            Object(Type type = None): type(type), value(nullptr) {}
            template <typename T>
            Object(Type type, const T &value): type(type), value(value) {}
        };
        class Program {
        public:
            AST::BlockNode *root;
            std::map<Identifier, Object> variables;
            Program(AST::BlockNode *root);
            ~Program() {
                delete root;
            }
            void run();
            Object EvaluateExpression(AST::ExpressionNode *node);
            template <typename OutIterater>
            OutIterater getFunctionArguments(AST::ExpressionNode *, OutIterater); 
            // 运行语句块
            void runBlock(AST::BlockNode *block);
        };
        Program::Program(AST::BlockNode *root): root(root) {
            // 注册内置函数
            variables.insert({{"print"}, Object{Object::Function, Identifier{"print"}}});
            variables.insert({{"println"}, Object{Object::Function, Identifier{"println"}}});
            variables.insert({{"scan"}, Object{Object::Function, Identifier{"scan"}}});
            variables.insert({{"set"}, Object{Object::Function, Identifier{"set"}}});
            variables.insert({{"test_sort"}, Object{Object::Function, Identifier{"test_sort"}}});
        }
        // 展开逗号分隔符表达式，获取函数参数列表，写入到输出迭代器
        // 例如，后缀表达式 a, b, (comma), c, (comma)
        // 提取出参数列表 a, b, c
        // node 为一个逗号运算符，或唯一的参数
        template <typename OutIterater>
        OutIterater Program::getFunctionArguments(AST::ExpressionNode *node, OutIterater out) {
            if (node->op == AST::ExpressionNode::SplitComma) {
                // 当前为逗号，向左右子树获取参数
                out = getFunctionArguments(node->left, out);
                out = getFunctionArguments(node->right, out);
            } else {
                if (node->op == AST::ExpressionNode::NoneOp) {
                    // 到达叶子节点（值节点）
                    auto value_node = dynamic_cast<AST::ValueNode *>(node);
                    // 判断是否为空
                    // 如果是 NoneTag，说明为空参数
                    if (value_node->token.tag == Compiler::Token::NoneTag)  return out;
                }
                // 从下方的表达式获取
                *out++ = EvaluateExpression(node);
            }
            return out;
        }
        void Program::run() {
            runBlock(root);
        }
        Object Program::EvaluateExpression(AST::ExpressionNode *node) {
            if (node->op == node->NoneOp) {
                // 叶子节点
                auto value_node = dynamic_cast<AST::ValueNode *>(node);
                assert(value_node);
                if (value_node->token.tag == Compiler::Token::IntegerTag) {
                    Object res(Object::Int);
                    res.value = std::shared_ptr<int>{new int(std::get<Compiler::Integer>(value_node->token.value).value)};
                    return res;
                } else if (value_node->token.tag == Compiler::Token::IdentifierTag) {
                    auto identifier = std::get<Compiler::Identifier>(value_node->token.value);
                    assert(variables.contains(identifier));
                    return variables.at(identifier);
                } else if (value_node->token.tag == Compiler::Token::StringTag) {
                    return Object{Object::String, std::shared_ptr<std::string>{new std::string(std::get<Compiler::String>(value_node->token.value).value)}};
                } else {
                    assert(false);
                    return Object(Object::Struct);
                }
            } else {
                // 运算符节点
                auto l_son = EvaluateExpression(node->left);
                auto r_son = EvaluateExpression(node->right);
#define JOIN_INT_OP(op_name, symbol) else if (node->op == AST::ExpressionNode::op_name) {  \
                    assert(l_son.type == Object::Int and r_son.type == Object::Int);  \
                    Object res(Object::Int);  \
                    res.value = std::shared_ptr<int>{new int(*std::get<std::shared_ptr<int>>(l_son.value) symbol *std::get<std::shared_ptr<int>>(r_son.value))};  \
                    return res;  \
                }
                if (0) {}
                JOIN_INT_OP(Add, +)
                JOIN_INT_OP(Sub, -)
                JOIN_INT_OP(Mul, *)
                JOIN_INT_OP(Div, /)
                JOIN_INT_OP(Mod, %)
#undef JOIN_INT_OP
                else if (node->op == AST::ExpressionNode::Call) {
                    assert(l_son.type == Object::Function);
                    auto name = std::get<Identifier>(l_son.value).name;  // 函数名
                    if (name == "print") {
                        if (r_son.type == Object::Int) {
                            auto num = *std::get<std::shared_ptr<int>>(r_son.value);
                            io << num;
                        } else if (r_son.type == Object::String) {
                            io << *std::get<std::shared_ptr<std::string>>(r_son.value);
                        } else {
                            assert(false);
                        }
                        return Object(Object::None);
                    } else if (name == "println") {
                        assert(r_son.type == Object::Int);
                        auto num = *std::get<std::shared_ptr<int>>(r_son.value);
                        io << num << endl;
                        return Object(Object::None);
                    } else if (name == "scan") {
                        assert(r_son.type == Object::Int);
                        io >> *std::get<std::shared_ptr<int>>(r_son.value);
                        return Object(Object::None);
                    } else if (name == "set") {
                        // 需要两个参数
                        std::vector<Object> args(2);
                        auto it = getFunctionArguments(node->right, args.begin());  // 获取参数
                        assert(it == args.end());
                        assert(args[0].type == Object::Int and args[1].type == Object::Int);
                        *std::get<std::shared_ptr<int>>(args[0].value) = *std::get<std::shared_ptr<int>>(args[1].value);
                        return Object(Object::None);
                    } else if (name == "test_sort") {
                        std::vector<Object> args;
                        getFunctionArguments(node->right, std::back_inserter(args));

                        std::vector<int> nums(args.size());
                        ranges::transform(args, nums.begin(), [](auto &obj) {
                            assert(obj.type == Object::Int);
                            return *std::get<std::shared_ptr<int>>(obj.value);
                        });
                        ranges::sort(nums, ranges::less{});
                        for (auto x: nums)  io << x << ' ';
                        io << endl;
                        return Object(Object::None);
                    } else {
                        assert(false);
                        return Object(Object::Struct);
                    }
                } else if (node->op == AST::ExpressionNode::Assign) {
                    assert(l_son.type == Object::Int and r_son.type == Object::Int);
                    *std::get<std::shared_ptr<int>>(l_son.value) = *std::get<std::shared_ptr<int>>(r_son.value);
                    return l_son;
                } else if (node->op == AST::ExpressionNode::SplitComma) {
                    return Object(Object::None);
                } else {
                    assert(false);
                    return Object(Object::Struct);
                }
            }
        }
        void Program::runBlock(AST::BlockNode *block) {
            // todo 检查当前块类型
            if (block->type == AST::BlockNode::IfBlock or block->type == AST::BlockNode::WhileBlock) {
                auto if_block = dynamic_cast<AST::ConditionalBlockNode *>(block);
                auto condition = if_block->condition;
                assert(condition == AST::IfBlockNode::Less);  // 只支持小于号
                auto l_son = EvaluateExpression(if_block->left);
                assert(l_son.type == Object::Int);
                auto r_son = EvaluateExpression(if_block->right);
                assert(r_son.type == Object::Int);
                if (*std::get<std::shared_ptr<int>>(l_son.value) >= *std::get<std::shared_ptr<int>>(r_son.value))  return;
            }
            for (auto &x: block->statements) {
                if (x->type == AST::StatementNode::ExpressionEvaluateStatement) {
                    auto expression_evaluate_statement = dynamic_cast<AST::ExpressionEvaluateStatementNode *>(x);
                    EvaluateExpression(expression_evaluate_statement->expr);
                } else if (x->type == AST::StatementNode::VariableDeclareStatement) {
                    auto variable_declare_statement = dynamic_cast<AST::VariableDeclareStatementNode *>(x);
                    auto type = variable_declare_statement->type;
                    assert(type->op == type->NoneOp);
                    auto type_identifier = std::get<Identifier>(dynamic_cast<AST::ValueNode *>(type)->token.value);
                    assert(type_identifier.name == "int");
                    auto variable_name = variable_declare_statement->name;
                    assert(not variables.contains(variable_name));  // 重复声明
                    variables.insert({variable_name, Object(Object::Int, std::shared_ptr<int>{new int(0)})});
                } else if (x->type == AST::StatementNode::RunBlockStatement) {
                    auto run_block_statement = dynamic_cast<AST::RunBlockStatementNode *>(x);
                    runBlock(run_block_statement->block);
                } else {
                    assert(false);
                }
            }
            if (block->type == AST::BlockNode::WhileBlock) {
                runBlock(block);
            }
        }
    }
    void test() {
        auto tokens = Compiler::tokenize(io);
        auto ast = Compiler::AST::BlockNode::parse(tokens).node;
        auto program = Interpreter::Program(ast);
        program.run();
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
