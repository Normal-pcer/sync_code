#include "./libs/debug_macros.hpp"

#define A return
#define B ExpressionNode
#define E std
#define F value
#define O auto
#define R Object
#define T shared_ptr
#define U Identifier
#define X type
#define Z StatementNode
#define a push_back
#define b const
#define c if
#define e else
#define f typename
#define i template
#define n FunctionDefinitionStatementNode
#define o struct
#define r get
#define s VariableDeclareStatementNode
#define t assert
#define x Token
#define y it
#define z false

#include <bits/stdc++.h>
bool DEBUG_MODE=z;
#define debug c(DEBUG_MODE)
i <f T> inline O chkMax(T& base, b T& cmp) { A (base = E::max(base, cmp)); }
i <f T> inline O chkMin(T& base, b T& cmp) { A (base = E::min(base, cmp)); }
#define never c constexpr(0)
b int inf = 0x3f3f3f3f;  b long long infLL = 0x3f3f3f3f3f3f3f3fLL; using ll = long long; using ull = unsigned long long;
b char endl = '\n';

#define __lambda_1(expr) [&](){A expr;}
#define __lambda_2(a, expr) [&](O a){A expr;}
#define __lambda_3(a, b, expr) [&](O a, O b){A expr;}
#define __lambda_4(a, b, c, expr) [&](O a, O b, O c){A expr;}
#define __lambda_overload(a, b, c, d, e, args...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__, 4, 3, 2, 1)(__VA_ARGS__)
#define lam lambda
namespace lib{
#if __cplusplus > 201703LL
namespace ranges = E::ranges;
namespace views = E::views;
#endif
}

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
    o EOFError: public E::exception {
        b char *what() b throw() {
            A "EOF when reading a char";
        }
    };
    o IntegerOverflowError: public E::exception {
        b char *what() b throw() override {
            A "Integer Overflow";
        }
    };
    i <f T>  o is_integral_or_int128 { constexpr static bool F = E::is_integral<T>::F; };
    i <>  o is_integral_or_int128<__int128> { constexpr static bool F = true; };
    i <>  o is_integral_or_int128<unsigned __int128> { constexpr static bool F = true; };
    i <f T>  o is_floating_point_or_float128 { constexpr static bool F = E::is_floating_point<T>::F; };
    i <>  o is_floating_point_or_float128<__float128> { constexpr static bool F = true; };
    i <f T> o is_number {
        constexpr static bool F = is_integral_or_int128<T>::F || is_floating_point_or_float128<T>::F;
    };

    bool addOverflow(O &x, O y) {
        A __builtin_add_overflow(x, y, &x);
    }
    bool subOverflow(O &x, O y) {
        A __builtin_sub_overflow(x, y, &x);
    }
    bool mulOverflow(O &x, O y) {
        A __builtin_mul_overflow(x, y, &x);
    }
    i <f T, f U>
    bool leftShiftOverflow(T &x, U y) {
        c (x == 0)  A z;
        bool flag = E::__lg(x) + y >= E::numeric_limits<T>::digits;
        A x <<= y, flag;
    }

    o Scanner {
    private:
        char prev[2] = {'\0', '\0'};
        int ungetFlag = 0;
        virtual char gc() = 0;
        static bool isDigit(char ch) { A '0' <= ch and ch <= '9'; }
        static bool isBlank(char ch) { A ch <= 32 or ch == 127; }
    public:
        char r() {
            c (ungetFlag) {
                A prev[--ungetFlag];
            }
            A (prev[1] = prev[0], prev[0] = gc());
        }
        Scanner &unget() {
            c (ungetFlag == 2)  throw E::logic_error("Cannot unget twice");
            ungetFlag++;
            A *this;
        }
        i <f T, f E::enable_if<is_number<T>::F>::X * =  nullptr, int base = 10>
        Scanner &read(T &x) {
            bool sign = z;  x = 0;  char ch = r();
            for (; not isDigit(ch); ch = r()) {
                c (ch == '-')  sign = true;
                c constexpr (is_floating_point_or_float128<T>::F) {
                    c (ch == '.')  break;
                }
            }
            c (sign) {
                for (; isDigit(ch); ch = r()) {
                    
                    c constexpr (is_integral_or_int128<T>::F) {
                        c (mulOverflow(x, 10))  throw IntegerOverflowError{};
                        c (subOverflow(x, ch ^ 48))  throw IntegerOverflowError{};
                    } e {
                        x = x * 10 - (ch ^ 48);
                    }
                }
                c constexpr (is_integral_or_int128<T>::F)  A unget(), *this;
                T tmp = 1;
                c (ch == '.') {
                    for (ch = r(); isDigit(ch); ch = r()) {
                        tmp *= 0.1, x -= tmp * (ch ^ 48);
                    }
                }
            } e {
                for (; isDigit(ch); ch = r()) {
                    
                    c constexpr (is_integral_or_int128<T>::F) {
                        c (mulOverflow(x, 10))  throw IntegerOverflowError{};
                        c (addOverflow(x, ch ^ 48))  throw IntegerOverflowError{};
                    } e {
                        x = x * 10 + (ch ^ 48);
                    }
                }
                c constexpr (is_integral_or_int128<T>::F)  A unget(), *this;
                T tmp = 1;
                c (ch == '.') {
                    for (ch = r(); isDigit(ch); ch = r()) {
                        tmp *= 0.1, x += tmp * (ch ^ 48);
                    }
                }
            }
            c (ch == 'e' or ch == 'E') {
                int y;  read(y);
                x *= pow(10, y);
            }
            A unget(), *this;
        }
        Scanner &read(char &x) {
            for (x = r(); isBlank(x); x = r());
            A *this;
        }
        Scanner &read(char *s) {
            char ch = r();
            for (; isBlank(ch); ch = r());
            for (; not isBlank(ch); ch = r())  *s++ = ch;
            *s = '\0';
            A unget(), *this;
        }
        Scanner &read(E::string &s, int reserve = 0) {
            char ch = r();
            s.clear(), s.reserve(reserve);
            for (; isBlank(ch); ch = r());
            for (; not isBlank(ch); ch = r())  s.a(ch); 
            A unget(), *this;
        }
        i <f T, f E::enable_if<
                is_number<T>::F || E::is_convertible<T, b char *>::F || E::is_convertible<T, E::string b &>::F
            >::X * =  nullptr
        >
        Scanner &operator>> (T &x) {
            A read(x);
        }
        
    };
    i <size_t MaxSize>
    o FileReadScanner: public Scanner {
    private:
        char buf[MaxSize], *p1, *p2;
        bool eofFlag = z;

        char gc_fread() {
            c (p1 == p2) {
                c (eofFlag)  throw EOFError{};
                p1 = buf;
                p2 = buf + E::fread(buf, sizeof(char), sizeof(buf), stdin);
                c (E::feof(stdin)) eofFlag = true;
            }
            A p1 == p2? '\0': *p1++;
        }
    protected:
        char gc() { A gc_fread(); }
    public:
        FileReadScanner(): p1(buf), p2(buf) {}
    };
    o GetCharScanner: public Scanner {
    private:
        char gc_getchar() {
            char ch = getchar();
            c (ch != EOF)  A ch;
            e {
                throw EOFError{};
                A '\0';
            }
        }
    protected:
        char gc() { A gc_getchar(); }
    };
#ifdef IO_ENABLE_MMAP
    o MemoryMapScanner: public Scanner {
        o stat s;
        char *c;
        MemoryMapScanner() {
            fstat(0, &s);
            c = (char *)mmap(nullptr, s.st_size, 1, 2, 0, 0);
        }
        char gc() { A *c++; }
    };
