/**
 * @link https://www.luogu.com.cn/problem/P4911
 */

#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=(n);i++)
#define from(i,b,e) for(int i=(b);i<=(e);i++)
#define rev(i,e,b) for(int i=(e);i>=(b);i--)
#define main() main(int argc, char const *argv[])
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
#define __macro_arg_counter(_1,_2,_3,_4,_5, N, ...) N
#define macro_arg_counter(...)  __macro_arg_counter(__VA_ARGS__,5,4,3,2,1,0)
#define __macro_choose_helper(M,count)  M##count
#define macro_choose_helper(M,count)   __macro_choose_helper(M,count)
#define __lambda_1(expr) [&](){return expr;}
#define __lambda_2(a, expr) [&](auto a){return expr;}
#define __lambda_3(a, b, expr) [&](auto a, auto b){return expr;}
#define __lambda_4(a, b, c, expr) [&](auto a, auto b, auto c){return expr;}
#define lambda(args...) macro_choose_helper(__lambda_, macro_arg_counter(args))(args)
#define lam lambda
namespace lib{}
#include <bits/stdc++.h>
#define USE_FREAD
// #undef USE_FREAD
// 取消注释上一行会使用 getchar() 替代 fread，可以不使用 EOF 结束读入，但是降低性能 
#include <bits/stdc++.h>
#define USE_FREAD
// #undef USE_FREAD
// 取消注释上一行会使用 getchar() 替代 fread，可以不使用 EOF 结束读入，但是降低性能 
namespace lib{
    using std::string;
    struct __IO {
        virtual inline char gc() = 0;
        inline bool blank(char ch) { return ch==' ' or ch=='\n' or ch=='\r' or ch=='\t'; }
        inline bool isd(char x) {return (x>='0' and x<='9');}
        string input(int reserve=0) {
            char c = gc(); string res=""; res.reserve(reserve);
            while((c&&!blank(c)) || c==' ') {  res.push_back(c); c = gc(); }
            return res;
        }
        template <class T>
        inline void read(T &x) {
            double tmp=1; bool sign=0; x=0; char ch=gc();
            for (; not isd(ch); ch=gc()) if (ch=='-') sign=1;
            for (; isd(ch); ch=gc()) x=x*10+(ch^48);
            if (ch=='.') for (ch=gc(); isd(ch); ch=gc()) tmp*=.1f,x+=tmp*(ch^48);
            if (sign) x=-x;
        }
        inline void read(char *s) {
            char ch=gc();
            for (; blank(ch); ch=gc());
            for (; not blank(ch); ch=gc()) *s++=ch;
            *s=0;
        }
        inline void readln(char *s) {
            char c = gc(); while((c&&!blank(c)) || c==' ') {  *(s++)=c; c = gc();  } *s=0;
        }
        inline void readln(string &res, int reserve=0) {
            char c = gc(); string().swap(res); res.reserve(reserve);
            while((c&&!blank(c)) || c==' ') {  res.push_back(c); c = gc(); }
        }
        inline void read(char &c) {  for (c=gc(); blank(c); c=gc());  }
        inline void read(string &s){
            string().swap(s); char ch=gc();
            for (; blank(ch); ch=gc());
            for (; not blank(ch); ch=gc()) s.push_back(ch);
        }
        template <class T,class... Types> inline void read(T &x,Types &...args){  read(x); read(args...);  }
        template <class T> inline void scan(const T &x) { read(*x); }
        template <class T,class ...Types> inline void scan(const T &x,const Types &...args) {  read(*x); scan(args...);  }
        virtual inline void push(const char c) = 0;
        
        inline void write(const char c) {  push(c);  }
        inline void write(const string &s){  for (auto i:s)  push(i);  }
        inline void write(const char *s){  for (; *s; ++s) push(*s);  }
        template <class T
#if __cplusplus > 201403L
        , class = typename std::enable_if_t<std::is_integral_v<T>>
#endif
        >
        inline void write(T x) {
            if (x<0) x=-x,push('-');
            static char sta[40]; int top=0;
            do {  sta[top++]=x%10^48,x/=10;  } while (x);
            while (top) push(sta[--top]);
        }
        template <class T,class... Types>  inline void write(const T &x,const Types &...args){ write(x); write(' '); write(args...); }
        template <class... Types> inline void writeln(const Types &...args){  write(args...); write('\n');  }
        template<class T=int> inline T get() {  T x; read(x); return x;  }
        // 流式输入输出
        template <class T> inline auto& operator>>(T&x) {  read(x); return *this; }
        template <class T> inline auto& operator<<(const T&x) {  write(x); return *this; }
    };

