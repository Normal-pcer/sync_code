#include "./libs/debug_macros.hpp"

#define xxxxx * =

#include "./lib_v3.hpp"

using namespace lib;

namespace IO {
#ifdef __linux__
#include <sys/stat.h>
#include <sys/mman.h>
#define IO_ENABLE_MMAP
#endif  
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
        char prev[2] = {'\0', '\0'};
        int ungetFlag = 0;
        virtual char gc() = 0;
        static bool isDigit(char ch) { return '0' <= ch and ch <= '9'; }
        static bool isBlank(char ch) { return ch <= 32 or ch == 127; }
    public:
        char get() {
            if (ungetFlag) {
                return prev[--ungetFlag];
            }
            return (prev[1] = prev[0], prev[0] = gc());
        }
        Scanner &unget() {
            if (ungetFlag == 2)  throw std::logic_error("Cannot unget twice");
            ungetFlag++;
            return *this;
        }
        template <typename T, typename std::enable_if<is_number<T>::value>::type xxxxx  nullptr, int base = 10>
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
            >::type xxxxx  nullptr
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
#endif  
    struct Printer {
        virtual void put(char) = 0;
        virtual void flush() {}
        template <typename T, typename std::enable_if<is_floating_point_or_float128<T>::value>::type xxxxx  nullptr>
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
        template <typename T, typename std::enable_if<is_integral_or_int128<T>::value>::type xxxxx  nullptr>
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
            >::type xxxxx  nullptr>
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
#else  
    struct DefaultIO: public GetCharScanner, PutCharPrinter {};
    DefaultIO io;
#endif  
}
using IO::io;

using i32 = int32_t;  using i64 = int64_t;  using u32 = uint32_t;  using u64 = uint64_t;
using i128 = __int128;  using u128 = unsigned __int128;
using f32 = float;  using f64 = double;

namespace GenshinLang {
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
    
    
    using IdentifierIndexType = int;
    class IdentifierMap {
        struct IndexError: public std::exception {
            const char *err = nullptr;
            IndexError(const char *err): err(err) {}
            const char *what() const noexcept override {
                return err;
            }
        };
        
        std::unordered_map<std::string, IdentifierIndexType> mappingStringToIndex;
        
        std::unordered_map<int, std::string> mappingIndexToString;

    public:
        IdentifierMap();
        IdentifierIndexType getIndex(std::string const &s, IdentifierIndexType default_val) {
            auto it = mappingStringToIndex.find(s);
            if (it != mappingStringToIndex.end())  return it->second;
            else  return default_val;
        }
        IdentifierIndexType getIndex(std::string const &s) {
            auto it = mappingStringToIndex.find(s);
            if (it != mappingStringToIndex.end())  return it->second;
            else  throw IndexError("IdentifierMap::getIndex: name not found");
        }
        const std::string &getString(IdentifierIndexType index) {
            auto it = mappingIndexToString.find(index);
            if (it != mappingIndexToString.end())  return it->second;
            else  throw IndexError("IdentifierMap::getString: index out of range");
        }
        IdentifierIndexType insert(std::string const &s) {
            auto index = mappingIndexToString.size() + 1;
            while (containsIndex(index))  index++;
            auto [new_it, success] = mappingStringToIndex.insert({s, index});
            if (success)  mappingIndexToString.insert({index, s});
            return new_it->second;
        }
        bool containsString(std::string const &s) {
            return mappingStringToIndex.find(s) != mappingStringToIndex.end();
        }
        bool containsIndex(IdentifierIndexType index) {
            return mappingIndexToString.find(index) != mappingIndexToString.end();
        }
        
        void join(IdentifierIndexType index, const std::string &s) {
            assert(not containsIndex(index) and not containsString(s));
            mappingStringToIndex.insert({s, index});
            mappingIndexToString.insert({index, s});
        }
    };
    namespace Keywords {
        enum KeywordType: int {
            None, If, While, For, Var, Def, Return, Else
        };
        void joinKeywords(IdentifierMap &idMap) {
            idMap.join(If, "if");
            idMap.join(While, "while");
            idMap.join(For, "for");
            idMap.join(Var, "var");
            idMap.join(Def, "def");
            idMap.join(Return, "return");
            idMap.join(Else, "else");
        }
    }
    IdentifierMap::IdentifierMap(): mappingStringToIndex(), mappingIndexToString() {
        Keywords::joinKeywords(*this);
    }
    IdentifierMap identifierMap;

    struct Program;

    void compileError(std::string const &s) {
        ioError << s << endl;
        std::exit(-1);
    }
    class Tokenizer {
    public:
        
        static bool isBlank(char ch) {
            return ch <= 32 or ch == 127;
        }
        static bool isIdentifierStart(char ch) {
            return ('A' <= ch and ch <= 'Z') or ('a' <= ch and ch <= 'z') or ch == '_';
        }
        static bool isDigit(char ch) {
            return '0' <= ch and ch <= '9';
        }

        
        inline static Trie::Trie symbols {
            "<=", ">=", "!=", "==", "<<", ">>", "<=>", "&&", "||", "+=", "-=", "*=", "/=", "%=", "|=", "&=", "^=", "->",
            "++", "--", ".."
        };

        
        struct ParseAble {
            virtual void parse(IO::Scanner &io) = 0;
            friend IO::Scanner &operator>>(IO::Scanner &io, ParseAble &pa) {
                return pa.parse(io), io;
            }
        };
        struct DumpAble {
            virtual void dump(IO::Printer &io) const = 0;
            friend IO::Printer &operator<<(IO::Printer &io, const DumpAble &da) {
                return da.dump(io), io;
            }
        };
        
        struct Identifier: public ParseAble, public DumpAble {
            IdentifierIndexType name;
            Identifier(): name() {}
            Identifier(IdentifierIndexType name): name(name) {}
            Identifier(const std::string &str_name) {
                auto index = identifierMap.insert(str_name);
                name = index;
            }

            void parse(IO::Scanner &io) {
                std::string str_name;  
                
                
                
                char ch = io.get();
                for (; not isIdentifierStart(ch); ch = io.get());
                for (; isIdentifierStart(ch) or isDigit(ch); ch = io.get()) {
                    str_name.push_back(ch);
                }
                io.unget();
                auto index = identifierMap.insert(str_name);
                name = index;
            }
            void dump(IO::Printer &io) const override {
                io << name;
            }

            std::strong_ordering operator<=> (Identifier const &other) const {
                return name <=> other.name;
            }
            bool operator== (std::string const &other) const {
                return identifierMap.getString(name) == other;
            }
            bool operator== (IdentifierIndexType const &x) const {
                return name == x;
            }
        };
        
        struct Integer: public ParseAble, public DumpAble {
            u64 value;
            std::vector<Identifier> suffixOperators;  
            Integer(): value(0) {}
            Integer(u64 value): value(value) {}
            
