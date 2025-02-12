/**
 * @link https://www.luogu.com.cn/problem/P3695
 */
#if true
#include "./libs/debug_macros.hpp"

#endif
// #pragma GCC optimize("Ofast")
// #pragma GCC optimize("inline")
// #pragma GCC optimize(3)

#include <bits/stdc++.h>
bool DEBUG_MODE=false;
#define debug if(DEBUG_MODE)
template <typename T> inline auto chkMax(T& base, const T& cmp) -> T { return (base = std::max(base, cmp)); }
template <typename T> inline auto chkMin(T& base, const T& cmp) -> T { return (base = std::min(base, cmp)); }
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

#define unreachable() (assert(false), __builtin_unreachable())
namespace lib{
#if __cplusplus > 201703LL
namespace ranges = std::ranges;
namespace views = std::views;
#endif
}

using namespace lib;

namespace MyStd {
    template <typename T>
    auto constexpr max(T const &x, T const &y) -> T { return x > y? x :y; }
    template <typename ...Ts>
    struct __MaxSizeof;
    template <>
    struct __MaxSizeof<> {
        static constexpr size_t value = 0;
    };
    template <typename T, typename ...Ts>
    struct __MaxSizeof<T, Ts...> {
        static constexpr size_t value = max(sizeof(T), __MaxSizeof<Ts...>::value);
    };

    template <typename ...Ts>
    struct __MaxAlignof;
    template <>
    struct __MaxAlignof<> {
        static constexpr size_t value = 1;
    };
    template <typename T, typename ...Ts>
    struct __MaxAlignof<T, Ts...> {
        static constexpr size_t value = max(alignof(T), __MaxAlignof<Ts...>::value);
    };

    template <int index, typename ...Ts>
    struct __IndexOfPack;
    template <int index, typename T, typename ...Ts>
    struct __IndexOfPack<index, T, Ts...> {
        using type = typename __IndexOfPack<index - 1, Ts...>::type;
    };
    template <typename T, typename ...Ts>
    struct __IndexOfPack<0, T, Ts...> {
        using type = T;
    };
    template <typename T, typename ...Ts>
    struct __FindInPack;
    template <typename T, typename U, typename ...Ts>
    struct __FindInPack<T, U, Ts...> {
        static constexpr int value = __FindInPack<T, Ts...>::value + 1;
    };
    template <typename T, typename ...Ts>
    struct __FindInPack<T, T, Ts...> {
        static constexpr int value = 0;
    };
    template <typename T>
    struct __FindInPack<T> {
        static constexpr int value = 0;
    };

    template <typename ...Ts>
    struct __Destroyer; 
    template <>
    struct __Destroyer<> {
        static auto destroy(int, char *) -> void {
            assert(false);
        }
    };
    template <typename T, typename ...Ts>
    struct __Destroyer<T, Ts...> {
        static auto destroy(int index, char *data) -> void {
            if (index == 0) {
                reinterpret_cast<T *>(data)->~T();
            } else {
                __Destroyer<Ts...>::destroy(index - 1, data);
            }
        }
    };
    template <typename ...Ts>
    struct __Copier;
    template <>
    struct __Copier<> {
        static auto copy(int, const char *, char *) -> void {
            assert(false);
        }
    };
    template <typename T, typename ...Ts>
    struct __Copier<T, Ts...> {
        static auto copy(int index, char const *src, char *dst) -> void {
            if (index == 0) {
                new (dst) T(*reinterpret_cast<T const *>(src));
            } else {
                __Copier<Ts...>::copy(index - 1, src, dst);
            }
        }
    };
    template <typename ...Ts>
    struct __Mover;
    template <>
    struct __Mover<> {
        static auto move(int, char *, char *) -> void {
            assert(false);
        }
    };
    template <typename T, typename ...Ts>
    struct __Mover<T, Ts...> {
        static auto move(int index, char *src, char *dst) -> void {
            if (index == 0) {
                new (dst) T(std::move(*reinterpret_cast<T *>(src)));
            } else {
                __Mover<Ts...>::move(index - 1, src, dst);
            }
        }
    };
    template <typename ...Ts>
    struct __CountPack;
    template <>
    struct __CountPack<> {
        static constexpr int value = 0;
    };
    template <typename T, typename ...Ts>
    struct __CountPack<T, Ts...> {
        static constexpr int value = 1 + __CountPack<Ts...>::value;
    };
    template <typename ...Ts> 
    struct alignas(std::max_align_t) variant {
        char data[__MaxSizeof<Ts...>::value];
        int index = -1;  // -1 表示无效值