    template <const long long MAXSIZE, const long long MAX_ITEM_SZ=500>
    struct IO: __IO {
#ifdef USE_FREAD
        char buf[MAXSIZE],*p1,*p2;
        char pbuf[MAXSIZE],*pp;
        IO():p1(buf),p2(buf),pp(pbuf) {}
        ~IO() {  fwrite(pbuf,1,pp-pbuf,stdout);  }
        inline char gc() {
            if (p1==p2) p2=(p1=buf)+fread(buf,1,MAXSIZE,stdin);
            return p1==p2?' ':*p1++;
        }
        inline void sync() { fwrite(pbuf,1,MAXSIZE,stdout); pp=pbuf; }
        inline void push(const char c) {
            if (pp-pbuf==MAXSIZE) sync();
            *pp++=c;
        }
#else
        inline void gc() { return getchar(); }
        inline void push(const char c) { putchar(c); }
#endif
    };

    struct SIO: __IO {
        const string& src;
        size_t index = 0;

        SIO(const string& src): src(src) { index=0; }
        inline char gc() {
            return index==src.size()? ' ': src[index++];
        }
        inline void push(const char c) { putchar(c); }
    };

    struct SIO_w: __IO {
        string& src;
        size_t index = 0;

        SIO_w(string& src): src(src) { index=0; }
        inline char gc() {
            return index==src.size()? ' ': src[index++];
        }
        inline void push(const char c) { src.push_back(c); }
    };

    IO<1<<20> io;
    const char endl[] = "\n";

}

using namespace lib;
using std::string, std::vector;


namespace Utilities {
    /**
     * “提取”一个容器中的前 SZ 项，其中 SZ 为编译期常量。
     * 提取后的项可以用于结构化绑定。
     * ! 修改提取后的项不会影响到原元素。
     */
    namespace Extractions {
        template <class T, size_t SZ>  requires (SZ>0)
        struct _Extraction {  T container;  };

        template <size_t N, class Ext>
        auto get(const Ext& c) {
            return c.container[N];
        }

        /**
         * 提取容器 con 的前 SZ 项，其中 SZ 为模板参数。
         * 用法示例：
         * std::vector<int> vec = {0, 1, 2, 3, 4};
         * auto [a, b, c] = extract<3>(vec);
         * -> a = 0, b = 1, c = 2
         * 在这样操作后，对这些变量的修改**不会**同步到原容器，反之亦然
         */
        template <size_t SZ, class Container_T>
        auto extract(Container_T& con) {
            assert(con.size());
            return _Extraction<Container_T, SZ> {con};
        }
    }

    string& to_upper(string& src) {
        for (auto& c: src)  c = std::toupper(c);
        return src;
    }
}

using Utilities::Extractions::get, Utilities::to_upper;

namespace std {
    // 定义接口，用于实现 Utilities::Extractions::_Extraction 的结构化绑定。
    template <class T, size_t SZ>
    struct tuple_size<Utilities::Extractions::_Extraction<T, SZ>> { const static int value = SZ; };
    template <size_t N, class T, size_t SZ>
    struct tuple_element<N, Utilities::Extractions::_Extraction<T, SZ>> {
        using type = decltype(std::move(std::declval<T>()[0]));
    };
}

namespace Processor {
    struct Arguments;
    using operation_t = std::function<void(Arguments)>;
    using operation_no_arg_t = std::function<void()>;
    std::unordered_map<string, operation_t> OperationsMapping;
    std::unordered_map<string, operation_no_arg_t> OperationsMapping0;
}

/**
 * 作为一个解释后的“程序”和用户交互
 */
namespace Program {