            void parse(IO::Scanner &io) override {
                value = 0;
                suffixOperators.clear();
                
                
                
                char ch = io.get();
                try {
                    if (ch == '0') {
                        ch = io.get();
                        if (ch == 'x' or ch == 'X') {
                            
                            for (ch = io.get(); isDigit(ch) or ('A' <= ch and ch <= 'F') or ('a' <= ch and ch <= 'f') or ch == '\''; ch = io.get()) {
                                if (ch == '\'')  continue;
                                if (IO::leftShiftOverflow(value, 4))  throw IO::IntegerOverflowError{};
                                auto cur = ch <= '9'? ch ^ 48: (ch & 15) + 9;
                                if (IO::addOverflow(value, cur))  throw IO::IntegerOverflowError{};
                            }
                            io.unget();
                        } else if (ch == 'b' or ch == 'B') {
                            
                            for (ch = io.get(); ch == '0' or ch == '1' or ch == '\''; ch = io.get()) {
                                if (ch == '\'')  continue;
                                if (IO::leftShiftOverflow(value, 1))  throw IO::IntegerOverflowError{};
                                if (IO::addOverflow(value, ch ^ 48))  throw IO::IntegerOverflowError{};
                            }
                            io.unget();
                        } else if (ch == 'o' or ch == 'O') {
                            
                            for (ch = io.get(); ('0' <= ch and ch <= '7') or ch == '\''; ch = io.get()) {
                                if (ch == '\'')  continue;
                                if (IO::leftShiftOverflow(value, 3))  throw IO::IntegerOverflowError{};
                                if (IO::addOverflow(value, ch ^ 48))  throw IO::IntegerOverflowError{};
                            }
                            io.unget();
                        } else {
                            goto egg;  
                        }
                    } else {
                    egg:
                        
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
                
                Identifier id;
                
                
                if (ch = io.get(); isIdentifierStart(ch) and ch != 'E' and ch != 'e') {
                    io.unget();
                    io >> id;
                    suffixOperators.push_back(id);
                } else {
                    io.unget();
                }
                while (io.get() == '\'') {
                    io >> id;
                    suffixOperators.push_back(id.name);
                }
                io.unget();
            }
            void dump(IO::Printer &io) const override {
                io << value;
                for (auto &op: suffixOperators) {
                    io << "'" << op;
                }
            }
        };
        
        struct String: public ParseAble, public DumpAble {
            std::string value;
            std::vector<Identifier> suffixOperators;
            String(): value() {}
            String(std::string value): value(std::move(value)) {}
            String(std::string &&value): value(std::move(value)) {}
            void parse(IO::Scanner &io) {
                value.clear();
                suffixOperators.clear();
                
                
                
                
                char ch = io.get();
                if (ch != '"')  throw "String literal should starts with '\"'";
                for (ch = io.get(); ch != '"'; ch = io.get()) {
                    if (ch == '\\') {
                        ch = io.get();
                        switch (ch) {
                        case 'a': value.push_back('\a'); break;  
                        case 'b': value.push_back('\b'); break;  
                        case 'f': value.push_back('\f'); break;  
                        case 'n': value.push_back('\n'); break;  
                        case 'r': value.push_back('\r'); break;  
                        case 't': value.push_back('\t'); break;  
                        case 'v': value.push_back('\v'); break;  
                        case '\\': value.push_back('\\'); break;  
                        case '\'': value.push_back('\''); break;  
                        case '"': value.push_back('"'); break;  
                        case '?': value.push_back('\?'); break;  
                        case 'x': {
                            
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
                        default: value.push_back(ch); break;  
                        }
                    } else {
                        value.push_back(ch);
                    }
                }
                
                Identifier id;
                if (ch = io.get(); isIdentifierStart(ch)) {
                    io.unget();
                    io >> id;
                    suffixOperators.push_back(id);
                } else {
                    io.unget();
                }
                while (io.get() == '\'') {
                    io >> id;
                    suffixOperators.push_back(id.name);
                }
                io.unget();
            }
            void dump(IO::Printer &io) const override {
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
            void parse(IO::Scanner &io) {
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
            void dump(IO::Printer &io) const override {
                io << value;
            }
            
        };
        
        struct FloatingPointNumber: public ParseAble, public DumpAble {
            double value;
            std::vector<Identifier> suffixOperators;
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
                    suffixOperators.push_back(id);
                } else {
                    io.unget();
                }
                while (io.get() == '\'') {
                    io >> id;
                    suffixOperators.push_back(id.name);
                }
                io.unget();
            }
            void dump(IO::Printer &io) const override {
                io << value;
                for (auto i: suffixOperators)  io << '\'' << i;
            }
        };
        struct Token {
            enum Tag {
                NoneTag, IdentifierTag, SymbolTag, KeywordTag, IntegerTag, StringTag, EndOfLineTag, FloatingPointTag
            } tag = NoneTag;
            std::variant<int, Identifier, Integer, String, Symbol, FloatingPointNumber> value = 0;
            friend IO::Printer &operator<<(IO::Printer &io, const Token &token) {
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

        static std::vector<Token> tokenize(IO::Scanner &io) {
                    
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
                        
                        if (integer.suffixOperators.empty()) {
                            if (io.get() == '.') {
                                if (not isDigit(io.get())) {
                                    
                                    io.unget(), io.unget();  
                                    goto egg;  
                                }
                                io.unget();  
                                isInteger = false;
                                io >> fp;
                                fp.value += integer.value;
                            } else {
                                io.unget();
                            }
                            if (ch = io.get(); ch == 'e' or ch == 'E') {
                                
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
                    egg:
                        if (isInteger)  tokens.push_back({Token::IntegerTag, integer});
                        else  tokens.push_back({Token::FloatingPointTag, fp});
                    } else if (isIdentifierStart(ch)) {
                        Identifier identifier;
                        io >> identifier;
                        tokens.push_back({Token::IdentifierTag, identifier});
                    } else {
                        Symbol symbol;
                        io >> symbol;
                        if (symbol.value == "#") {
                            
                            while (io.get() != '\n');
                            continue;  
                        }
                        tokens.push_back({Token::SymbolTag, symbol});
                    }
                }
            } catch (IO::EOFError &) {}
            return tokens;
        }
    };
    using Token = Tokenizer::Token;
    using Identifier = Tokenizer::Identifier;
    using Symbol = Tokenizer::Symbol;
    
    
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
                IfStatement,
                WhileStatement,
                ForStatement,
                FunctionDefinitionStatement,
                ReturnStatement,
            } type = NoneStatement;
            StatementNode(Type type = NoneStatement): type(type) {}
            virtual ~StatementNode() = default;
        };
        struct BlockNode: public Node {
            enum Type {
                NoneBlock,  
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
                
                NoneOp,
                Bracket,    
                Call,       
                FunctionArgsBracket,  
                Subscript,  
                SubscriptBracket,  
                SplitComma,  
                