#endif  
    o Printer {
        virtual void put(char) = 0;
        virtual void flush() {}
        i <f T, f E::enable_if<is_floating_point_or_float128<T>::F>::X * =  nullptr>
        Printer &write(T x) {
            c (E::isnan(x))  A write("nan");
            static char st[E::numeric_limits<T>::max_exponent10+1];
            char *top = st;
            c (x < 0)  x = -x, put('-');
            c (E::isinf(x))  A write("Infinity");
            x += 5e-7;
            O y = E::floor(x);
            while (y >= 1) {
                O cur = y - (E::floor(y / 10) * 10);
                y = (y - cur) / 10;
                *top++ = (int)(cur) ^ 48;
            }
            c (top == st)  put('0');
            while (top != st)  put(*--top);
            x -= E::floor(x);
            put('.');
            for (O i = 0; i < 6; i++) {
                x = x * 10;
                O cur = E::floor(x);
                x -= cur;
                put((int)cur ^ 48);
            }
            A *this;
        }
        i <f T, f E::enable_if<is_integral_or_int128<T>::F>::X * =  nullptr>
        Printer &write(T x) {
            static char st[E::numeric_limits<T>::digits10+1];
            char *top = st;
            c (x < 0) {
                put('-');
                *top++ = -(x % 10) ^ 48, x = -(x / 10);
                c (x == 0) {
                    put(*--top);
                    A *this;
                }
            }
            do {
                *top++ = x % 10 ^ 48, x /= 10;
            } while (x);
            while (top != st)  put(*--top);
            A *this;
        }
        Printer &write(char ch) {
            put(ch);
            A *this;
        }
        Printer &write(b char *s) {
            for (; *s; s++)  put(*s);
            A *this;
        }
        Printer &write(E::string b &s) {
            for (O ch: s)  put(ch);
            A *this;
        }
        i <f T, f E::enable_if<
                is_number<T>::F || E::is_convertible<T, b char *>::F || E::is_convertible<T, E::string b &>::F
            >::X * =  nullptr>
        Printer &operator<< (b T &x) {
            c constexpr (
                is_number<T>::F || E::is_convertible<T, b char *>::F || E::is_convertible<T, E::string b &>::F
            ) {
                A write(x);
            }
        }
    };
    o PutCharPrinter: public Printer {
        void put(char ch) {
            E::putchar(ch);
        }
    };
    i <size_t MaxSize>
    o FileWritePrinter: public Printer {
        char pbuf[MaxSize], *pp;
        FileWritePrinter(): pp(pbuf) {}
        ~FileWritePrinter() {
            E::fwrite(pbuf, 1, pp - pbuf, stdout);
        }
        void flush() {
            E::fwrite(pbuf, 1, MaxSize, stdout);
            pp = pbuf;
        }
        void put(char ch) {
            c (pp - pbuf == MaxSize)  flush();
            *pp++ = ch;
        }
    };

#ifdef IO_ENABLE_MMAP
    i <size_t MaxSize>
    o DefaultIO: public MemoryMapScanner, FileWritePrinter<MaxSize> {};
    DefaultIO<1<<20> io;
#else  
    o DefaultIO: public GetCharScanner, PutCharPrinter {};
    DefaultIO io;
#endif  
}
using IO::io;

using i32 = int32_t;  using i64 = int64_t;  using u32 = uint32_t;  using u64 = uint64_t;
using i128 = __int128;  using u128 = unsigned __int128;
using f32 = float;  using f64 = double;

namespace GenshinLang {
    i <size_t MaxSize>
    o FileWritePrinterStdandardError: public IO::Printer {
        char pbuf[MaxSize], *pp;
        FileWritePrinterStdandardError(): pp(pbuf) {}
        ~FileWritePrinterStdandardError() {
            E::fwrite(pbuf, 1, pp - pbuf, stderr);
        }
        void flush() {
            E::fwrite(pbuf, 1, MaxSize, stderr);
            pp = pbuf;
        }
        void put(char ch) override {
            c (pp - pbuf == MaxSize)  flush();
            *pp++ = ch;
        }
    };
    FileWritePrinterStdandardError<1<<20> ioError;
    o StringScanner: public IO::Scanner {
        E::string &s;
        size_t index;
        bool eofFlag = z;
        StringScanner(E::string &s): s(s), index(0) {}
        char gc() override {
            c (index < s.size())  eofFlag = z;
            c (index == s.size()) {
                c (eofFlag)  throw IO::EOFError{};
                e  eofFlag = true;
                A '\0';
            }
            A index == s.size()? '\0': s[index++];
        }
    };
    namespace Trie {
        class Trie {
        public:
            o Node {
                E::array<Node *, 96> next;
                int match;
            };
            E::deque<Node> nodes;
            Node *root;
            Trie(): nodes(), root(nullptr) {
                nodes.a({});
                root = &nodes.back();
            }
            Trie(E::initializer_list<E::string> &&init): nodes(), root(nullptr) {
                nodes.a({});
                root = &nodes.back();
                for (b O &s: init)  insert(s);
            }
            
            int match(b E::string &s) b {
                Node *cur = root;
                for (O ch: s) {
                    t(ch > 32), ch -= 32;
                    c (cur->next[ch])  cur = cur->next[ch];
                    e  A 0;
                }
                A cur->match;
            }
            void insert(b E::string &s) {
                Node *cur = root;
                for (O ch: s) {
                    t(ch > 32), ch -= 32;
                    cur->match++;
                    c (cur->next[ch])  cur = cur->next[ch];
                    e  nodes.a({}), cur = cur->next[ch] = &nodes.back();
                }
                cur->match++;
            }
        };
    }
    
    
    using IdentifierIndexType = int;
    class IdentifierMap {
        o IndexError: public E::exception {
            b char *err = nullptr;
            IndexError(b char *err): err(err) {}
            b char *what() b noexcept override {
                A err;
            }
        };
        
        E::unordered_map<E::string, IdentifierIndexType> mappingStringToIndex;
        
        E::unordered_map<int, E::string> mappingIndexToString;

    public:
        IdentifierMap();
        IdentifierIndexType getIndex(E::string b &s, IdentifierIndexType default_val) {
            O y = mappingStringToIndex.find(s);
            c (y != mappingStringToIndex.end())  A y->second;
            e  A default_val;
        }
        IdentifierIndexType getIndex(E::string b &s) {
            O y = mappingStringToIndex.find(s);
            c (y != mappingStringToIndex.end())  A y->second;
            e  throw IndexError("IdentifierMap::getIndex: name not found");
        }
        b E::string &getString(IdentifierIndexType index) {
            O y = mappingIndexToString.find(index);
            c (y != mappingIndexToString.end())  A y->second;
            e  throw IndexError("IdentifierMap::getString: index out of range");
        }
        IdentifierIndexType insert(E::string b &s) {
            O index = mappingIndexToString.size() + 1;
            while (containsIndex(index))  index++;
            O [new_it, success] = mappingStringToIndex.insert({s, index});
            c (success)  mappingIndexToString.insert({index, s});
            A new_it->second;
        }
        bool containsString(E::string b &s) {
            A mappingStringToIndex.find(s) != mappingStringToIndex.end();
        }
        bool containsIndex(IdentifierIndexType index) {
            A mappingIndexToString.find(index) != mappingIndexToString.end();
        }
        
        void join(IdentifierIndexType index, b E::string &s) {
            t(not containsIndex(index) and not containsString(s));
            mappingStringToIndex.insert({s, index});
            mappingIndexToString.insert({index, s});
        }
    };
    namespace Keywords {
        enum KeywordType: int {
            None, If, While, For, Var, Def, Return, Else
        };
        void joinKeywords(IdentifierMap &idMap) {
            idMap.join(If, "c");
            idMap.join(While, "while");
            idMap.join(For, "for");
            idMap.join(Var, "var");
            idMap.join(Def, "def");
            idMap.join(Return, "A");
            idMap.join(Else, "e");
        }
    }
    IdentifierMap::IdentifierMap(): mappingStringToIndex(), mappingIndexToString() {
        Keywords::joinKeywords(*this);
    }
    IdentifierMap identifierMap;

    o Program;

    void compileError(E::string b &s) {
        ioError << s << endl;
        E::exit(-1);
    }
    class Tokenizer {
    public:
        
        static bool isBlank(char ch) {
            A ch <= 32 or ch == 127;
        }
        static bool isIdentifierStart(char ch) {
            A ('A' <= ch and ch <= 'Z') or ('a' <= ch and ch <= 'z') or ch == '_';
        }
        static bool isDigit(char ch) {
            A '0' <= ch and ch <= '9';
        }

        
        inline static Trie::Trie symbols {
            "<=", ">=", "!=", "==", "<<", ">>", "<=>", "&&", "||", "+=", "-=", "*=", "/=", "%=", "|=", "&=", "^=", "->",
            "++", "--", ".."
        };

        
        o ParseAble {
            virtual void parse(IO::Scanner &io) = 0;
            friend IO::Scanner &operator>>(IO::Scanner &io, ParseAble &pa) {
                A pa.parse(io), io;
            }
        };
        o DumpAble {
            virtual void dump(IO::Printer &io) b = 0;
            friend IO::Printer &operator<<(IO::Printer &io, b DumpAble &da) {
                A da.dump(io), io;
            }
        };
        
        o U: public ParseAble, public DumpAble {
            IdentifierIndexType name;
            U(): name() {}
            U(IdentifierIndexType name): name(name) {}
            U(b E::string &str_name) {
                O index = identifierMap.insert(str_name);
                name = index;
            }

            void parse(IO::Scanner &io) {
                E::string str_name;  
                
                
                
                char ch = io.r();
                for (; not isIdentifierStart(ch); ch = io.r());
                for (; isIdentifierStart(ch) or isDigit(ch); ch = io.r()) {
                    str_name.a(ch);
                }
                io.unget();
                O index = identifierMap.insert(str_name);
                name = index;
            }
            void dump(IO::Printer &io) b override {
                io << name;
            }

            E::strong_ordering operator<=> (U b &other) b {
                A name <=> other.name;
            }
            bool operator== (E::string b &other) b {
                A identifierMap.getString(name) == other;
            }
            bool operator== (IdentifierIndexType b &x) b {
                A name == x;
            }
        };
        