    enum ProgramStatus { OK, Stopped, CompileError, RuntimeError };

    enum ExceptionType {  // 异常类型
        NoError,                // 保留
        IndexError,             // 越界访问
        ZeroDivisionError,      // 除以零
        ModifyConstantError,    // 修改常量
    };

    struct Exception {  // 运行时异常
        ExceptionType type;     // 异常类型
        string description;     // 描述
        string information;     // 详细信息

        Exception(ExceptionType type, string desc, string info=""): 
            type(type), description(desc), information(info) {}
    };


    struct Operation {
        int line;
        string label;
        Processor::Arguments *args;

        void execute();
    };

    struct Parsed {
        vector<Operation> operations;
        std::unordered_map<string, int> functionToId;
        std::unordered_map<int, string> IdToFunction;
        std::unordered_map<int, int> FunctionIdToDefintionLineIndex;
        std::unordered_map<int, Operation> lineIndexMapping;
    };

    
    struct Program {
        ProgramStatus status;   // 运行状态
        vector<Operation>::iterator step;  // 当前的位置

        Parsed code;
        
        void raise(const Exception& exception) {  // 抛出一个运行时异常
            status = RuntimeError;  // 停止运行，输出异常信息
            io << std::format("Runtime Error while running on line {}.\n {}: {}\n{}\n", 
                step->line, (int)exception.type, exception.description, exception.information);
        }
        
        void halt() {  // 计划中的停机
            status = Stopped;
        }

        void launch() {  // 开始运行
            step = code.operations.begin();
            step->execute();
            while (step != code.operations.end() and status == OK) {
                step->execute();
                step++;
            }
        }
    } program;  // 当前在运行的程序
}
using namespace Program;

/**
 * 存储
 * 在本题中包含寄存器和内存
 */
namespace Storage {
    const int _R = 12, _Mem = 0x1000000;

    enum RegisterLabel { R1, R2, R3, R4, E1, E2, E3, E4, FLAG, VAL, RET, LINE };
    struct Register { string name; int data; }  registers[_R];
    std::unordered_map<string, Register*> registerMapping;  // 名称到寄存器指针的映射
    int memory[_Mem];  // 内存

    enum ReferenceType { Null/*空引用*/, Constant/*常量*/, InRegister/*寄存器*/, InMemory/*内存*/ };

    /**
     * 数据存储的位置
     */
    struct StorageReference {
        ReferenceType type;             // 引用类型
        int valueConstant = 0;          // 存储的常量值
        int *valuePointer = nullptr;    // 寄存器数据的指针；如果是常量则为空指针
        int MemoryIndex = 0;            // 内存中的地址
        // 特别地，对于常量内存地址，存储在 MemoryIndex 中；
        // 对于寄存器中的内存地址，寄存器指针在 valuePointer 中。

        int operator* () {  // 重载运算符 解引用
            return (int)*this;
        }

        operator int() const {
            if (type == Constant)  return valueConstant;
            if (type == InRegister)  return *valuePointer;
            if (valuePointer) {  // 寄存器地址
                return (int)(StorageReference){InMemory, 0, nullptr, *valuePointer};
            } else {
                if (MemoryIndex < 0 or MemoryIndex >= _Mem) {  // 访问越界
                    program.raise({IndexError, std::format("Illegal memory access to {}.", MemoryIndex)});
                    return valueConstant;  // 防止解释器本身 RE
                }
                return memory[MemoryIndex];
            }
        }

        int operator = (int val) {
            if (type == Constant) {
                program.raise({ModifyConstantError, "Modifying a constant reference."});
                return -1;
            }
            if (type == InRegister) {
                return (*valuePointer = val);
            }
            if (valuePointer) {  // 寄存器地址
                return ((StorageReference){InMemory, 0, nullptr, *valuePointer} = val);
            } else {
                if (MemoryIndex < 0 or MemoryIndex >= _Mem) {  // 访问越界
                    program.raise({IndexError, std::format("Illegal memory access to {}.", MemoryIndex)});
                    return -1;  // 防止解释器本身 RE
                }
                return (memory[MemoryIndex] = val);
            }
        }