                UnaryAdd,  
                UnarySub,  
                Add,  
                Sub,  
                Mul,  
                Div,  
                Mod,  
                
                Less,  
                LessEqual,  
                Greater,  
                GreaterEqual,  
                Equal,  
                NotEqual,  
                
                And,  
                Or,  
                Not,  
                
                BitAnd,  
                BitOr,  
                BitXor,  
                BitNot,  
                BitShiftLeft,  
                BitShiftRight,  
                
                Assign,  
                
                Range,  
            } op = NoneOp;
            static constexpr const char *opNames[] = {"NoneOp", "Bracket", "Call", "FunctionArgsBracket", "Subscript", "SubscriptBracket", "SplitComma", "UnaryAdd", "UnarySub", "Add", "Sub", "Mul", "Div", "Mod", "Less", "LessEqual", "Greater", "GreaterEqual", "Equal", "NotEqual", "And", "Or", "Not", "BitAnd", "BitOr", "BitXor", "BitNot", "BitShiftLeft", "BitShiftRight", "Assign", "Range"};
            
            ExpressionNode *left = nullptr, *right = nullptr;
            ExpressionNode(Operator op = NoneOp): op(op) {}
            virtual ~ExpressionNode();

            struct OperatorInfo {
                static constexpr const int priority_max = 0x3f3f3f3f;
                int priority = 0;  
                bool leftAssociative = false;  
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
                case Range:  return {14, false};
                default:  return {OperatorInfo::priority_max, false};
                }
            }
            template <typename T, typename PredType>
            static ParseResult<ExpressionNode> parse(const T &, PredType &&);
            template <typename T>
            static ParseResult<ExpressionNode> parse(const T &);
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
            
            BlockNode *block;
            RunBlockStatementNode(BlockNode *block): StatementNode(StatementNode::RunBlockStatement), block(block) {}
            ~RunBlockStatementNode() {
                delete block;
            }
        };
        struct ConditionalStatementNode: public StatementNode {
            
            ExpressionNode *condition;  
            BlockNode *body;  
            ConditionalStatementNode(StatementNode::Type type): StatementNode(type) {}
            virtual ~ConditionalStatementNode() {
                delete condition;
                delete body;
            }
            template <typename T, typename Out>
            static ParseResult<Out> parse(const T &);
        };
        struct IfStatementNode: public ConditionalStatementNode {
            BlockNode *elseBody = nullptr;  
            IfStatementNode(): ConditionalStatementNode(StatementNode::IfStatement) {}

            template <typename T>
            static ParseResult<IfStatementNode> parse(const T &src) {
                return ConditionalStatementNode::parse<decltype(src), IfStatementNode>(src);
            }
        };
        struct WhileStatementNode: public ConditionalStatementNode {
            WhileStatementNode(): ConditionalStatementNode(StatementNode::WhileStatement) {}

            template <typename T>
            static ParseResult<WhileStatementNode> parse(const T &src) {
                return ConditionalStatementNode::parse<decltype(src), WhileStatementNode>(src);
            }
        };
        struct ForStatementNode: public StatementNode {
            
            
            StatementNode *init;
            ExpressionNode *condition, *step;
            BlockNode *body;