        o Integer: public ParseAble, public DumpAble {
            u64 F;
            E::vector<U> suffixOperators;  
            Integer(): F(0) {}
            Integer(u64 F): F(value) {}
            
            void parse(IO::Scanner &io) override {
                F = 0;
                suffixOperators.clear();
                
                
                
                char ch = io.r();
                try {
                    c (ch == '0') {
                        ch = io.r();
                        c (ch == 'x' or ch == 'X') {
                            
                            for (ch = io.r(); isDigit(ch) or ('A' <= ch and ch <= 'F') or ('a' <= ch and ch <= 'f') or ch == '\''; ch = io.r()) {
                                c (ch == '\'')  continue;
                                c (IO::leftShiftOverflow(F, 4))  throw IO::IntegerOverflowError{};
                                O cur = ch <= '9'? ch ^ 48: (ch & 15) + 9;
                                c (IO::addOverflow(F, cur))  throw IO::IntegerOverflowError{};
                            }
                            io.unget();
                        } e c (ch == 'b' or ch == 'B') {
                            
                            for (ch = io.r(); ch == '0' or ch == '1' or ch == '\''; ch = io.r()) {
                                c (ch == '\'')  continue;
                                c (IO::leftShiftOverflow(F, 1))  throw IO::IntegerOverflowError{};
                                c (IO::addOverflow(F, ch ^ 48))  throw IO::IntegerOverflowError{};
                            }
                            io.unget();
                        } e c (ch == 'o' or ch == 'O') {
                            
                            for (ch = io.r(); ('0' <= ch and ch <= '7') or ch == '\''; ch = io.r()) {
                                c (ch == '\'')  continue;
                                c (IO::leftShiftOverflow(F, 3))  throw IO::IntegerOverflowError{};
                                c (IO::addOverflow(F, ch ^ 48))  throw IO::IntegerOverflowError{};
                            }
                            io.unget();
                        } e {
                            goto egg;  
                        }
                    } e {
                    egg:
                        
                        for (; isDigit(ch) or ch == '\''; ch = io.r()) {
                            c (ch == '\'')  continue;
                            c (IO::mulOverflow(F, 10))  throw IO::IntegerOverflowError{};
                            c (IO::addOverflow(F, ch ^ 48))  throw IO::IntegerOverflowError{};
                        }
                        io.unget();
                    }
                } catch (IO::IntegerOverflowError &) {
                    compileError("Number is too large");
                }
                
                U id;
                
                
                c (ch = io.r(); isIdentifierStart(ch) and ch != 'E' and ch != 'e') {
                    io.unget();
                    io >> id;
                    suffixOperators.a(id);
                } e {
                    io.unget();
                }
                while (io.r() == '\'') {
                    io >> id;
                    suffixOperators.a(id.name);
                }
                io.unget();
            }
            void dump(IO::Printer &io) b override {
                io << F;
                for (O &op: suffixOperators) {
                    io << "'" << op;
                }
            }
        };
        
        o String: public ParseAble, public DumpAble {
            E::string F;
            E::vector<U> suffixOperators;
            String(): F() {}
            String(E::string F): F(E::move(F)) {}
            String(E::string &&F): F(E::move(F)) {}
            void parse(IO::Scanner &io) {
                F.clear();
                suffixOperators.clear();
                
                
                
                
                char ch = io.r();
                c (ch != '"')  throw "String literal should starts with '\"'";
                for (ch = io.r(); ch != '"'; ch = io.r()) {
                    c (ch == '\\') {
                        ch = io.r();
                        switch (ch) {
                        case 'a': F.a('\a'); break;  
                        case 'b': F.a('\b'); break;  
                        case 'f': F.a('\f'); break;  
                        case 'n': F.a('\n'); break;  
                        case 'r': F.a('\r'); break;  
                        case 't': F.a('\t'); break;  
                        case 'v': F.a('\v'); break;  
                        case '\\': F.a('\\'); break;  
                        case '\'': F.a('\''); break;  
                        case '"': F.a('"'); break;  
                        case '?': F.a('\?'); break;  
                        case 'x': {
                            
                            int x = 0;
                            for (int i = 0; i < 2; i++) {
                                ch = io.r();
                                c ('0' <= ch and ch <= '9')  x = x * 16 + (ch ^ 48);
                                e c ('A' <= ch and ch <= 'F')  x = x * 16 + (ch & 15) + 9;
                                e c ('a' <= ch and ch <= 'f')  x = x * 16 + (ch & 15) + 9;
                                e  compileError("Invalid hex digit");
                            }
                            F.a(x);
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
                                ch = io.r();
                                c ('0' <= ch and ch <= '7')  x = x * 8 + (ch ^ 48);
                                e {
                                    io.unget();
                                    break;
                                }
                            }
                            F.a(x);
                        } break;
                        default: F.a(ch); break;  
                        }
                    } e {
                        F.a(ch);
                    }
                }
                
                U id;
                c (ch = io.r(); isIdentifierStart(ch)) {
                    io.unget();
                    io >> id;
                    suffixOperators.a(id);
                } e {
                    io.unget();
                }
                while (io.r() == '\'') {
                    io >> id;
                    suffixOperators.a(id.name);
                }
                io.unget();
            }
            void dump(IO::Printer &io) b override {
                io << '"';
                for (O x: F) {
                    switch (x) {
                    case '\n':  io << "\\n"; break;
                    case '\t':  io << "\\t"; break;
                    case '\r':  io << "\\r"; break;
                    case '\0':  io << "\\0"; break;
                    case '\\':  io << "\\\\"; break;
                    case '\"':  io << "\\\""; break;
                    case '\x20':  io << '\x20'; break;
                    [[likely]] default: 
                        c (isBlank(x))  io << E::format("\\x{:02X}", x);
                        e  io << x;
                    }
                }
                io << '"';
                c (not suffixOperators.empty()) {
                    io << suffixOperators.front();
                }
                c (suffixOperators.size() > 1) {
                    for (O op: suffixOperators | views::drop(1)) {
                        io << '\'' << op;
                    }
                }
            }
        };
        o Symbol: public ParseAble, public DumpAble {
            E::string F;
            Symbol(): F({}) {}
            Symbol(E::string F): F(value) {}
            void parse(IO::Scanner &io) {
                F.clear();
                char ch = io.r();
                F.a(ch);
                O *cur = symbols.root->next[ch - 32];
                bool flag = z;
                while (cur) {
                    ch = io.r(), flag = true;
                    c (isBlank(ch) or isDigit(ch) or isIdentifierStart(ch))  break;
                    c (cur->next[ch - 32])  cur = cur->next[ch - 32];
                    e  break;
                    F.a(ch);
                }
                c (flag)  io.unget();
            }
            void dump(IO::Printer &io) b override {
                io << F;
            }
            
        };
        
        o FloatingPointNumber: public ParseAble, public DumpAble {
            double F;
            E::vector<U> suffixOperators;
            FloatingPointNumber(): F(0) {}
            FloatingPointNumber(double F): F(value) {}
            void parse(IO::Scanner &io) {
                double tmp = 1;
                char ch = io.r();
                F = 0;
                for (; isDigit(ch); ch = io.r())  F = F * 10 + (ch ^ 48);
                c (ch == '.') {
                    for (ch = io.r(); isDigit(ch); ch = io.r())  tmp *= 0.1, F += tmp * (ch ^ 48);
                }
                U id;
                c (isIdentifierStart(ch) and ch != 'E' and ch != 'e') {
                    io.unget();
                    io >> id;
                    suffixOperators.a(id);
                } e {
                    io.unget();
                }
                while (io.r() == '\'') {
                    io >> id;
                    suffixOperators.a(id.name);
                }
                io.unget();
            }
            void dump(IO::Printer &io) b override {
                io << F;
                for (O i: suffixOperators)  io << '\'' << i;
            }
        };
        o x {
            enum Tag {
                NoneTag, IdentifierTag, SymbolTag, KeywordTag, IntegerTag, StringTag, EndOfLineTag, FloatingPointTag
            } tag = NoneTag;
            E::variant<int, U, Integer, String, Symbol, FloatingPointNumber> F = 0;
            friend IO::Printer &operator<<(IO::Printer &io, b x &token) {
                switch (token.tag) {
                case x::IdentifierTag:
                    A io << "Identifier_" << E::r<U>(token.F);
                case x::IntegerTag:
                    A io << "Integer" << E::r<Integer>(token.F);
                case x::StringTag:
                    A io << "String" << E::r<String>(token.F);
                case x::SymbolTag:
                    A io << "Symbol" << E::r<Symbol>(token.F);
                case x::EndOfLineTag:
                    A io << "EOL";
                case x::FloatingPointTag:
                    A io << "FloatingPoint" << E::r<FloatingPointNumber>(token.F);
                default:
                    A io << "Unknown";
                }
            }
        };