        operator bool () const { return (type != Null); }
        // 等价于 Javascript 的 ?? 运算符
        StorageReference operator || (StorageReference const& other) const {
            return *this? *this: other;
        }
    };

    StorageReference constRef(int data) { return {Constant, data}; }
    StorageReference memoryRef(int index) { return {InMemory, 0, nullptr, index}; }
    StorageReference memoryRef(Register& reg) { return {InMemory, 0, &reg.data}; }
    StorageReference memoryRef(RegisterLabel label) { return {InMemory, 0, &registers[label].data}; }
    StorageReference registerRef(Register& reg) { return {InRegister, 0, &reg.data}; }
    StorageReference registerRef(RegisterLabel label) { return {InRegister, 0, &registers[label].data}; }
    StorageReference nullRef() { return {Null, 0}; }

    void init() {
        // 注册所有寄存器
        #define reg(x) registerMapping.insert(std::pair{string(#x), registers+x})
        reg(R1); reg(R2); reg(R3); reg(R4); reg(E1); reg(E2); reg(E3); reg(E4); 
        reg(FLAG); reg(VAL); reg(RET); reg(LINE);
        #undef reg
    }
}
using Storage::StorageReference;

/**
 * 处理器
 * 负责执行解析后的操作
 */
namespace Processor {
    /**
     * 组织参数列表，是一个元素为 StorageReference 的可变容器，继承自 vector。
     * 当使用中括号运算符取值时，会直接返回这个参数的值，使用 at(index) 则会正常返回引用。
     */
    struct Arguments: public vector<StorageReference> {
        Arguments() = default;
        Arguments(std::initializer_list<StorageReference> lis): vector<StorageReference>(lis) {}

        template <size_t N>
        auto take() const {
            return Utilities::Extractions::extract<N>(*this);
        }

        template <class index_t> StorageReference operator[] (index_t N) const {
            return (index_t)size() > N? at(N): Storage::nullRef();
        }
    };

    /**
     * 以下是没有参数的函数
     */
    void Undefined_UDEF() {}
    void NoOperation_NOP() {}

    void Halt_HLT() {
        program.status = Stopped;
    }

    /**
     * 一个示例的操作函数，这会直接输出传入的两个参数
     */
    void Example_EXP(Arguments args) {
        auto [a, b] = args.take<2>();
        io.writeln(*a, *b);
    }

    void Set(Arguments args) {  // 将 b 赋值为 a
        auto [source, target] = args.take<2>();
        io << target.MemoryIndex << *source << endl;
        target = *source;
    }


    void Add(Arguments args) {
        auto [a, b] = args.take<2>();
        auto target = args[2] || Storage::registerRef(Storage::VAL);
        target = *a + *b;
    }

    void ReadInt_RINT(Arguments args) {
        auto target = args[0] || Storage::registerRef(Storage::VAL);
        int x; io >> x;
        target = x;
    }


    void init() {
#define reg(s, fun) OperationsMapping.insert({#s, fun});
#define reg0(s, fun) OperationsMapping0.insert({#s, fun});
        reg0(HLT, Halt_HLT)
        reg(EXAMPLE, Example_EXP)
        reg(SET, Set)
        reg(ADD, Add)
        reg(RINT, ReadInt_RINT)
#undef reg
#undef reg0
    }
}

namespace Parser {
    void CompileError(string description, string information = "") {
        io << std::format("Compile Error: {}\n{}\n", description, information);
    }