            ForStatementNode(): StatementNode(StatementNode::ForStatement) {}
            ~ForStatementNode() {
                delete init;
                delete condition;
                delete step;
                delete body;
            }
            template <typename T>
            static ParseResult<ForStatementNode> parse(const T &);
        };
        struct FunctionDefinitionStatementNode: public StatementNode {
            
            
            Identifier name;
            std::vector<VariableDeclareStatementNode *> args;
            BlockNode *body;
            FunctionDefinitionStatementNode(): StatementNode(StatementNode::FunctionDefinitionStatement) {}
            ~FunctionDefinitionStatementNode() {
                delete body;
                for (auto ptr: args)  delete ptr;
            }
            template <typename T>
            static ParseResult<FunctionDefinitionStatementNode> parse(const T &);
        };
        struct ReturnStatementNode: public StatementNode {
            ExpressionNode *expr = nullptr;
            ReturnStatementNode(): StatementNode(StatementNode::ReturnStatement) {}
            ~ReturnStatementNode() {
                delete expr;
            }
            template <typename T>
            static ParseResult<ReturnStatementNode> parse(const T &);
        };
        template <typename T>
        ParseResult<ReturnStatementNode> ReturnStatementNode::parse(const T &src) {
            auto res = new ReturnStatementNode;
            auto it = src.begin();
            
            if (it->tag == Token::SymbolTag and std::get<Symbol>(it->value).value == ";")  return {res, ++it};
            if (it->tag == Token::EndOfLineTag)  return {res, it};
            auto [expr, end] = ExpressionNode::parse(TokensSubrange{it, src.end()});
            res->expr = expr;
            it = end;
            return {res, it};
        }
        template <typename T>
        ParseResult<VariableDeclareStatementNode> VariableDeclareStatementNode::parse(const T &src) {
            
            auto res = new VariableDeclareStatementNode;
            auto it = src.begin();
            
            
            assert(it->tag == Token::IdentifierTag), res->name = std::get<Identifier>(it->value), it++;
            assert(it->tag == Token::SymbolTag and std::get<Symbol>(it->value).value == ":"), it++;
            
            
            auto [type, end] = ExpressionNode::parse(TokensSubrange{it, src.end()});
            res->type = type;
            it = end;
            return {res, it};
        }
        template <typename T>
        ParseResult<BlockNode> BlockNode::parse(T const &src) {
            auto res = new BlockNode;
            auto it = src.begin();
            for (; it != src.end();) {
                if (it->tag == Token::EndOfLineTag) {
                    it++;
                    continue;
                }
                if (it->tag == Token::SymbolTag and std::get<Symbol>(it->value).value == "}")  return {res, ++it};
                
                if (it->tag == Token::SymbolTag and std::get<Symbol>(it->value).value == "{") {
                    auto [sub_block, next] = BlockNode::parse(TokensSubrange{++it, src.end()});
                    res->statements.push_back(new RunBlockStatementNode{sub_block}), it = next;
                    continue;
                }
                
                if (it->tag == Token::IdentifierTag and std::get<Identifier>(it->value) == Keywords::If) {
                    
                    auto [if_statement, next] = IfStatementNode::parse(TokensSubrange{++it, src.end()});
                    res->statements.push_back(if_statement), it = next;
                    
                    if (it->tag == Token::IdentifierTag and std::get<Identifier>(it->value) == Keywords::Else) {
                        it++, assert(it->tag == Token::SymbolTag and std::get<Symbol>(it->value).value == "{");
                        auto [else_block, next] = BlockNode::parse(TokensSubrange{++it, src.end()});
                        if_statement->elseBody = else_block, it = next;
                    }
                    continue;
                }
                
                if (it->tag == Token::IdentifierTag and std::get<Identifier>(it->value) == Keywords::While) {
                    auto [while_statement, next] = WhileStatementNode::parse(TokensSubrange{++it, src.end()});
                    res->statements.push_back(while_statement), it = next;
                    continue;
                }
                
                if (it->tag == Token::IdentifierTag and std::get<Identifier>(it->value) == Keywords::For) {
                    auto [for_statement, next] = ForStatementNode::parse(TokensSubrange{++it, src.end()});
                    res->statements.push_back(for_statement), it = next;
                    continue;
                }

                
                
                if (it->tag == Token::SymbolTag and std::get<Symbol>(it->value).value == ":") {
                    it++;
                    
                    assert(it->tag == Token::IdentifierTag);
                    auto funcToken = *it++;
                    std::vector<Token> tmp;
                    
                    while (it != src.end() and it->tag != Token::EndOfLineTag)  tmp.push_back(*it++);
                    
                    tmp.insert(tmp.begin(), {Token::SymbolTag, Symbol("(")});
                    tmp.insert(tmp.begin(), funcToken);
                    tmp.push_back({Token::SymbolTag, Symbol(")")});
                    assert(it != src.end()), it++, tmp.push_back({Token::EndOfLineTag});
                    
                    auto [expr, next] = ExpressionNode::parse(tmp);
                    assert(expr->op == ExpressionNode::Call and next == tmp.end());
                    res->statements.push_back(new ExpressionEvaluateStatementNode{expr});
                    continue;
                }
                
                if (it->tag == Token::IdentifierTag and std::get<Identifier>(it->value) == Keywords::Var) {
                    
                    auto [decl, next] = VariableDeclareStatementNode::parse(TokensSubrange{++it, src.end()});
                    res->statements.push_back(decl), it = next;
                    continue;
                }
                
                if (it->tag == Token::IdentifierTag and std::get<Identifier>(it->value) == Keywords::Def) {
                    auto [def, next] = FunctionDefinitionStatementNode::parse(TokensSubrange{++it, src.end()});
                    res->statements.push_back(def), it = next;
                    continue;
                }
                
                if (it->tag == Token::IdentifierTag and std::get<Identifier>(it->value) == Keywords::Return) {
                    auto [ret, next] = ReturnStatementNode::parse(TokensSubrange{++it, src.end()});
                    res->statements.push_back(ret), it = next;
                    continue;
                }
                
                auto [expr, next] = ExpressionNode::parse(TokensSubrange{it, src.end()});
                res->statements.push_back(new ExpressionEvaluateStatementNode{expr});
                it = next;
            }
            return {res, src.end()};
        }
        template <typename T, typename Out>
        ParseResult<Out> ConditionalStatementNode::parse(const T &src) {
            auto res = new Out;
            auto it = src.begin();
            {
                auto [condition, next] = ExpressionNode::parse(
                    TokensSubrange{it, src.end()},
                    [](auto const &op, auto const &ops_stack) -> bool {
                        
                        if (op == "{") {
                            bool flag = true;
                            for (auto x: ops_stack) {
                                if (x.op == ExpressionNode::Bracket) {
                                    flag = false;
                                    break;
                                }
                            }
                            if (flag) {
                                return true;
                            }
                        }
                        return false;
                    }
                );
                res->condition = condition;
                it = next;
            }{
                assert(it != src.end());
                auto [body, next] = BlockNode::parse(TokensSubrange{it, src.end()});
                res->body = body;
                it = next;
            }
            return {res, it};
        }
        template <typename T>
        ParseResult<ForStatementNode> ForStatementNode::parse(const T &src) {
            auto res = new ForStatementNode;
            auto it = src.begin();
            
            assert(it != src.end() and it->tag == Token::SymbolTag and std::get<Symbol>(it->value).value == "(");
            it++;
            
            {
                
                auto [expr, next] = ExpressionNode::parse(TokensSubrange{it, src.end()});
                res->init = new ExpressionEvaluateStatementNode{expr};
                it = next;
            }
            
            {
                auto [expr, next] = ExpressionNode::parse(TokensSubrange{it, src.end()});
                res->condition = expr, it = next;
            }
            
            {
                
                auto end_condition = [](auto const &op, auto const &ops_stack) -> bool {
                    if (op == ")") {
                        bool flag = true;
                        for (auto it = ops_stack.rbegin(); it != ops_stack.rend(); it++) {
                            if (it->op == ExpressionNode::Bracket) {
                                flag = false;
                            }
                        }
                        if (flag) {
                            return true;
                        }
                    }
                    return false;
                };
                auto [expr, next] = ExpressionNode::parse(TokensSubrange{it, src.end()}, end_condition);
                res->step = expr, it = next;
            }
            
            assert(it != src.end() and it->tag == Token::SymbolTag and std::get<Symbol>(it->value).value == "{");
            {
                auto [body, next] = BlockNode::parse(TokensSubrange{++it, src.end()});
                res->body = body, it = next;
            }
            return {res, it};
        }
        ExpressionNode::~ExpressionNode() {
            if (left)  delete left;
            if (right)  delete right;
        }
        template <typename T>
        ParseResult<ExpressionNode> ExpressionNode::parse(const T &src) {
            return parse(src, 0);
        }
        template <typename T, typename PredType>
        ParseResult<ExpressionNode> ExpressionNode::parse(const T &src, PredType &&pred) {
            struct StackValueType {
                Operator op;
                int args_remains;  
            };
            const auto &&end_condition = [&]() {
                if constexpr(std::is_same_v<PredType, int>) {
                    return [&](std::string const &op, std::vector<StackValueType> const &ops) {
                        if (op == "," and (ops.empty() or ops.back().op == NoneOp))  return true;
                        if (op == ";" or op == "\n")  return true;
                        return false;
                    };
                } else {
                    return pred;
                }
            }();
            auto [postfix, it] = [&]() {                
                const auto inf = OperatorInfo::priority_max;
                std::vector<StackValueType> ops {{NoneOp, 1}};  
                auto it = src.begin();
                
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
                    never io << __LINE__ << token << endl;
                    if (token.tag == Token::SymbolTag or token.tag == Token::EndOfLineTag) {
                        auto op = token.tag == Token::EndOfLineTag? "\n": std::get<Symbol>(token.value).value;
                        if (end_condition(op, ops)) {
                            it++;
                            break;
                        }
                        if (op == "(") {
                            
                            
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
                            
                            if (ops.back().op == FunctionArgsBracket) {
                                bool flag = ops.back().args_remains == 1;
                                ops.pop_back();
                                
                                if (flag)  postfix.push_back({false, ValueNode{ValueNode::NoneValue, Token{Token::NoneTag}}});
                            } else {
                                
                                assert(ops.back().op == Bracket and ops.back().args_remains == 0);
                                ops.pop_back();
                            }
                        } else if (op == ",") {
                            while (not ops.empty() and infoOf(ops.back().op).priority < infoOf(SplitComma).priority) {
                                postfix.push_back({true, {ops.back().op}}), ops.pop_back();
                            }
                            ops.push_back({SplitComma, 1});
                        } else if (op == "[") {
                            add(Subscript, 0);
                            ops.push_back({SubscriptBracket, 1});
                        } else if (op == "]") {
                            
                            while (infoOf(ops.back().op).priority != inf) {
                                postfix.push_back({true, ops.back().op}), ops.pop_back();
                            }
                            assert(ops.back().op == SubscriptBracket);
                            bool flag = ops.back().args_remains == 1;
                            
                            ops.pop_back();
                            if (flag)  postfix.push_back({false, ValueNode{ValueNode::NoneValue, Token{Token::NoneTag}}});
                        } else if (op == "+") {
                            
                            
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
                        JOIN_BINARY_OP(Range, "..")
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
                
                while (not ops.empty() and ops.size() != (size_t)1) {
                    postfix.push_back({true, ops.back().op});
                    ops.pop_back();
                }
                return std::pair{postfix, it};
            }();
            
            never for (auto &x: postfix) {
                if (x.symbol) {
                    io << __LINE__ << "Operator: " << (int)std::get<Operator>(x.item) << '\x20' << opNames[(int)std::get<Operator>(x.item)] << endl;
                } else {
                    io << __LINE__ << "Value: ";
                    auto token = std::get<ValueNode>(x.item).token;
                    io << token << endl;
                }
            }

            
            
            
            std::vector<AST::ExpressionNode *> nodes_stack;
            auto countOf = [&](Operator op) {
                switch (op) {
                case UnaryAdd:  return 1;
                case UnarySub:  return 1;
                case Not:  return 1;
                case BitNot:  return 1;
                default:  return 2;
                }
            };  
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
            
            assert(nodes_stack.size() == (size_t)1);
            return {nodes_stack.back(), it};
        }
        template <typename T>
        ParseResult<FunctionDefinitionStatementNode> FunctionDefinitionStatementNode::parse(const T &src) {
            auto res = new FunctionDefinitionStatementNode;
            auto it = src.begin();
            
            {
                auto name = std::get<Identifier>(it->value);
                res->name = name, it++;
            }
            assert(it->tag == Token::SymbolTag and std::get<Symbol>(it->value).value == "("), it++;
            
            while (true) {
                if (it->tag == Token::SymbolTag and std::get<Symbol>(it->value).value == ")") {
                    it++;
                    break;
                }
                auto arg = new VariableDeclareStatementNode;
                arg->type = nullptr;
                
                assert(it->tag == Token::IdentifierTag);
                arg->name = std::get<Identifier>(it->value), it++;
                bool stop = false;  
                
                
                if (it->tag == Token::SymbolTag) {
                    if (std::get<Symbol>(it->value).value == ":") {
                        
                        auto end_condition = [&](auto const &op, auto const &op_stack) -> bool {
                            if (op == "," or op == ")") {
                                
                                bool flag = false;
                                for (auto x: op_stack) {
                                    if (x.op == ExpressionNode::Bracket) {
                                        flag = true;
                                        break;
                                    }
                                }
                                if (not flag) {
                                    if (op == ")")  stop = true;
                                    return true;
                                }
                            }
                            return false;
                        };
                        auto [expr, next] = ExpressionNode::parse(TokensSubrange{++it, src.end()}, end_condition);
                        arg->type = expr, it = next;
                    } else if (std::get<Symbol>(it->value).value == ",") {
                        it++;
                    } else if (std::get<Symbol>(it->value).value == ")") {
                        it++;
                        stop = true;
                    }
                }
                res->args.push_back(arg);
                if (stop)  break;
            }
            assert(it->tag == Token::SymbolTag and std::get<Symbol>(it->value).value == "{");
            
            {
                auto [body, next] = BlockNode::parse(TokensSubrange{++it, src.end()});
                res->body = body, it = next;
            }
            return {res, it};
        }
    }
    
    struct Program {
        AST::BlockNode *root;
        IdentifierMap identifierMap;
        Program(AST::BlockNode *root, IdentifierMap identifierMap) : root(root), identifierMap(identifierMap) {}
        Program(Program &&other): root(other.root), identifierMap(std::move(other.identifierMap)) {
            other.root = nullptr;
        }
        ~Program() {
            if (root != nullptr) {
                delete root;
            }
        }
    };
    namespace Interpreter {
        struct ArrayObjectValue;
        struct Object {
            enum Type {
                Struct, Int, Function, String, None, Array, BuiltinFunction, Long
            } type;
            std::variant<
                std::nullptr_t, 
                std::shared_ptr<std::map<Identifier, Object>>, 
                std::shared_ptr<int>, 
                std::shared_ptr<std::string>, 
                std::shared_ptr<Identifier>,
                std::shared_ptr<ArrayObjectValue>,
                AST::FunctionDefinitionStatementNode *,
                std::shared_ptr<i64>
            > value;
            Object(Type type = None): type(type), value(nullptr) {}
            template <typename T>
            Object(Type type, const T &value): type(type), value(value) {}

            Object copy() {
                if (type == Int) {
                    return Object{Int, std::make_shared<int>(*std::get<std::shared_ptr<int>>(value))};
                } else if (type == Long) {
                    return Object{Long, std::make_shared<i64>(*std::get<std::shared_ptr<i64>>(value))};
                } else {
                    assert(false), __builtin_unreachable();
                    return Object{};
                }
            }
        };
        struct ArrayMeta;
        struct TypeName {
            enum Type {
                Undefined, Int, Array, Function, Long
            } type;
            std::variant<std::nullptr_t, std::shared_ptr<ArrayMeta>> meta;
            TypeName(Type type = Undefined): type(type), meta(nullptr) {}
            TypeName(Type type, const auto &meta): type(type), meta(meta) {}
            TypeName(TypeName const &other): type(other.type), meta(other.meta) {}
        };
        struct ArrayMeta {
            std::shared_ptr<TypeName> valueType;
            int minIndex, maxIndex;
        };
        struct ArrayObjectValue {
            std::shared_ptr<TypeName> type;
            std::vector<Object> values;
            ArrayObjectValue(std::shared_ptr<TypeName> type, std::vector<Object> values): type(type), values(values) {}
            ArrayObjectValue(ArrayObjectValue const &other): type(other.type), values(other.values) {}
            ArrayObjectValue(std::shared_ptr<TypeName> type): type(type), values() {
                auto meta = std::get<std::shared_ptr<ArrayMeta>>(type->meta);
                auto value_type = meta->valueType;
                auto min = meta->minIndex, max = meta->maxIndex;
                assert(min <= max);
                values.resize(max - min + 1);
                if (value_type->type == TypeName::Int) {
                    for (auto &x: values)  x = Object{Object::Int, std::shared_ptr<int>{new int{0}}};
                } else {
                    for (auto &x: values) {
                        x = Object{Object::Array, std::shared_ptr<ArrayObjectValue>{new ArrayObjectValue{value_type}}};
                    }
                }
            }
            Object &operator[](int index) {
                auto meta = std::get<std::shared_ptr<ArrayMeta>>(type->meta);
                auto min = meta->minIndex, max = meta->maxIndex;
                assert(index >= min and index <= max);
                return values[index - min];
            }
        };
        
        struct NameError: std::exception {
            std::string message;
            NameError(std::string name): message(
                std::format("NameError: name '{}' is not defined", name)
            ) {}
            
            const char *what() const noexcept override {
                return message.c_str();
            }
        };
        
        class Interpreter {
        public:
            struct Scope {
                std::map<Identifier, Object> variables;
                Scope *parent = nullptr;  
                Scope(Scope *parent = nullptr): parent(parent) {}

                
                Object &get(Identifier name) {
                    auto it = variables.find(name);
                    if (it != variables.end())  return it->second;
                    else {
                        if (parent != nullptr)  return parent->get(name);
                        else  throw NameError(identifierMap.getString(name.name));
                    }
                }
                
                Object *getPtr(Identifier name) {
                    auto it = variables.find(name);
                    if (it != variables.end())  return &it->second;
                    else {
                        if (parent != nullptr)  return parent->getPtr(name);
                        else  return nullptr;
                    }
                }
                
                void declare(Identifier name, TypeName type) {
                    assert(not variables.contains(name));  
                    if (type.type == type.Int) {
                        variables.insert({name, Object(Object::Int, std::make_shared<int>(0))});
                    } else if (type.type == type.Long) {
                        variables.insert({name, Object(Object::Long, std::make_shared<i64>(0))});
                    } else if (type.type == type.Array) {
                        auto value = std::shared_ptr<ArrayObjectValue>{new ArrayObjectValue{std::shared_ptr<TypeName>{new TypeName{type}}}};
                        variables.insert({name, Object(Object::Array, value)});
                    } else if (type.type == type.Function) {
                        variables.insert({name, Object{Object::Function}});
                    } else if (type.type == TypeName::Undefined) {
                        variables.insert({name, Object{}});
                    } else {
                        assert(false);
                    }
                }
            };
            Program program;
            std::vector<std::unique_ptr<Scope>> scopeStack;  
            Object ret;  
            bool returnFlag = false;  
            Interpreter(Program &&);

            void run();
            Object evaluateExpression(AST::ExpressionNode *);  
            TypeName evaluateType(AST::ExpressionNode *);  
            template <typename OutIterator>
            OutIterator getFunctionArguments(AST::ExpressionNode *, OutIterator);  
            void runBlock(AST::BlockNode *);  
            template <typename StatementPointer>
            void runDeclarationStatement(StatementPointer);  
            template <typename StatementPointer>
            void runStatement(StatementPointer);  
            void enterScope();  
            void leaveScope();  
            Interpreter::Scope *topScope();  
        };
        Interpreter::Interpreter(Program &&program): program(std::move(program)) {
            
            scopeStack.push_back(std::make_unique<Scope>());

            
            topScope()->variables.insert({{"print"}, Object{Object::BuiltinFunction, std::shared_ptr<Identifier>{new Identifier{"print"}}}});
            topScope()->variables.insert({{"println"}, Object{Object::BuiltinFunction, std::shared_ptr<Identifier>{new Identifier{"println"}}}});
            topScope()->variables.insert({{"scan"}, Object{Object::BuiltinFunction, std::shared_ptr<Identifier>{new Identifier{"scan"}}}});
            topScope()->variables.insert({{"set"}, Object{Object::BuiltinFunction, std::shared_ptr<Identifier>{new Identifier{"set"}}}});
            topScope()->variables.insert({{"test_sort"}, Object{Object::BuiltinFunction, std::shared_ptr<Identifier>{new Identifier{"test_sort"}}}});
            topScope()->variables.insert({{"yosoro"}, Object{Object::BuiltinFunction, std::shared_ptr<Identifier>{new Identifier{"yosoro"}}}});
        }
        
        void Interpreter::enterScope() {
            scopeStack.push_back(std::make_unique<Scope>(topScope()));
        }
        
        void Interpreter::leaveScope() {
            assert(not scopeStack.empty());
            scopeStack.pop_back();
        }
        
        
        
        
        template <typename OutIterator>
        OutIterator Interpreter::getFunctionArguments(AST::ExpressionNode *node, OutIterator out) {
            if (node->op == AST::ExpressionNode::SplitComma) {
                
                out = getFunctionArguments(node->left, out);
                out = getFunctionArguments(node->right, out);
            } else {
                if (node->op == AST::ExpressionNode::NoneOp) {
                    
                    auto value_node = dynamic_cast<AST::ValueNode *>(node);
                    
                    
                    if (value_node->token.tag == Token::NoneTag)  return out;
                }
                
                *out++ = evaluateExpression(node);
            }
            return out;
        }
        void Interpreter::run() {
            runBlock(program.root);
        }
        Object Interpreter::evaluateExpression(AST::ExpressionNode *node) {
            if (node == nullptr)  return Object{Object::None};
            if (node->op == node->NoneOp) {
                
                auto value_node = dynamic_cast<AST::ValueNode *>(node);
                assert(value_node);
                if (value_node->token.tag == Token::IntegerTag) {
                    Object res(Object::Int);
                    res.value = std::shared_ptr<int>{new int(std::get<Tokenizer::Integer>(value_node->token.value).value)};
                    return res;
                } else if (value_node->token.tag == Token::IdentifierTag) {
                    auto identifier = std::get<Identifier>(value_node->token.value);
                    return topScope()->get(identifier);
                } else if (value_node->token.tag == Token::StringTag) {
                    return Object{Object::String, std::shared_ptr<std::string>{new std::string(std::get<Tokenizer::String>(value_node->token.value).value)}};
                } else if (value_node->token.tag == Token::NoneTag) {
                    return Object{};
                } else {
                    assert(false);
                    return Object(Object::Struct);
                }
            } else {
                
                auto l_son = evaluateExpression(node->left);
                auto r_son = evaluateExpression(node->right);
#define JOIN_INT_OP(op_name, symbol) else if (node->op == AST::ExpressionNode::op_name) {  \
                    if (l_son.type == Object::Int and r_son.type == Object::Int) {  \
                        Object res(Object::Int);  \
                        assert(l_son.type == l_son.Int and r_son.type == r_son.Int); \
                        res.value = std::make_shared<int>(*std::get<std::shared_ptr<int>>(l_son.value) symbol *std::get<std::shared_ptr<int>>(r_son.value));  \
                        return res;  \
                    } else { \
                        Object res(Object::Long);  \
                        assert(l_son.type == l_son.Long and r_son.type == r_son.Long); \
                        res.value = std::make_shared<i64>(*std::get<std::shared_ptr<i64>>(l_son.value) symbol *std::get<std::shared_ptr<i64>>(r_son.value));  \
                        return res;  \
                    } \
                }
                if (0) {}
                JOIN_INT_OP(Add, +)
                JOIN_INT_OP(Sub, -)
                JOIN_INT_OP(Mul, *)
                JOIN_INT_OP(Div, /)
                JOIN_INT_OP(Mod, %)
                JOIN_INT_OP(Less, <)
                JOIN_INT_OP(Greater, >)
                JOIN_INT_OP(LessEqual, <=)
                JOIN_INT_OP(GreaterEqual, >=)
                JOIN_INT_OP(Equal, ==)
                JOIN_INT_OP(NotEqual, !=)
                JOIN_INT_OP(BitShiftLeft, <<)
                JOIN_INT_OP(BitShiftRight, >>)
                JOIN_INT_OP(BitOr, |)
                JOIN_INT_OP(Or, ||)
                JOIN_INT_OP(And, &&)
#undef JOIN_INT_OP
                else if (node->op == AST::ExpressionNode::UnaryAdd) {
                    assert(l_son.type == Object::Int);
                    return l_son;
                } else if (node->op == AST::ExpressionNode::UnarySub) {
                    assert(l_son.type == Object::Int);
                    return Object{Object::Int, std::shared_ptr<int>{new int(-*std::get<std::shared_ptr<int>>(l_son.value))}};
                } else if (node->op == AST::ExpressionNode::Call) {
                    if (l_son.type == Object::BuiltinFunction) {
                        auto name = *std::get<std::shared_ptr<Identifier>>(l_son.value);  
                        if (name == "print") {
                            if (r_son.type == Object::Int) {
                                auto num = *std::get<std::shared_ptr<int>>(r_son.value);
                                io << num;
                            } else if (r_son.type == Object::String) {
                                io << *std::get<std::shared_ptr<std::string>>(r_son.value);
                            } else {
                                assert(false);
                            }
                            return Object{};
                        } else if (name == "println") {
                            assert(r_son.type == Object::Int);
                            auto num = *std::get<std::shared_ptr<int>>(r_son.value);
                            io << num << endl;
                            return Object{};
                        } else if (name == "scan") {
                            assert(r_son.type == Object::Int);
                            io >> *std::get<std::shared_ptr<int>>(r_son.value);
                            return Object{};
                        } else if (name == "set") {
                            
                            std::vector<Object> args(2);
                            auto it = getFunctionArguments(node->right, args.begin());  
                            assert(it == args.end());
                            assert(args[0].type == Object::Int and args[1].type == Object::Int);
                            *std::get<std::shared_ptr<int>>(args[0].value) = *std::get<std::shared_ptr<int>>(args[1].value);
                            return Object{};
                        } else if (name == "test_sort") {
                            std::vector<Object> args;
                            getFunctionArguments(node->right, std::back_inserter(args));

                            std::vector<int> nums(args.size());
                            ranges::transform(args, nums.begin(), [](auto &obj) {
                                assert(obj.type == Object::Int);
                                return *std::get<std::shared_ptr<int>>(obj.value);
                            });
                            ranges::sort(nums, ranges::less{});
                            for (auto x: nums)  io << x << '\x20';
                            io << endl;
                            return Object{};
                        } else if (name == "yosoro") {
                            assert(r_son.type == Object::Int);
                            auto num = *std::get<std::shared_ptr<int>>(r_son.value);
                            io << num << '\x20';
                            return Object{};
                        } else {
                            assert(false);
                            return Object(Object::Struct);
                        }
                    } else {
                        assert(l_son.type == Object::Function);
                        auto func = std::get<AST::FunctionDefinitionStatementNode *>(l_son.value);
                        std::vector<Object> args;
                        getFunctionArguments(node->right, std::back_inserter(args));
                        assert(args.size() == func->args.size());
                        auto size = static_cast<int>(args.size());
                        enterScope();
                        for (auto i = 0; i < size; i++) {
                            topScope()->declare(func->args[i]->name, evaluateType(func->args[i]->type));
                            topScope()->get(func->args[i]->name) = args[i].copy();
                        }
                        runBlock(func->body);
                        leaveScope();
                        if (returnFlag)  return returnFlag = false, ret;
                        else  return Object{};
                    }
                } else if (node->op == AST::ExpressionNode::Assign) {
                    assert(l_son.type == Object::Int and r_son.type == Object::Int);
                    *std::get<std::shared_ptr<int>>(l_son.value) = *std::get<std::shared_ptr<int>>(r_son.value);
                    return l_son;
                } else if (node->op == AST::ExpressionNode::SplitComma) {
                    return Object{};
                } else if (node->op == AST::ExpressionNode::Subscript) {
                    assert(l_son.type == Object::Array and r_son.type == Object::Int);
                    auto &array = *std::get<std::shared_ptr<ArrayObjectValue>>(l_son.value);
                    return array[*std::get<std::shared_ptr<int>>(r_son.value)];
                } else {
                    assert(false);
                    return Object(Object::Struct);
                }
            }
        }
        TypeName Interpreter::evaluateType(AST::ExpressionNode *node) {
            if (node == nullptr)  return {TypeName::Undefined};
            if (node->op == AST::ExpressionNode::NoneOp) {
                auto value_node = dynamic_cast<AST::ValueNode *>(node);
                if (value_node->token.tag == Token::IdentifierTag) {
                    const auto &name = std::get<Identifier>(value_node->token.value);
                    if (name == "int") {
                        return {TypeName::Int};
                    } else if (name == "long") {
                        return {TypeName::Long};
                    } else if (name == "array") {
                        return {TypeName::Array};
                    }
                }
            }
            else {
                if (node->op == AST::ExpressionNode::Subscript) {
                    
                    assert(node->right->op == AST::ExpressionNode::SplitComma);
                    auto value_type = evaluateType(node->right->left);
                    auto range_node = node->right->right;
                    auto min_node = evaluateExpression(range_node->left), max_node = evaluateExpression(range_node->right);
                    assert(min_node.type == Object::Int and max_node.type == Object::Int);
                    auto min = *std::get<std::shared_ptr<int>>(min_node.value);
                    auto max = *std::get<std::shared_ptr<int>>(max_node.value);
                    return TypeName{TypeName::Array, std::shared_ptr<ArrayMeta>{
                            new ArrayMeta{std::shared_ptr<TypeName>{new TypeName{value_type}}, min, max
                        }}};
                }
            }
            assert(false);
            return {};
        }
        template <typename StatementPointer>
        void Interpreter::runDeclarationStatement(StatementPointer node) {
            auto variable_declare_statement = node;
            auto type_node = variable_declare_statement->type;

            auto type = evaluateType(type_node);
            auto variable_name = variable_declare_statement->name;
            topScope()->declare(variable_name, type);
        }
        
        void Interpreter::runBlock(AST::BlockNode *block) {
            enterScope();
            for (auto &x: block->statements) {
                runStatement(x);
                if (returnFlag)  return leaveScope(), void();
            }
            leaveScope();
        }
        template <typename StatementPointer>
        void Interpreter::runStatement(StatementPointer x) {
            if (x->type == AST::StatementNode::ExpressionEvaluateStatement) {
                auto expression_evaluate_statement = dynamic_cast<AST::ExpressionEvaluateStatementNode *>(x);
                evaluateExpression(expression_evaluate_statement->expr);
            } else if (x->type == AST::StatementNode::VariableDeclareStatement) {
                runDeclarationStatement(dynamic_cast<AST::VariableDeclareStatementNode *>(x));
            } else if (x->type == AST::StatementNode::RunBlockStatement) {
                auto run_block_statement = dynamic_cast<AST::RunBlockStatementNode *>(x);
                runBlock(run_block_statement->block);
                if (returnFlag)  return;
            } else if (x->type == AST::StatementNode::IfStatement) {
                auto if_statement = dynamic_cast<AST::IfStatementNode *>(x);
                auto condition = evaluateExpression(if_statement->condition);
                assert(condition.type == Object::Int);
                if (*std::get<std::shared_ptr<int>>(condition.value) != 0) {
                    runBlock(if_statement->body);
                } else if (if_statement->elseBody != nullptr) {
                    runBlock(if_statement->elseBody);
                }
            } else if (x->type == AST::StatementNode::ForStatement) {
                auto for_statement = dynamic_cast<AST::ForStatementNode *>(x);
                runStatement(for_statement->init);
                if (returnFlag)  return;
                while (true) {
                    auto condition = evaluateExpression(for_statement->condition);
                    assert(condition.type == Object::Int);
                    if (*std::get<std::shared_ptr<int>>(condition.value) == 0)  break;
                    runBlock(for_statement->body);
                    if (returnFlag)  return;
                    evaluateExpression(for_statement->step);
                }
            } else if (x->type == AST::StatementNode::FunctionDefinitionStatement) {
                auto function_definition_statement = dynamic_cast<AST::FunctionDefinitionStatementNode *>(x);
                
                topScope()->declare(function_definition_statement->name, TypeName{TypeName::Function});
                topScope()->get(function_definition_statement->name).value = function_definition_statement;
            } else if (x->type == AST::StatementNode::ReturnStatement) {
                
                auto return_statement = dynamic_cast<AST::ReturnStatementNode *>(x);
                ret = evaluateExpression(return_statement->expr);
                returnFlag = true;
            } else {
                assert(false);
            }
        }
        Interpreter::Scope *Interpreter::topScope() {
            return scopeStack.back().get();
        }
    }
    void test() {
        auto tokens = Tokenizer{}.tokenize(io);
        std::map<Identifier, int> cnt;
        for (auto x: tokens) {
            if (x.tag == x.IdentifierTag) {
                auto id = std::get<Identifier>(x.value);
                cnt[id] += identifierMap.getString(id.name).size();
            }
        }
        std::vector<std::pair<int, std::string>> ans;
        for (auto [key, val]: cnt) {
            ans.push_back({val, identifierMap.getString(key.name)});
        }
        ranges::sort(ans, ranges::greater{});
        for (auto [x, y]: ans) {
            std::cout << x << ' ' << y << std::endl;
        }
        auto letters = ranges::to<std::vector<char>>(
            ((ranges::to<std::string>(views::iota('A', 'Z'+1))) + (ranges::to<std::string>(views::iota('a', 'z'+1))))
                | views::filter([](char ch) {
                    std::string str{ch};
                    return identifierMap.containsString(str); })
        );
        auto lis = ans | views::take(letters.size());

        for (auto [x, y]: views::zip(lis, letters)) {
            std::cout << std::format("#define {} {}", y, x.second) << std::endl;
        }
    }
    void solve() {
        test();
        return;
        std::string code = R"raw(var n: int;
var m: int;
scan(n);
scan(m);

var i: int;
var init: array[int, 1..n];
for (i = 1; i <= n; i = i + 1) {
    scan(init[i]);
}

var size: int;
size = n << 2;
var begin: array[int, 1..size];
var end: array[int, 1..size];
var sum: array[int, 1..size];
var tag: array[int, 1..size];

def pushUp(p: int) {
    sum[p] = sum[p << 1] + sum[p << 1 | 1];
}

def pushDown(p: int) {
    if (tag[p]) {
        sum[p << 1] = sum[p << 1] + (end[p << 1] - begin[p << 1]) * tag[p];
        sum[p << 1 | 1] = sum[p << 1 | 1] + (end[p << 1 | 1] - begin[p << 1 | 1]) * tag[p];
        tag[p << 1] = tag[p << 1] + tag[p];
        tag[p << 1 | 1] = tag[p << 1 | 1] + tag[p];
        tag[p] = 0;
    }
}

def build(bg: int, ed: int, p: int) {
    begin[p] = bg;
    end[p] = ed;
    if (bg + 1 == ed) {
        sum[p] = init[bg];
        return;
    } else {
        var mid: int;
        mid = (bg + ed) >> 1;
        build(bg, mid, p << 1);
        build(mid, ed, p << 1 | 1);
    }
}

def query(bg: int, ed: int, p: int) {
    if (begin[p] >= bg && end[p] <= ed) {
        return sum[p];
    }
    pushDown(p);
    var res: int;
    res = 0;
    if (end[p << 1] > bg) { res = res + query(bg, ed, p << 1); }
    if (begin[p << 1 | 1] < ed) { res = res + query(bg, ed, p << 1 | 1); }
    return res;
}

def update(bg: int, ed: int, val: int, p: int) {
    if (begin[p] >= bg && end[p] <= ed) {
        sum[p] = sum[p] + val * (end[p] - begin[p]);
        tag[p] = tag[p] + val;
        return;
    }
    pushDown(p);
    if (end[p << 1] > bg) { update(bg, ed, val, p << 1); }
    if (begin[p << 1 | 1] < ed) { update(bg, ed, val, p << 1 | 1); }
    pushUp(p);
}

build(1, n+1, 1);

for (i = 0; i < m; i = i + 1) {
    var op: int;
    var x: int;
    var y: int;
    scan(op);
    scan(x);
    scan(y);
    if (op == 1) {
        var k: int;
        scan(k);
        update(x, y+1, k, 1);
    } else {
        print(query(x, y+1, 1));
        print("\n");
    }
}
)raw";
        StringScanner scan(code);
        auto tokens = Tokenizer{}.tokenize(scan);
        auto ast = AST::BlockNode::parse(tokens).node;
        auto interpreter = Interpreter::Interpreter(Program{ast, IdentifierMap{}});
        interpreter.run();
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    GenshinLang::solve();
    return 0;
}