        static constexpr int count = __CountPack<Ts...>::value;

        variant() = default;
        variant(variant<Ts...> const &other) {
            if (other.index != -1)  __Copier<Ts...>::copy(other.index, other.data, data);
            index = other.index;
        }
        variant(variant<Ts...> &&other) {
            if (other.index != -1)  __Mover<Ts...>::move(other.index, other.data, data);
            index = other.index;
            other.index = -1;
        }
        template <typename T, typename = typename std::enable_if<not std::is_same<typename std::remove_reference<T>::type, MyStd::variant<Ts...>>::value>::type>
        variant(T &&x) {
            set(std::forward<T>(x));
        }
        ~variant() {
            if (index != -1)  __Destroyer<Ts...>::destroy(index, data);
        }

        template <typename T>
        auto get() -> T & {
            static_assert(__FindInPack<T, Ts...>::value != count, "type not found");
            assert((index == __FindInPack<T, Ts...>::value));
            return *reinterpret_cast<T *>(data);
        }

        template <typename T>
        auto get() const -> T const & {
            static_assert(__FindInPack<T, Ts...>::value != count, "type not found");
            assert((index == __FindInPack<T, Ts...>::value));
            return *reinterpret_cast<T const *>(data);
        }

        template <typename T>
        auto set(T &&x) -> void {
            using rm_ref = typename std::remove_reference<T>::type;
            static_assert(__FindInPack<rm_ref, Ts...>::value != count, "type not found");
            index = __FindInPack<rm_ref, Ts...>::value;
            new (data) rm_ref(std::forward<T>(x));
        }

        template <typename T, typename = typename std::enable_if<not std::is_same<typename std::remove_reference<T>::type, MyStd::variant<Ts...>>::value>::type>
        auto operator= (T &&x) -> variant & {
            set(std::forward<T>(x));
            return *this;
        }
        auto operator= (variant<Ts...> const &other) -> variant & {
            if (other.index != -1)  __Copier<Ts...>::copy(other.index, other.data, data);
            index = other.index;
            return *this;
        }
        auto operator= (variant<Ts...> &&other) -> variant & {
            if (other.index != -1)  __Mover<Ts...>::move(other.index, other.data, data);
            index = other.index, other.index = -1;
            return *this;
        }
    };
}

template <typename T, typename ...Ts>
auto get(MyStd::variant<Ts...> &v) -> T & {
    return v.template get<T>();
}

template <typename T, typename ...Ts>
auto get(MyStd::variant<Ts...> const &v) -> T const & {
    return v.template get<T>();
}

namespace IO {
#ifdef __linux__
#include <sys/stat.h>
#include <sys/mman.h>
// #define IO_ENABLE_MMAP
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