    /**
     * 从解释器的标准输入中读入代码，然后解析这段代码。
     * 结果输出到给定的程序中
     */
    void readAndParse(Program::Program& dest) {
        Parsed& res = dest.code;

        int N; io >> N;
        // 去除所有注释
        string code = "\n";  // 对齐行号
        string line;
        int commentDepth = 0;
        int commandCount = 0;  // 通过分号统计的指令数
        from(i, 1, N) {
            io.readln(line);
            for (char c: line) {
                if (c == '[')  commentDepth++;
                if (c == ';') {
                    code += ' ';  // 分号前面追加一个空格，方便解析
                    commandCount++;
                } 
                if (c == ',')  c = ' ';  // 改为空格分割参数
                if (commentDepth == 0)  code += c;
                if (c == ']')  commentDepth--;
            }
            code += '\n';
        }
        

        // 记录每一行的行首字符
        std::vector<int> LineBeginCharIndex = {0};
        SIO sio(code);
        from(l, 0, N) {
            sio.readln(line);
            LineBeginCharIndex.push_back(sio.index);
        }
        sio.index = 0;  // 复原指针
        while (commandCount) {  // 按语句读入
            int line = std::lower_bound(LineBeginCharIndex.begin(), LineBeginCharIndex.end(), 
                            sio.index) - LineBeginCharIndex.begin();  // 行号
            // 读入一个命令
            string label;  sio >> label;
            auto op = Operation(line, to_upper(label), new Processor::Arguments());

            // 接下来，读入若干个参数
            // 假定在分号之前都是参数
            char c; sio >> c;
            while (c != ';') {
                StorageReference arg;
                if (c == '#') {  // 期望读到一个编译期常量，在此题中必为 #Line
                    string temp;  sio >> temp;
                    if (to_upper(temp) == "LINE") {
                        arg = Storage::constRef(line);
                    } else {
                        CompileError(std::format("Unknown constexpr {}.", temp));
                        goto error;
                    }
                } else if (c == '%') {  // 期望读到一个寄存器名称
                    string name; sio >> name;
                    if (Storage::registerMapping.find(to_upper(name)) == Storage::registerMapping.end()) {
                        CompileError(std::format("Unknown register {}.", name));
                        goto error;
                    } else {
                        arg = Storage::registerRef(*Storage::registerMapping[name]);
                    }
                } else if (c == '@') {  // 内存
                    char c2; sio >> c2;
                    if (c2 == '%') {  // 寄存器
                        string name; sio >> name;
                        if (Storage::registerMapping.find(to_upper(name)) == Storage::registerMapping.end()) {
                            CompileError(std::format("Unknown register {}.", name));
                            goto error;
                        } else {
                            arg = Storage::memoryRef(*Storage::registerMapping[name]);
                        }
                    } else {  // 常数地址
                        sio.index--;  // 回退指针
                        int num; sio >> num;
                        arg = Storage::memoryRef(num);
                    }
                } else if (c == '$') {  // 期望读到一个函数标识符
                    string name; sio >> name;
                    int id = 0;
                    if (res.functionToId.find(name) == res.functionToId.end()) {
                        int id = res.functionToId.size();
                        res.functionToId[name] = id;
                        res.IdToFunction[id] = name;
                    } else {
                        id = res.functionToId[name];
                    }

                    if (label == "FUNCTION") {
                        res.FunctionIdToDefintionLineIndex[id] = line;
                    }
                    arg = Storage::constRef(id);
                } else {  // 常数
                    sio.index--;
                    int num; sio >> num;
                    arg = Storage::constRef(num);
                }
                op.args->push_back(arg);
                sio >> c;
            }
            commandCount--;
            if (label != "FUNCTION"){
                res.operations.push_back(op);
            }
        }

        
    
    error:
        return;
    }
}

namespace Program {
    void Operation::execute() {
        if (args->empty()) {
            assert(Processor::OperationsMapping0[label]);
            std::invoke(Processor::OperationsMapping0[label]);
        } else {
            assert(Processor::OperationsMapping[label]);
            std::invoke(Processor::OperationsMapping[label], *args);
        }
    }
}

namespace Solution {

    void init() {
        Storage::init();
        Processor::init();
    }
    
    void test() {
        init();
        // auto a = Storage::registerRef(Storage::VAL);
        // a = 5;
        // Processor::Add({Storage::constRef(114), a});
        // io << *Storage::registerRef(Storage::VAL) << endl;
        // Processor::Set({Storage::constRef(616), Storage::memoryRef(Storage::VAL)});
        // io << *Storage::memoryRef(119) << endl;
        Parser::readAndParse(program);
        log("RUN!!!\n")
        program.launch();
    }


    void solve() {
        test();
        init();
    }
}


int main() {;

    initDebug;
    Solution::solve();
    return 0;
}