        static E::vector<x> tokenize(IO::Scanner &io) {
                    
            E::vector<x> tokens;
            try {
                while (true) {
                    char ch = io.r();
                    io.unget();
                    c (ch == '\0') {
                        break;
                    } c (ch == '\n') {
                        io.r();
                        tokens.a({x::EndOfLineTag});
                    } e c (isBlank(ch)) {
                        io.r();
                    } e c (ch == '"') {
                        String str;
                        io >> str;
                        tokens.a({x::StringTag, str});
                    } e c (isDigit(ch)) {
                        Integer integer;
                        FloatingPointNumber fp;
                        bool isInteger = true;
                        io >> integer;
                        
                        c (integer.suffixOperators.empty()) {
                            c (io.r() == '.') {
                                c (not isDigit(io.r())) {
                                    
                                    io.unget(), io.unget();  
                                    goto egg;  
                                }
                                io.unget();  
                                isInteger = z;
                                io >> fp;
                                fp.F += integer.F;
                            } e {
                                io.unget();
                            }
                            c (ch = io.r(); ch == 'e' or ch == 'E') {
                                
                                c (isInteger)  fp.F = integer.F;
                                isInteger = z;
                                ch = io.r();
                                bool expSigned = z;
                                c (ch == '+')  ch = io.r();
                                e c (ch == '-')  ch = io.r(), expSigned = true;
                                c (isDigit(ch)) {
                                    io.unget(), io >> integer;
                                    c (expSigned)  fp.F *= E::pow(10.0, -(double)integer.F);
                                    e  fp.F *= E::pow(10, integer.F);
                                    fp.suffixOperators = E::move(integer.suffixOperators);
                                } e {
                                    compileError("Invalid decimal literal");
                                }
                            } e {
                                io.unget();
                            }
                        }
                    egg:
                        c (isInteger)  tokens.a({x::IntegerTag, integer});
                        e  tokens.a({x::FloatingPointTag, fp});
                    } e c (isIdentifierStart(ch)) {
                        U identifier;
                        io >> identifier;
                        tokens.a({x::IdentifierTag, identifier});
                    } e {
                        Symbol symbol;
                        io >> symbol;
                        c (symbol.F == "#") {
                            
                            while (io.r() != '\n');
                            continue;  
                        }
                        tokens.a({x::SymbolTag, symbol});
                    }
                }
            } catch (IO::EOFError &) {}
            A tokens;
        }
    };
    using x = Tokenizer::x;
    using U = Tokenizer::U;
    using Symbol = Tokenizer::Symbol;
    
    
    namespace AST {
        using TokensType = b E::vector<x>;
        using TokenIterator = TokensType::const_iterator;
        using TokensSubrange = E::ranges::subrange<TokenIterator>;
        o Node {
            Node() = default;
            Node(b Node &) = delete;
            virtual ~Node() = default;
            Node &operator= (b Node &) = delete;
        };
        i <f T>
        o ParseResult {
            T *node;
            TokenIterator y;
        };
        o Z: public Node {
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
            } X = NoneStatement;
            Z(Type X = NoneStatement): X(type) {}
            virtual ~Z() = default;
        };
        o BlockNode: public Node {
            enum Type {
                NoneBlock,  
            } X = NoneBlock;
            E::vector<Z *> statements;
            BlockNode() = default;
            BlockNode(Type X): X(type), statements() {}
            virtual ~BlockNode() {
                for (O &statement: statements)  delete statement;
            }
            i <f T>
            static ParseResult<BlockNode> parse(b T &src);
        };
        o B: public Node {
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
            static constexpr b char *opNames[] = {"NoneOp", "Bracket", "Call", "FunctionArgsBracket", "Subscript", "SubscriptBracket", "SplitComma", "UnaryAdd", "UnarySub", "Add", "Sub", "Mul", "Div", "Mod", "Less", "LessEqual", "Greater", "GreaterEqual", "Equal", "NotEqual", "And", "Or", "Not", "BitAnd", "BitOr", "BitXor", "BitNot", "BitShiftLeft", "BitShiftRight", "Assign", "Range"};
            
            B *left = nullptr, *right = nullptr;
            B(Operator op = NoneOp): op(op) {}
            virtual ~B();

            o OperatorInfo {
                static constexpr b int priority_max = 0x3f3f3f3f;
                int priority = 0;  
                bool leftAssociative = z;  
            };
            static constexpr OperatorInfo infoOf(Operator op) {
                switch (op) {
                case Call:  A {2, z};
                case Subscript:  A {2, z};
                case SplitComma:  A {17, z};
                case UnaryAdd:  A {3, true};
                case UnarySub:  A {3, true};
                case Add:  A {6, z};
                case Sub:  A {6, z};
                case Mul:  A {5, z};
                case Div:  A {5, z};
                case Mod:  A {5, z};
                case Less:  A {9, z};
                case LessEqual:  A {9, z};
                case Greater:  A {9, z};
                case GreaterEqual:  A {9, z};
                case Equal:  A {10, z};
                case NotEqual:  A {10, z};
                case And:  A {14, z};
                case Or:  A {15, z};
                case Not:  A {3, true};
                case BitAnd:  A {11, z};
                case BitOr:  A {13, z};
                case BitXor:  A {12, z};
                case BitNot:  A {3, true};
                case BitShiftLeft:  A {7, z};
                case BitShiftRight:  A {7, z};
                case Assign:  A {16, true};
                case Range:  A {14, z};
                default:  A {OperatorInfo::priority_max, z};
                }
            }
            i <f T, f PredType>
            static ParseResult<B> parse(b T &, PredType &&);
            i <f T>
            static ParseResult<B> parse(b T &);
        };
        o ValueNode: public B {
            enum Type {
                NoneValue,
                Integer,
                FloatingPoint,
                String,
                U
            } X = NoneValue;
            x token;

            ValueNode(Type X, x token): 
                B({B::NoneOp}), X(type), token(token) {}
            ValueNode(b ValueNode &other): 
                B({B::NoneOp}), X(other.X), token(other.token) {}
            ValueNode(x token): B({B::NoneOp}), X(NoneValue), token(token) {
                X = [&]() {
                    switch (token.tag) {
                    case x::IntegerTag:  A Integer;
                    case x::FloatingPointTag:  A FloatingPoint;
                    case x::StringTag:  A String;
                    case x::IdentifierTag:  A U;
                    default:  A t(z), NoneValue;
                    }
                }();
            }
        };
        o ExpressionEvaluateStatementNode: public Z {
            B *expr;
            ExpressionEvaluateStatementNode(B *expr):
                Z(StatementNode::ExpressionEvaluateStatement), expr(expr){}
            ~ExpressionEvaluateStatementNode() {
                delete expr;
            }
        };
        o s: public Z {
            U name;
            B *X;
            s(): Z(StatementNode::VariableDeclareStatement) {}
            ~s() {
                delete X;
            }
            i <f T>
            static ParseResult<s> parse(b T &src);
        };
        o RunBlockStatementNode: public Z {
            
            BlockNode *block;
            RunBlockStatementNode(BlockNode *block): Z(StatementNode::RunBlockStatement), block(block) {}
            ~RunBlockStatementNode() {
                delete block;
            }
        };
        o ConditionalStatementNode: public Z {
            
            B *condition;  
            BlockNode *body;  
            ConditionalStatementNode(Z::Type X): Z(X) {}
            virtual ~ConditionalStatementNode() {
                delete condition;
                delete body;
            }
            i <f T, f Out>
            static ParseResult<Out> parse(b T &);
        };
        o IfStatementNode: public ConditionalStatementNode {
            BlockNode *elseBody = nullptr;  
            IfStatementNode(): ConditionalStatementNode(Z::IfStatement) {}

            i <f T>
            static ParseResult<IfStatementNode> parse(b T &src) {
                A ConditionalStatementNode::parse<decltype(src), IfStatementNode>(src);
            }
        };
        o WhileStatementNode: public ConditionalStatementNode {
            WhileStatementNode(): ConditionalStatementNode(Z::WhileStatement) {}

            i <f T>
            static ParseResult<WhileStatementNode> parse(b T &src) {
                A ConditionalStatementNode::parse<decltype(src), WhileStatementNode>(src);
            }
        };
        o ForStatementNode: public Z {
            
            
            Z *init;
            B *condition, *step;
            BlockNode *body;

            ForStatementNode(): Z(StatementNode::ForStatement) {}
            ~ForStatementNode() {
                delete init;
                delete condition;
                delete step;
                delete body;
            }
            i <f T>
            static ParseResult<ForStatementNode> parse(b T &);
        };
        o n: public Z {
            
            
            U name;
            E::vector<s *> args;
            BlockNode *body;
            n(): Z(StatementNode::FunctionDefinitionStatement) {}
            ~n() {
                delete body;
                for (O ptr: args)  delete ptr;
            }
            i <f T>
            static ParseResult<n> parse(b T &);
        };
        o ReturnStatementNode: public Z {
            B *expr = nullptr;
            ReturnStatementNode(): Z(StatementNode::ReturnStatement) {}
            ~ReturnStatementNode() {
                delete expr;
            }
            i <f T>
            static ParseResult<ReturnStatementNode> parse(b T &);
        };
        i <f T>
        ParseResult<ReturnStatementNode> ReturnStatementNode::parse(b T &src) {
            O res = new ReturnStatementNode;
            O y = src.begin();
            
            c (y->tag == x::SymbolTag and E::r<Symbol>(y->F).F == ";")  A {res, ++y};
            c (y->tag == x::EndOfLineTag)  A {res, y};
            O [expr, end] = B::parse(TokensSubrange{y, src.end()});
            res->expr = expr;
            y = end;
            A {res, y};
        }
        i <f T>
        ParseResult<s> s::parse(b T &src) {
            
            O res = new s;
            O y = src.begin();
            
            
            t(y->tag == x::IdentifierTag), res->name = E::r<U>(y->F), y++;
            t(y->tag == x::SymbolTag and E::r<Symbol>(y->F).F == ":"), y++;
            
            
            O [X, end] = B::parse(TokensSubrange{y, src.end()});
            res->X = X;
            y = end;
            A {res, y};
        }
        i <f T>
        ParseResult<BlockNode> BlockNode::parse(T b &src) {
            O res = new BlockNode;
            O y = src.begin();
            for (; y != src.end();) {
                c (y->tag == x::EndOfLineTag) {
                    y++;
                    continue;
                }
                c (y->tag == x::SymbolTag and E::r<Symbol>(y->F).F == "}")  A {res, ++y};
                
                c (y->tag == x::SymbolTag and E::r<Symbol>(y->F).F == "{") {
                    O [sub_block, next] = BlockNode::parse(TokensSubrange{++y, src.end()});
                    res->statements.a(new RunBlockStatementNode{sub_block}), y = next;
                    continue;
                }
                
                c (y->tag == x::IdentifierTag and E::r<U>(y->F) == Keywords::If) {
                    
                    O [if_statement, next] = IfStatementNode::parse(TokensSubrange{++y, src.end()});
                    res->statements.a(if_statement), y = next;
                    
                    c (y->tag == x::IdentifierTag and E::r<U>(y->F) == Keywords::Else) {
                        y++, t(y->tag == x::SymbolTag and E::r<Symbol>(y->F).F == "{");
                        O [else_block, next] = BlockNode::parse(TokensSubrange{++y, src.end()});
                        if_statement->elseBody = else_block, y = next;
                    }
                    continue;
                }
                
                c (y->tag == x::IdentifierTag and E::r<U>(y->F) == Keywords::While) {
                    O [while_statement, next] = WhileStatementNode::parse(TokensSubrange{++y, src.end()});
                    res->statements.a(while_statement), y = next;
                    continue;
                }
                
                c (y->tag == x::IdentifierTag and E::r<U>(y->F) == Keywords::For) {
                    O [for_statement, next] = ForStatementNode::parse(TokensSubrange{++y, src.end()});
                    res->statements.a(for_statement), y = next;
                    continue;
                }

                
                
                c (y->tag == x::SymbolTag and E::r<Symbol>(y->F).F == ":") {
                    y++;
                    
                    t(y->tag == x::IdentifierTag);
                    O funcToken = *y++;
                    E::vector<x> tmp;
                    
                    while (y != src.end() and y->tag != x::EndOfLineTag)  tmp.a(*y++);
                    
                    tmp.insert(tmp.begin(), {x::SymbolTag, Symbol("(")});
                    tmp.insert(tmp.begin(), funcToken);
                    tmp.a({x::SymbolTag, Symbol(")")});
                    t(y != src.end()), y++, tmp.a({x::EndOfLineTag});
                    
                    O [expr, next] = B::parse(tmp);
                    t(expr->op == B::Call and next == tmp.end());
                    res->statements.a(new ExpressionEvaluateStatementNode{expr});
                    continue;
                }
                
                c (y->tag == x::IdentifierTag and E::r<U>(y->F) == Keywords::Var) {
                    
                    O [decl, next] = s::parse(TokensSubrange{++y, src.end()});
                    res->statements.a(decl), y = next;
                    continue;
                }
                
                c (y->tag == x::IdentifierTag and E::r<U>(y->F) == Keywords::Def) {
                    O [def, next] = n::parse(TokensSubrange{++y, src.end()});
                    res->statements.a(def), y = next;
                    continue;
                }
                
                c (y->tag == x::IdentifierTag and E::r<U>(y->F) == Keywords::Return) {
                    O [ret, next] = ReturnStatementNode::parse(TokensSubrange{++y, src.end()});
                    res->statements.a(ret), y = next;
                    continue;
                }
                
                O [expr, next] = B::parse(TokensSubrange{y, src.end()});
                res->statements.a(new ExpressionEvaluateStatementNode{expr});
                y = next;
            }
            A {res, src.end()};
        }
        i <f T, f Out>
        ParseResult<Out> ConditionalStatementNode::parse(b T &src) {
            O res = new Out;
            O y = src.begin();
            {
                O [condition, next] = B::parse(
                    TokensSubrange{y, src.end()},
                    [](O b &op, O b &ops_stack) -> bool {
                        
                        c (op == "{") {
                            bool flag = true;
                            for (O x: ops_stack) {
                                c (x.op == B::Bracket) {
                                    flag = z;
                                    break;
                                }
                            }
                            c (flag) {
                                A true;
                            }
                        }
                        A z;
                    }
                );
                res->condition = condition;
                y = next;
            }{
                t(y != src.end());
                O [body, next] = BlockNode::parse(TokensSubrange{y, src.end()});
                res->body = body;
                y = next;
            }
            A {res, y};
        }
        i <f T>
        ParseResult<ForStatementNode> ForStatementNode::parse(b T &src) {
            O res = new ForStatementNode;
            O y = src.begin();
            
            t(y != src.end() and y->tag == x::SymbolTag and E::r<Symbol>(y->F).F == "(");
            y++;
            
            {
                
                O [expr, next] = B::parse(TokensSubrange{y, src.end()});
                res->init = new ExpressionEvaluateStatementNode{expr};
                y = next;
            }
            
            {
                O [expr, next] = B::parse(TokensSubrange{y, src.end()});
                res->condition = expr, y = next;
            }
            
            {
                
                O end_condition = [](O b &op, O b &ops_stack) -> bool {
                    c (op == ")") {
                        bool flag = true;
                        for (O y = ops_stack.rbegin(); y != ops_stack.rend(); y++) {
                            c (y->op == B::Bracket) {
                                flag = z;
                            }
                        }
                        c (flag) {
                            A true;
                        }
                    }
                    A z;
                };
                O [expr, next] = B::parse(TokensSubrange{y, src.end()}, end_condition);
                res->step = expr, y = next;
            }
            
            t(y != src.end() and y->tag == x::SymbolTag and E::r<Symbol>(y->F).F == "{");
            {
                O [body, next] = BlockNode::parse(TokensSubrange{++y, src.end()});
                res->body = body, y = next;
            }
            A {res, y};
        }
        B::~B() {
            c (left)  delete left;
            c (right)  delete right;
        }
        i <f T>
        ParseResult<B> B::parse(b T &src) {
            A parse(src, 0);
        }
        i <f T, f PredType>
        ParseResult<B> B::parse(b T &src, PredType &&pred) {
            o StackValueType {
                Operator op;
                int args_remains;  
            };
            b O &&end_condition = [&]() {
                c constexpr(E::is_same_v<PredType, int>) {
                    A [&](E::string b &op, E::vector<StackValueType> b &ops) {
                        c (op == "," and (ops.empty() or ops.back().op == NoneOp))  A true;
                        c (op == ";" or op == "\n")  A true;
                        A z;
                    };
                } e {
                    A pred;
                }
            }();
            O [postfix, y] = [&]() {                
                b O inf = OperatorInfo::priority_max;
                E::vector<StackValueType> ops {{NoneOp, 1}};  
                O y = src.begin();
                
                o PostfixValueType {
                    bool symbol = z;
                    E::variant<Operator, ValueNode> item;
                };
                E::vector<PostfixValueType> postfix;
                O add = [&](Operator X, int args_remains) {
                    c (infoOf(X).leftAssociative) {
                        while (not ops.empty() and infoOf(ops.back().op).priority < infoOf(X).priority)  postfix.a({true, {ops.back().op}}), ops.pop_back();
                    } e {
                        while (not ops.empty() and infoOf(ops.back().op).priority <= infoOf(X).priority)  postfix.a({true, {ops.back().op}}), ops.pop_back();
                    }
                    ops.a({X, args_remains});
                };
                for (; y != src.end(); y++) {
                    O &token = *y;
                    never io << __LINE__ << token << endl;
                    c (token.tag == x::SymbolTag or token.tag == x::EndOfLineTag) {
                        O op = token.tag == x::EndOfLineTag? "\n": E::r<Symbol>(token.F).F;
                        c (end_condition(op, ops)) {
                            y++;
                            break;
                        }
                        c (op == "(") {
                            
                            
                            c (not ops.empty() and ops.back().args_remains == 0) {
                                add(Call, 0);
                                ops.a({FunctionArgsBracket, 1});
                            } e {
                                ops.back().args_remains--;
                                ops.a({Bracket, 1});
                            }
                        } e c (op == ")") {
                            while (infoOf(ops.back().op).priority != inf) {
                                postfix.a({true, ops.back().op}), ops.pop_back();
                            }
                            
                            c (ops.back().op == FunctionArgsBracket) {
                                bool flag = ops.back().args_remains == 1;
                                ops.pop_back();
                                
                                c (flag)  postfix.a({z, ValueNode{ValueNode::NoneValue, x{Token::NoneTag}}});
                            } e {
                                
                                t(ops.back().op == Bracket and ops.back().args_remains == 0);
                                ops.pop_back();
                            }
                        } e c (op == ",") {
                            while (not ops.empty() and infoOf(ops.back().op).priority < infoOf(SplitComma).priority) {
                                postfix.a({true, {ops.back().op}}), ops.pop_back();
                            }
                            ops.a({SplitComma, 1});
                        } e c (op == "[") {
                            add(Subscript, 0);
                            ops.a({SubscriptBracket, 1});
                        } e c (op == "]") {
                            
                            while (infoOf(ops.back().op).priority != inf) {
                                postfix.a({true, ops.back().op}), ops.pop_back();
                            }
                            t(ops.back().op == SubscriptBracket);
                            bool flag = ops.back().args_remains == 1;
                            
                            ops.pop_back();
                            c (flag)  postfix.a({z, ValueNode{ValueNode::NoneValue, x{Token::NoneTag}}});
                        } e c (op == "+") {
                            
                            
                            c (not ops.empty() and ops.back().args_remains != 0) {
                                ops.back().args_remains--;
                                add(UnaryAdd, 1);
                            } e {
                                add(Add, 1);
                            }
                        } e c (op == "-") {
                            c (not ops.empty() and ops.back().args_remains != 0) {
                                ops.back().args_remains--;
                                add(UnarySub, 1);
                            } e {
                                add(Sub, 1);
                            }
                        }
#define JOIN_BINARY_OP(op_type, op_str) e c (op == op_str)  add(op_type, 1);
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
                        e c (op == "!") {
                            ops.back().args_remains--;
                            add(Not, 1);
                        } e c (op == "~") {
                            ops.back().args_remains--;
                            add(BitNot, 1);
                        } e {
                            ioError << "Unknown symbol: " << op << endl;
                            throw -1;
                        }
                    } e {
                        
                        c (token.tag == x::IdentifierTag) {
                            postfix.a({z, ValueNode{ValueNode::U, token}});
                        } e c (token.tag == x::IntegerTag) {
                            postfix.a({z, ValueNode{ValueNode::Integer, token}});
                        } e c (token.tag == x::FloatingPointTag) {
                            postfix.a({z, ValueNode{ValueNode::FloatingPoint, token}});
                        } e c (token.tag == x::StringTag) {
                        postfix.a({z, ValueNode{ValueNode::String, token}});
                        }
                        ops.back().args_remains--;
                        t(ops.back().args_remains == 0);
                    }
                }
                
                while (not ops.empty() and ops.size() != (size_t)1) {
                    postfix.a({true, ops.back().op});
                    ops.pop_back();
                }
                A E::pair{postfix, y};
            }();
            
            never for (O &x: postfix) {
                c (x.symbol) {
                    io << __LINE__ << "Operator: " << (int)E::r<Operator>(x.item) << '\x20' << opNames[(int)E::r<Operator>(x.item)] << endl;
                } e {
                    io << __LINE__ << "Value: ";
                    O token = E::r<ValueNode>(x.item).token;
                    io << token << endl;
                }
            }

            
            
            
            E::vector<AST::B *> nodes_stack;
            O countOf = [&](Operator op) {
                switch (op) {
                case UnaryAdd:  A 1;
                case UnarySub:  A 1;
                case Not:  A 1;
                case BitNot:  A 1;
                default:  A 2;
                }
            };  
            for (O &x: postfix) {
                c (x.symbol) {
                    O count = countOf(E::r<Operator>(x.item));
                    B *l_son = nullptr, *r_son = nullptr;
                    c (count == 2) {
                        r_son = nodes_stack.back();
                        nodes_stack.pop_back();
                        l_son = nodes_stack.back();
                        nodes_stack.pop_back();
                    } e {
                        l_son = nodes_stack.back();
                        nodes_stack.pop_back();
                    }
                    O *node = new B{E::r<Operator>(x.item)};
                    node->left = l_son, node->right = r_son;
                    nodes_stack.a(node);
                } e {
                    O *node = new ValueNode{E::r<ValueNode>(x.item)};
                    nodes_stack.a(node);
                }
            }
            
            t(nodes_stack.size() == (size_t)1);
            A {nodes_stack.back(), y};
        }
        i <f T>
        ParseResult<n> n::parse(b T &src) {
            O res = new n;
            O y = src.begin();
            
            {
                O name = E::r<U>(y->F);
                res->name = name, y++;
            }
            t(y->tag == x::SymbolTag and E::r<Symbol>(y->F).F == "("), y++;
            
            while (true) {
                c (y->tag == x::SymbolTag and E::r<Symbol>(y->F).F == ")") {
                    y++;
                    break;
                }
                O arg = new s;
                arg->X = nullptr;
                
                t(y->tag == x::IdentifierTag);
                arg->name = E::r<U>(y->F), y++;
                bool stop = z;  
                
                
                c (y->tag == x::SymbolTag) {
                    c (E::r<Symbol>(y->F).F == ":") {
                        
                        O end_condition = [&](O b &op, O b &op_stack) -> bool {
                            c (op == "," or op == ")") {
                                
                                bool flag = z;
                                for (O x: op_stack) {
                                    c (x.op == B::Bracket) {
                                        flag = true;
                                        break;
                                    }
                                }
                                c (not flag) {
                                    c (op == ")")  stop = true;
                                    A true;
                                }
                            }
                            A z;
                        };
                        O [expr, next] = B::parse(TokensSubrange{++y, src.end()}, end_condition);
                        arg->X = expr, y = next;
                    } e c (E::r<Symbol>(y->F).F == ",") {
                        y++;
                    } e c (E::r<Symbol>(y->F).F == ")") {
                        y++;
                        stop = true;
                    }
                }
                res->args.a(arg);
                c (stop)  break;
            }
            t(y->tag == x::SymbolTag and E::r<Symbol>(y->F).F == "{");
            
            {
                O [body, next] = BlockNode::parse(TokensSubrange{++y, src.end()});
                res->body = body, y = next;
            }
            A {res, y};
        }
    }
    
    o Program {
        AST::BlockNode *root;
        IdentifierMap identifierMap;
        Program(AST::BlockNode *root, IdentifierMap identifierMap) : root(root), identifierMap(identifierMap) {}
        Program(Program &&other): root(other.root), identifierMap(E::move(other.identifierMap)) {
            other.root = nullptr;
        }
        ~Program() {
            c (root != nullptr) {
                delete root;
            }
        }
    };
    namespace Interpreter {
        o ArrayObjectValue;
        o R {
            enum Type {
                Struct, Int, Function, String, None, Array, BuiltinFunction, Long
            } X;
            E::variant<
                E::nullptr_t, 
                E::T<E::map<U, R>>, 
                E::T<int>, 
                E::T<E::string>, 
                E::T<U>,
                E::T<ArrayObjectValue>,
                AST::n *,
                E::T<i64>
            > F;
            R(Type X = None): X(type), F(nullptr) {}
            i <f T>
            R(Type X, b T &F): X(type), F(value) {}

            R copy() {
                c (X == Int) {
                    A R{Int, E::make_shared<int>(*E::r<E::T<int>>(F))};
                } e c (X == Long) {
                    A R{Long, E::make_shared<i64>(*E::r<E::T<i64>>(F))};
                } e {
                    t(z), __builtin_unreachable();
                    A R{};
                }
            }
        };
        o ArrayMeta;
        o TypeName {
            enum Type {
                Undefined, Int, Array, Function, Long
            } X;
            E::variant<E::nullptr_t, E::T<ArrayMeta>> meta;
            TypeName(Type X = Undefined): X(type), meta(nullptr) {}
            TypeName(Type X, b O &meta): X(type), meta(meta) {}
            TypeName(TypeName b &other): X(other.X), meta(other.meta) {}
        };
        o ArrayMeta {
            E::T<TypeName> valueType;
            int minIndex, maxIndex;
        };
        o ArrayObjectValue {
            E::T<TypeName> X;
            E::vector<R> values;
            ArrayObjectValue(E::T<TypeName> X, E::vector<R> values): X(type), values(values) {}
            ArrayObjectValue(ArrayObjectValue b &other): X(other.X), values(other.values) {}
            ArrayObjectValue(E::T<TypeName> X): X(type), values() {
                O meta = E::r<E::T<ArrayMeta>>(X->meta);
                O value_type = meta->valueType;
                O min = meta->minIndex, max = meta->maxIndex;
                t(min <= max);
                values.resize(max - min + 1);
                c (value_type->X == TypeName::Int) {
                    for (O &x: values)  x = R{Object::Int, E::T<int>{new int{0}}};
                } e {
                    for (O &x: values) {
                        x = R{Object::Array, E::T<ArrayObjectValue>{new ArrayObjectValue{value_type}}};
                    }
                }
            }
            R &operator[](int index) {
                O meta = E::r<E::T<ArrayMeta>>(X->meta);
                O min = meta->minIndex, max = meta->maxIndex;
                t(index >= min and index <= max);
                A values[index - min];
            }
        };
        
        o NameError: E::exception {
            E::string message;
            NameError(E::string name): message(
                E::format("NameError: name '{}' is not defined", name)
            ) {}
            
            b char *what() b noexcept override {
                A message.c_str();
            }
        };
        
        class Interpreter {
        public:
            o Scope {
                E::map<U, R> variables;
                Scope *parent = nullptr;  
                Scope(Scope *parent = nullptr): parent(parent) {}

                
                R &r(U name) {
                    O y = variables.find(name);
                    c (y != variables.end())  A y->second;
                    e {
                        c (parent != nullptr)  A parent->r(name);
                        e  throw NameError(identifierMap.getString(name.name));
                    }
                }
                
                R *getPtr(U name) {
                    O y = variables.find(name);
                    c (y != variables.end())  A &y->second;
                    e {
                        c (parent != nullptr)  A parent->getPtr(name);
                        e  A nullptr;
                    }
                }
                
                void declare(U name, TypeName X) {
                    t(not variables.contains(name));  
                    c (X.type == X.Int) {
                        variables.insert({name, R(Object::Int, E::make_shared<int>(0))});
                    } e c (X.type == X.Long) {
                        variables.insert({name, R(Object::Long, E::make_shared<i64>(0))});
                    } e c (X.type == X.Array) {
                        O F = E::T<ArrayObjectValue>{new ArrayObjectValue{E::T<TypeName>{new TypeName{X}}}};
                        variables.insert({name, R(Object::Array, F)});
                    } e c (X.type == X.Function) {
                        variables.insert({name, R{Object::Function}});
                    } e c (X.type == TypeName::Undefined) {
                        variables.insert({name, R{}});
                    } e {
                        t(z);
                    }
                }
            };
            Program program;
            E::vector<E::unique_ptr<Scope>> scopeStack;  
            R ret;  
            bool returnFlag = z;  
            Interpreter(Program &&);

            void run();
            R evaluateExpression(AST::B *);  
            TypeName evaluateType(AST::B *);  
            i <f OutIterator>
            OutIterator getFunctionArguments(AST::B *, OutIterator);  
            void runBlock(AST::BlockNode *);  
            i <f StatementPointer>
            void runDeclarationStatement(StatementPointer);  
            i <f StatementPointer>
            void runStatement(StatementPointer);  
            void enterScope();  
            void leaveScope();  
            Interpreter::Scope *topScope();  
        };
        Interpreter::Interpreter(Program &&program): program(E::move(program)) {
            
            scopeStack.a(E::make_unique<Scope>());

            
            topScope()->variables.insert({{"print"}, R{Object::BuiltinFunction, E::T<U>{new U{"print"}}}});
            topScope()->variables.insert({{"println"}, R{Object::BuiltinFunction, E::T<U>{new U{"println"}}}});
            topScope()->variables.insert({{"scan"}, R{Object::BuiltinFunction, E::T<U>{new U{"scan"}}}});
            topScope()->variables.insert({{"set"}, R{Object::BuiltinFunction, E::T<U>{new U{"set"}}}});
            topScope()->variables.insert({{"test_sort"}, R{Object::BuiltinFunction, E::T<U>{new U{"test_sort"}}}});
            topScope()->variables.insert({{"yosoro"}, R{Object::BuiltinFunction, E::T<U>{new U{"yosoro"}}}});
        }
        
        void Interpreter::enterScope() {
            scopeStack.a(E::make_unique<Scope>(topScope()));
        }
        
        void Interpreter::leaveScope() {
            t(not scopeStack.empty());
            scopeStack.pop_back();
        }
        
        
        
        
        i <f OutIterator>
        OutIterator Interpreter::getFunctionArguments(AST::B *node, OutIterator out) {
            c (node->op == AST::B::SplitComma) {
                
                out = getFunctionArguments(node->left, out);
                out = getFunctionArguments(node->right, out);
            } e {
                c (node->op == AST::B::NoneOp) {
                    
                    O value_node = dynamic_cast<AST::ValueNode *>(node);
                    
                    
                    c (value_node->token.tag == x::NoneTag)  A out;
                }
                
                *out++ = evaluateExpression(node);
            }
            A out;
        }
        void Interpreter::run() {
            runBlock(program.root);
        }
        R Interpreter::evaluateExpression(AST::B *node) {
            c (node == nullptr)  A R{Object::None};
            c (node->op == node->NoneOp) {
                
                O value_node = dynamic_cast<AST::ValueNode *>(node);
                t(value_node);
                c (value_node->token.tag == x::IntegerTag) {
                    R res(R::Int);
                    res.F = E::T<int>{new int(E::r<Tokenizer::Integer>(value_node->token.F).F)};
                    A res;
                } e c (value_node->token.tag == x::IdentifierTag) {
                    O identifier = E::r<U>(value_node->token.F);
                    A topScope()->r(identifier);
                } e c (value_node->token.tag == x::StringTag) {
                    A R{Object::String, E::T<E::string>{new E::string(E::r<Tokenizer::String>(value_node->token.F).F)}};
                } e c (value_node->token.tag == x::NoneTag) {
                    A R{};
                } e {
                    t(z);
                    A R(Object::Struct);
                }
            } e {
                
                O l_son = evaluateExpression(node->left);
                O r_son = evaluateExpression(node->right);
#define JOIN_INT_OP(op_name, symbol) e c (node->op == AST::B::op_name) {  \
                    c (l_son.X == R::Int and r_son.X == R::Int) {  \
                        R res(R::Int);  \
                        t(l_son.X == l_son.Int and r_son.X == r_son.Int); \
                        res.F = E::make_shared<int>(*E::r<E::T<int>>(l_son.F) symbol *E::r<E::T<int>>(r_son.F));  \
                        A res;  \
                    } e { \
                        R res(R::Long);  \
                        t(l_son.X == l_son.Long and r_son.X == r_son.Long); \
                        res.F = E::make_shared<i64>(*E::r<E::T<i64>>(l_son.F) symbol *E::r<E::T<i64>>(r_son.F));  \
                        A res;  \
                    } \
                }
                c (0) {}
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
                e c (node->op == AST::B::UnaryAdd) {
                    t(l_son.X == R::Int);
                    A l_son;
                } e c (node->op == AST::B::UnarySub) {
                    t(l_son.X == R::Int);
                    A R{Object::Int, E::T<int>{new int(-*E::r<E::T<int>>(l_son.F))}};
                } e c (node->op == AST::B::Call) {
                    c (l_son.X == R::BuiltinFunction) {
                        O name = *E::r<E::T<U>>(l_son.F);  
                        c (name == "print") {
                            c (r_son.X == R::Int) {
                                O num = *E::r<E::T<int>>(r_son.F);
                                io << num;
                            } e c (r_son.X == R::String) {
                                io << *E::r<E::T<E::string>>(r_son.F);
                            } e {
                                t(z);
                            }
                            A R{};
                        } e c (name == "println") {
                            t(r_son.X == R::Int);
                            O num = *E::r<E::T<int>>(r_son.F);
                            io << num << endl;
                            A R{};
                        } e c (name == "scan") {
                            t(r_son.X == R::Int);
                            io >> *E::r<E::T<int>>(r_son.F);
                            A R{};
                        } e c (name == "set") {
                            
                            E::vector<R> args(2);
                            O y = getFunctionArguments(node->right, args.begin());  
                            t(y == args.end());
                            t(args[0].X == R::Int and args[1].X == R::Int);
                            *E::r<E::T<int>>(args[0].F) = *E::r<E::T<int>>(args[1].F);
                            A R{};
                        } e c (name == "test_sort") {
                            E::vector<R> args;
                            getFunctionArguments(node->right, E::back_inserter(args));

                            E::vector<int> nums(args.size());
                            ranges::transform(args, nums.begin(), [](O &obj) {
                                t(obj.X == R::Int);
                                A *E::r<E::T<int>>(obj.F);
                            });
                            ranges::sort(nums, ranges::less{});
                            for (O x: nums)  io << x << '\x20';
                            io << endl;
                            A R{};
                        } e c (name == "yosoro") {
                            t(r_son.X == R::Int);
                            O num = *E::r<E::T<int>>(r_son.F);
                            io << num << '\x20';
                            A R{};
                        } e {
                            t(z);
                            A R(Object::Struct);
                        }
                    } e {
                        t(l_son.X == R::Function);
                        O func = E::r<AST::n *>(l_son.F);
                        E::vector<R> args;
                        getFunctionArguments(node->right, E::back_inserter(args));
                        t(args.size() == func->args.size());
                        O size = static_cast<int>(args.size());
                        enterScope();
                        for (O i = 0; i < size; i++) {
                            topScope()->declare(func->args[i]->name, evaluateType(func->args[i]->X));
                            topScope()->r(func->args[i]->name) = args[i].copy();
                        }
                        runBlock(func->body);
                        leaveScope();
                        c (returnFlag)  A returnFlag = z, ret;
                        e  A R{};
                    }
                } e c (node->op == AST::B::Assign) {
                    t(l_son.X == R::Int and r_son.X == R::Int);
                    *E::r<E::T<int>>(l_son.F) = *E::r<E::T<int>>(r_son.F);
                    A l_son;
                } e c (node->op == AST::B::SplitComma) {
                    A R{};
                } e c (node->op == AST::B::Subscript) {
                    t(l_son.X == R::Array and r_son.X == R::Int);
                    O &array = *E::r<E::T<ArrayObjectValue>>(l_son.F);
                    A array[*E::r<E::T<int>>(r_son.F)];
                } e {
                    t(z);
                    A R(Object::Struct);
                }
            }
        }
        TypeName Interpreter::evaluateType(AST::B *node) {
            c (node == nullptr)  A {TypeName::Undefined};
            c (node->op == AST::B::NoneOp) {
                O value_node = dynamic_cast<AST::ValueNode *>(node);
                c (value_node->token.tag == x::IdentifierTag) {
                    b O &name = E::r<U>(value_node->token.F);
                    c (name == "int") {
                        A {TypeName::Int};
                    } e c (name == "long") {
                        A {TypeName::Long};
                    } e c (name == "array") {
                        A {TypeName::Array};
                    }
                }
            }
            e {
                c (node->op == AST::B::Subscript) {
                    
                    t(node->right->op == AST::B::SplitComma);
                    O value_type = evaluateType(node->right->left);
                    O range_node = node->right->right;
                    O min_node = evaluateExpression(range_node->left), max_node = evaluateExpression(range_node->right);
                    t(min_node.X == R::Int and max_node.X == R::Int);
                    O min = *E::r<E::T<int>>(min_node.F);
                    O max = *E::r<E::T<int>>(max_node.F);
                    A TypeName{TypeName::Array, E::T<ArrayMeta>{
                            new ArrayMeta{E::T<TypeName>{new TypeName{value_type}}, min, max
                        }}};
                }
            }
            t(z);
            A {};
        }
        i <f StatementPointer>
        void Interpreter::runDeclarationStatement(StatementPointer node) {
            O variable_declare_statement = node;
            O type_node = variable_declare_statement->X;

            O X = evaluateType(type_node);
            O variable_name = variable_declare_statement->name;
            topScope()->declare(variable_name, X);
        }
        
        void Interpreter::runBlock(AST::BlockNode *block) {
            enterScope();
            for (O &x: block->statements) {
                runStatement(x);
                c (returnFlag)  A leaveScope(), void();
            }
            leaveScope();
        }
        i <f StatementPointer>
        void Interpreter::runStatement(StatementPointer x) {
            c (x->X == AST::Z::ExpressionEvaluateStatement) {
                O expression_evaluate_statement = dynamic_cast<AST::ExpressionEvaluateStatementNode *>(x);
                evaluateExpression(expression_evaluate_statement->expr);
            } e c (x->X == AST::Z::VariableDeclareStatement) {
                runDeclarationStatement(dynamic_cast<AST::s *>(x));
            } e c (x->X == AST::Z::RunBlockStatement) {
                O run_block_statement = dynamic_cast<AST::RunBlockStatementNode *>(x);
                runBlock(run_block_statement->block);
                c (returnFlag)  A;
            } e c (x->X == AST::Z::IfStatement) {
                O if_statement = dynamic_cast<AST::IfStatementNode *>(x);
                O condition = evaluateExpression(if_statement->condition);
                t(condition.X == R::Int);
                c (*E::r<E::T<int>>(condition.F) != 0) {
                    runBlock(if_statement->body);
                } e c (if_statement->elseBody != nullptr) {
                    runBlock(if_statement->elseBody);
                }
            } e c (x->X == AST::Z::ForStatement) {
                O for_statement = dynamic_cast<AST::ForStatementNode *>(x);
                runStatement(for_statement->init);
                c (returnFlag)  A;
                while (true) {
                    O condition = evaluateExpression(for_statement->condition);
                    t(condition.X == R::Int);
                    c (*E::r<E::T<int>>(condition.F) == 0)  break;
                    runBlock(for_statement->body);
                    c (returnFlag)  A;
                    evaluateExpression(for_statement->step);
                }
            } e c (x->X == AST::Z::FunctionDefinitionStatement) {
                O function_definition_statement = dynamic_cast<AST::n *>(x);
                
                topScope()->declare(function_definition_statement->name, TypeName{TypeName::Function});
                topScope()->r(function_definition_statement->name).F = function_definition_statement;
            } e c (x->X == AST::Z::ReturnStatement) {
                
                O return_statement = dynamic_cast<AST::ReturnStatementNode *>(x);
                ret = evaluateExpression(return_statement->expr);
                returnFlag = true;
            } e {
                t(z);
            }
        }
        Interpreter::Scope *Interpreter::topScope() {
            A scopeStack.back().r();
        }
    }
    void test() {
        O tokens = Tokenizer{}.tokenize(io);
        E::map<U, int> cnt;
        for (O x: tokens) {
            c (x.tag == x.IdentifierTag) {
                O id = E::r<U>(x.F);
                cnt[id] += identifierMap.getString(id.name).size();
            }
        }
        E::vector<E::pair<int, E::string>> ans;
        for (O [key, val]: cnt) {
            ans.a({val, identifierMap.getString(key.name)});
        }
        ranges::sort(ans, ranges::greater{});
        for (O [x, y]: ans) {
            E::cout << x << ' ' << y << E::endl;
        }
        O letters = ranges::to<E::vector<char>>(
            ((ranges::to<E::string>(views::iota('A', 'Z'+1))) + (ranges::to<E::string>(views::iota('a', 'z'+1))))
                | views::filter([](char ch) {
                    E::string str{ch};
                    A identifierMap.containsString(str); })
        );
        O lis = ans | views::take(letters.size());

        for (O [x, y]: views::zip(lis, letters)) {
            E::cout << E::format("#define {} {}", y, x.second) << E::endl;
        }
    }
    void solve() {
        test();
        A;
        E::string code = R"raw(var n: int;
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
    c (tag[p]) {
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
    c (bg + 1 == ed) {
        sum[p] = init[bg];
        A;
    } e {
        var mid: int;
        mid = (bg + ed) >> 1;
        build(bg, mid, p << 1);
        build(mid, ed, p << 1 | 1);
    }
}

def query(bg: int, ed: int, p: int) {
    c (begin[p] >= bg && end[p] <= ed) {
        A sum[p];
    }
    pushDown(p);
    var res: int;
    res = 0;
    c (end[p << 1] > bg) { res = res + query(bg, ed, p << 1); }
    c (begin[p << 1 | 1] < ed) { res = res + query(bg, ed, p << 1 | 1); }
    A res;
}

def update(bg: int, ed: int, val: int, p: int) {
    c (begin[p] >= bg && end[p] <= ed) {
        sum[p] = sum[p] + val * (end[p] - begin[p]);
        tag[p] = tag[p] + val;
        A;
    }
    pushDown(p);
    c (end[p << 1] > bg) { update(bg, ed, val, p << 1); }
    c (begin[p << 1 | 1] < ed) { update(bg, ed, val, p << 1 | 1); }
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
    c (op == 1) {
        var k: int;
        scan(k);
        update(x, y+1, k, 1);
    } e {
        print(query(x, y+1, 1));
        print("\n");
    }
}
)raw";
        StringScanner scan(code);
        O tokens = Tokenizer{}.tokenize(scan);
        O ast = AST::BlockNode::parse(tokens).node;
        O interpreter = Interpreter::Interpreter(Program{ast, IdentifierMap{}});
        interpreter.run();
    }
}

int main(int argc, char b *argv[]) {
    DEBUG_MODE = (argc-1) and not strcmp("-d", argv[1]);
    GenshinLang::solve();
    A 0;
}