    template <typename T, typename U>
    bool addOverflow(T &x, U y) {
        return __builtin_add_overflow(x, y, &x);
    }
    template <typename T, typename U>
    bool subOverflow(T &x, U y) {
        return __builtin_sub_overflow(x, y, &x);
    }
    template <typename T, typename U>
    bool mulOverflow(T &x, U y) {
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
        template <typename T, typename std::enable_if<is_number<T>::value>::type* = nullptr, int base = 10>
        Scanner &read(T &x) {
            bool sign = false;  x = 0;  char ch = get();
            for (; not isDigit(ch); ch = get()) {
                if (ch == '-')  sign = true;
                if (is_floating_point_or_float128<T>::value) {
                    if (ch == '.')  break;
                }
            }
            if (sign) {
                for (; isDigit(ch); ch = get()) {
                    // Check overflow
                    if (is_integral_or_int128<T>::value) {
                        if (mulOverflow(x, 10))  throw IntegerOverflowError{};
                        if (subOverflow(x, ch ^ 48))  throw IntegerOverflowError{};
                    } else {
                        x = x * 10 - (ch ^ 48);
                    }
                }
                if (is_integral_or_int128<T>::value)  return unget(), *this;
                T tmp = 1;
                if (ch == '.') {
                    for (ch = get(); isDigit(ch); ch = get()) {
                        tmp *= 0.1, x -= tmp * (ch ^ 48);
                    }
                }
            } else {
                for (; isDigit(ch); ch = get()) {
                    // Check overflow
                    if (is_integral_or_int128<T>::value) {
                        if (mulOverflow(x, 10))  throw IntegerOverflowError{};
                        if (addOverflow(x, ch ^ 48))  throw IntegerOverflowError{};
                    } else {
                        x = x * 10 + (ch ^ 48);
                    }
                }
                if (is_integral_or_int128<T>::value)  return unget(), *this;
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
            if (
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
    struct StringPrinter: public IO::Printer {
        std::string s;
        StringPrinter(): s() {}
        void put(char ch) {
            s.push_back(ch);
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
    
    // 标识符字符串和数字之间的映射
    using IdentifierIndexType = int;
    class IdentifierMap {
        struct IndexError: public std::exception {
            const char *err = nullptr;
            IndexError(const char *err): err(err) {}
            const char *what() const noexcept override {
                return err;
            }
        };
        // 为 vector 中的下标 +1，0 留作缺省值
        std::unordered_map<std::string, IdentifierIndexType> mappingStringToIndex;
        // int 到 string
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
            auto pair = mappingStringToIndex.insert({s, index});
            auto new_it = pair.first;
            auto success = pair.second;
            if (success)  mappingIndexToString.insert({index, s});
            return new_it->second;
        }
        bool containsString(std::string const &s) {
            return mappingStringToIndex.find(s) != mappingStringToIndex.end();
        }
        bool containsIndex(IdentifierIndexType index) {
            return mappingIndexToString.find(index) != mappingIndexToString.end();
        }
        // 指定索引插入字符串
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
        // 工具函数
        static bool isBlank(char ch) {
            return ch <= 32 or ch == 127;
        }
        static bool isIdentifierStart(char ch) {
            return ('A' <= ch and ch <= 'Z') or ('a' <= ch and ch <= 'z') or ch == '_';
        }
        static bool isDigit(char ch) {
            return '0' <= ch and ch <= '9';
        }

        // 符号表
        static Trie::Trie symbols;

        // Token 定义
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
                std::string str_name;  // 标识符的字符串形式
                // 读取一个标识符
                // 包括字母、数字、下划线
                // 不能以数字开头
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

            // std::strong_ordering operator<=> (Identifier const &other) const {
            //     return name <=> other.name;
            // }
            bool operator< (Identifier const &other) const { return name < other.name; }
            bool operator== (std::string const &other) const {
                return identifierMap.getString(name) == other;
            }
            bool operator== (IdentifierIndexType const &x) const {
                return name == x;
            }
        };
        // 整数字面量，允许带有后缀运算符
        struct Integer: public ParseAble, public DumpAble {
            std::string value;
            std::vector<Identifier> suffixOperators;  // 后缀运算符
            Integer(): value("") {}
            Integer(std::string value): value(value) {}
            
            void parse(IO::Scanner &io) override {
                value.clear();
                suffixOperators.clear();
                // 读取一个整数字面量
                // 支持十进制、十六进制、八进制、二进制
                // 支持后缀 u, U, l, L, ll, LL
                char ch = io.get();
                try {
                    if (ch == '0') {
                        value.push_back(ch);
                        ch = io.get();
                        value.push_back(ch);
                        if (ch == 'x' or ch == 'X') {
                            // 十六进制
                            for (ch = io.get(); isDigit(ch) or ('A' <= ch and ch <= 'F') or ('a' <= ch and ch <= 'f') or ch == '\''; ch = io.get()) {
                                if (ch == '\'')  continue;
                                value.push_back(ch);
                            }
                            io.unget();
                        } else if (ch == 'b' or ch == 'B') {
                            // 二进制
                            for (ch = io.get(); ch == '0' or ch == '1' or ch == '\''; ch = io.get()) {
                                if (ch == '\'')  continue;
                                value.push_back(ch);
                            }
                            io.unget();
                        } else if (ch == 'o' or ch == 'O') {
                            // 八进制
                            for (ch = io.get(); ('0' <= ch and ch <= '7') or ch == '\''; ch = io.get()) {
                                if (ch == '\'')  continue;
                                value.push_back(ch);
                            }
                            io.unget();
                        } else {
                            value.pop_back();
                            goto egg;  // 解析十进制
                        }
                    } else {
                    egg:
                        // 十进制
                        for (; isDigit(ch) or ch == '\''; ch = io.get()) {
                            if (ch == '\'')  continue;
                            value.push_back(ch);
                        }
                        io.unget();
                    }
                } catch (IO::IntegerOverflowError &) {
                    compileError("Number is too large");
                }
            }
            void dump(IO::Printer &io) const override {
                io << value;
                for (auto &op: suffixOperators) {
                    io << "'" << op;
                }
            }
        };
        // 字符串字面量，允许带有后缀运算符
        struct String: public ParseAble, public DumpAble {
            std::string value;
            std::vector<Identifier> suffixOperators;
            bool singleQuote = false;
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
                if (ch != '\"' and ch != '\'')  throw "String literal should starts with '\"'";
                singleQuote = ch == '\'';
                for (ch = io.get(); ch != (singleQuote? '\'': '\"'); ch = io.get()) {
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
                        case '\'': value.push_back('\1'); break;  // 单引号
                        case '"': value.push_back('\2'); break;  // 双引号
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
                ch = io.get();
                if (isIdentifierStart(ch)) {
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
                io << (singleQuote? '\'': '\"');
                for (auto x: value) {
                    switch (x) {
                    case '\n':  io << "\\n"; break;
                    case '\t':  io << "\\t"; break;
                    case '\r':  io << "\\r"; break;
                    case '\0':  io << "\\0"; break;
                    case '\\':  io << "\\\\"; break;
                    case '\1':  io << "\\\'"; break;
                    case '\2':  io << "\\\""; break;
                    case '\x20':  io << '\x20'; break;
                    [[likely]] default: 
                        io << x;
                    }
                }
                io << (singleQuote? '\'': '\"');
                if (not suffixOperators.empty()) {
                    io << suffixOperators.front();
                }
                // if (suffixOperators.size() > 1) {
                //     for (auto op: suffixOperators | views::drop(1)) {
                //         io << '\'' << op;
                //     }
                // }
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
            std::string value;
            std::vector<Identifier> suffixOperators;
            FloatingPointNumber(): value() {}
            FloatingPointNumber(std::string value): value(value) {}
            void parse(IO::Scanner &io) {
                char ch = io.get();
                value.clear();
                for (; isDigit(ch); ch = io.get())  value.push_back(ch);
                if (ch == '.') {
                    value.push_back('.');
                    for (ch = io.get(); isDigit(ch); ch = io.get())  value.push_back(ch);
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
            MyStd::variant<int, Identifier, Integer, String, Symbol, FloatingPointNumber> value = 0;

            Token() = default;
            Token(Tag tag, MyStd::variant<int, Identifier, Integer, String, Symbol, FloatingPointNumber> value = 0): tag(tag), value(value) {}
            friend IO::Printer &operator<<(IO::Printer &io, const Token &token) {
                switch (token.tag) {
                case Token::IdentifierTag:
                    {
                        auto s = identifierMap.getString(get<Identifier>(token.value).name);
                        static std::set<std::string> set{
                            "and", "break", "do", "else", "elseif", "end", "false", "for", "function", 
                            "if", "in", "local", "nil", "not", "or", "repeat", "return", "then", 
                            "true", "until", "while"
                        };
                        if (set.find(s) != set.end())  return io << "[RESERVED] " << s;
                        return io << "[NAME] " << s;
                    }
                case Token::IntegerTag:
                    return io << "[NUMBER] " << get<Integer>(token.value);
                case Token::StringTag:
                    return io << "[STRING] " << get<String>(token.value);
                case Token::SymbolTag:
                    return io << "[SYMBOL] " << get<Symbol>(token.value);
                case Token::EndOfLineTag:
                    return io << "[EOL]";
                case Token::FloatingPointTag:
                    return io << "[NUMBER] " << get<FloatingPointNumber>(token.value);
                default:
                    unreachable();
                    return io << "[UNKNOWN] ";
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
                        tokens.push_back(Token{Token::EndOfLineTag});
                    } else if (isBlank(ch)) {
                        io.get();
                    } else if (ch == '\"' or ch == '\'') {
                        String str;
                        io >> str;
                        tokens.push_back({Token::StringTag, str});
                    } else if (isDigit(ch) or ch == '.') {
                        Integer integer;
                        FloatingPointNumber fp;
                        bool isInteger = true;
                        if (ch != '.')  io >> integer;
                        else {
                            io.get();
                            if (io.get() == '.') {
                                // 特判运算符
                                io.unget(), io.unget();
                                goto readSymbol;
                                continue;
                            }
                            io.unget(), io.unget();
                        }
                        // 已经有了后缀运算符，不应视为浮点数
                        if (integer.suffixOperators.empty() and integer.value.substr(0, 2) != "0x") {
                            if (io.get() == '.') {
                                fp.value += '.';
                                isInteger = false;
                                if (not isDigit(io.get())) {
                                    // 特判单个点
                                    if (integer.value.empty()) {
                                        io.unget(), io.unget();
                                        goto readSymbol;
                                    }
                                    io.unget();
                                    fp.value = integer.value + fp.value;
                                    goto egg;
                                }
                                io.unget();
                                // 如果后面不是数字（类似“1.”）不用继续读
                                io.unget(), io >> fp;  // 从小数点开始读
                                fp.value = integer.value + fp.value;
                            } else {
                                io.unget();
                            }
                        egg:
                            ch = io.get();
                            if (ch == 'e' or ch == 'E') {
                                fp.value.push_back(ch);
                                // 科学计数法
                                if (isInteger)  fp.value = integer.value + fp.value;
                                ch = io.get();
                                if (ch == '+')  fp.value.push_back(ch), ch = io.get();
                                else if (ch == '-')  fp.value.push_back(ch), ch = io.get();
                                isInteger = false;
                                if (isDigit(ch)) {
                                    io.unget(), io >> integer;
                                    StringPrinter print;  print << integer;
                                    fp.value += print.s;
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
                    readSymbol:
                        Symbol symbol;
                        io >> symbol;
                        if (symbol.value == "--") {
                            // 单行注释
                            while (io.get() != '\n');
                            io.unget();
                            continue;  // 忽略井号
                        }
                        tokens.push_back({Token::SymbolTag, symbol});
                    }
                }
            } catch (IO::EOFError &) {}
            return tokens;
        }
    };
    Trie::Trie Tokenizer::symbols {
        "==", ">=", "<=", "~=", "..", "...", "--"
    };
    using Token = Tokenizer::Token;
    using Identifier = Tokenizer::Identifier;
    using Symbol = Tokenizer::Symbol;
    void solve() {
        auto tokens = Tokenizer{}.tokenize(io);
        for (auto x: tokens) {
            io << x << endl;
        }
    }
}

int main(int argc, char const *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    GenshinLang::solve();
    return 0;
}
