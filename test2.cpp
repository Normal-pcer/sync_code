#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

using u8 = unsigned char;
using i16 = short;
using u16 = unsigned short;
using i32 = int;
using u32 = unsigned int;
using u64 = unsigned long long;

struct Reader
{
    char ch, unget;
    Reader(bool)
    {
    }
    Reader()
    {
        unget = '\0';
        ch = getchar();
    }
    char seek()
    {
        return unget ? unget : ch;
    }
    void skip()
    {
        if (unget)
            unget = '\0';
        else
            ch = getchar();
    }
    bool eof()
    {
        return ch == EOF && unget == '\0';
    }
    void read(char &c)
    {
        c = seek();
        skip();
    }
    char read()
    {
        char res;
        return read(res), res;
    }
    void ungetc(char c)
    {
        unget = c;
    }
} reader(false);

const u32 MAX_DATA_SIZE = 16 * 1024;
const u32 ID_HASH_LENGTH = 16 * 1024;
const u32 MAX_FUNCTION = 1024;
char str_data[MAX_DATA_SIZE];
char *data_ptr = str_data;
char *id_hash[ID_HASH_LENGTH];
u32 easy_hash(const char *str)
{
    u32 res = 0;
    while (*str != '\0')
        res = res * 331 + *(str++);
    return res;
}
vector<u32> used_hash_index;
u32 insert_id(const char *str)
{
    u32 hsh = easy_hash(str) & (ID_HASH_LENGTH - 1);
    while (id_hash[hsh] != nullptr && strcmp(id_hash[hsh], str))
        hsh = (hsh + 1) & (ID_HASH_LENGTH - 1);
    if (id_hash[hsh] == nullptr)
    {
        char *strr = new char[strlen(str) + 1];
        strcpy(strr, str);
        id_hash[hsh] = strr;
        used_hash_index.push_back(hsh);
    }
    return hsh;
}
template <typename T> unique_ptr<T> create_unique()
{
    return unique_ptr<T>(new T);
}
template <typename T> shared_ptr<T> create_shared()
{
    return shared_ptr<T>(new T);
}

enum TokenType
{
    Reserved = 1,
    Number,
    String,
    Symbol,
    Name,
    Eol,
    Eof
};
enum ReservedType
{
    Error = 0,
    And = 64,
    Break,
    Do,
    Else,
    Elseif,
    End,
    False,
    For,
    Function,
    If,
    In,
    Local,
    Nil,
    Not,
    Or,
    Repeat,
    Return,
    Then,
    True,
    Until,
    While
};
enum SymbolType
{
    ERROR = 0,
    PLUS = 128,
    MINUS,
    MUL,
    DIV,
    MOD,
    POWER,
    HASHTAG,
    NEGATIVE,
    EQ,
    GT,
    LT,
    GE,
    LE,
    NE,
    LPAREN,
    RPAREN,
    LBRACKET,
    RBRACKET,
    LCURLY,
    RCURLY,
    SEMICOLON,
    COLON,
    COMMA,
    DOT,
    DOT2,
    DOT3,
    ASSIGN,
    COMMENT
};
enum ValueType
{
    _Nil = 0,
    _Boolean,
    _Number,
    _String,
    _Function,
    _Table,
    _Name
};
enum ReturnType
{
    RNormal = 0,
    RBreak,
    RReturn
};
enum InstrOp
{
    NO_OP = 0,
    BINARY_OP,
    UNARY_OP,
    PUSH_NUM,
    PUSH_NIL,
    PUSH_TRUE,
    PUSH_FALSE,
    PUSH_STR,
    PUSH_NAME,
    PUSH_TABLE,
    FUNC_CALL,
    DEREF_VAR,
    ASSIGN_VAR,
    JMP_IF,
    JMP_IFN
};

struct Token
{
    TokenType type;
    union {
        ReservedType rtype;
        double num;
        char *str;
        SymbolType stype;
        u32 id;
    };
    Token(TokenType type) : type(type)
    {
    }
    Token(ReservedType rtype) : type(Reserved), rtype(rtype)
    {
    }
    Token(double num) : type(Number), num(num)
    {
    }
    Token(char *str) : type(String), str(str)
    {
    }
    Token(SymbolType stype) : type(Symbol), stype(stype)
    {
    }
    Token(u32 id) : type(Name), id(id)
    {
    }
    bool operator==(const Token &tk) const
    {
        if (type != tk.type)
            return false;
        if (type == Reserved)
            return rtype == tk.rtype;
        if (type == Symbol)
            return stype == tk.stype;
        return true;
    }
    bool operator!=(const Token &tk) const
    {
        return !operator==(tk);
    }
};

namespace Tokenize
{
template <typename T> struct Match
{
    const static int MAX_NODE = 128;
    u8 transition[MAX_NODE][256];
    T info[MAX_NODE];
    u8 cnt;
    Match(const vector<pair<const char *, T>> &vec)
    {
        cnt = 0;
        for (auto &ele : vec)
        {
            const char *ptr = ele.first;
            u8 cur = 0;
            while (*ptr != '\0')
            {
                u8 &nxt = transition[cur][(u8)(*(ptr++))];
                if (nxt == 0)
                    nxt = ++cnt;
                cur = nxt;
            }
            info[cur] = ele.second;
        }
    }
    T readFromReader(Reader &r) const
    {
        u8 cur = 0, temp = 0;
        while ((temp = transition[cur][(u8)r.seek()]))
            cur = temp, r.skip();
        return info[cur];
    }
    template <typename T2> T readFromStr(T2 str) const
    {
        u8 cur = 0, temp = 0;
        while (*str != '\0')
        {
            temp = transition[cur][(u8)(*(str++))];
            if (temp == 0)
                return info[0];
            cur = temp;
        }
        return info[cur];
    }
};

const Match<ReservedType> resMatch(vector<pair<const char *, ReservedType>>{
    {"and", And},       {"break", Break},   {"do", Do},     {"else", Else},         {"elseif", Elseif},
    {"end", End},       {"false", False},   {"for", For},   {"function", Function}, {"if", If},
    {"in", In},         {"local", Local},   {"nil", Nil},   {"not", Not},           {"or", Or},
    {"repeat", Repeat}, {"return", Return}, {"then", Then}, {"true", True},         {"until", Until},
    {"while", While}});
const Match<SymbolType> symMatch(vector<pair<const char *, SymbolType>>{
    {"+", PLUS},   {"-", MINUS},    {"*", MUL},      {"/", DIV},    {"%", MOD},    {"^", POWER},     {"#", HASHTAG},
    {"==", EQ},    {">", GT},       {"<", LT},       {">=", GE},    {"<=", LE},    {"~=", NE},       {"(", LPAREN},
    {")", RPAREN}, {"[", LBRACKET}, {"]", RBRACKET}, {"{", LCURLY}, {"}", RCURLY}, {";", SEMICOLON}, {":", COLON},
    {",", COMMA},  {".", DOT},      {"..", DOT2},    {"...", DOT3}, {"=", ASSIGN}, {"--", COMMENT}});

double parse_number()
{
    double res = 0;
    while (isdigit(reader.seek()))
        res = res * 10 + (reader.read() - '0');
    if (reader.seek() == '.')
    {
        reader.skip();
        double power = 0.1;
        while (isdigit(reader.seek()))
        {
            res += power * (reader.read() - '0');
            power *= 0.1;
        }
    }
    if (reader.seek() == 'E' || reader.seek() == 'e')
    {
        reader.skip();
        double flg = 1, exp = 0;
        if (reader.seek() == '+' || reader.seek() == '-')
            flg = reader.read() == '-' ? -1 : 1;
        while (isdigit(reader.seek()))
            exp = exp * 10 + (reader.read() - '0');
        res *= pow(10, exp * flg);
    }
    return res;
}

Token __current(Eof);
void read_next_token()
{
    while (isspace(reader.seek()) && reader.seek() != '\n')
        reader.skip();
    if (reader.eof())
        __current = Eof;
    else if (reader.seek() == '\n')
        __current = Eol, reader.skip();
    else if (reader.seek() == '\'' || reader.seek() == '"')
    {
        __current = data_ptr;
        char open = reader.read();
        while (!reader.eof() && reader.seek() != open)
        {
            *data_ptr = reader.read();
            if (*data_ptr == '\\')
            {
                char nxt = reader.seek();
                if (nxt == 'n')
                    *data_ptr = '\n';
                else if (nxt == '\\')
                    ;
                else if (nxt == '\'')
                    *data_ptr = '\'';
                else if (nxt == '"')
                    *data_ptr = '"';
                reader.skip();
            }
            ++data_ptr;
        }
        *data_ptr = '\0';
        ++data_ptr;
        reader.skip();
    }
    else if (isdigit(reader.seek()))
    {
        char first = reader.read();
        if (first == '0' && tolower(reader.seek()) == 'x')
        {
            double num = 0;
            reader.skip();
            while (1)
            {
                char cur = reader.seek();
                if (isdigit(cur))
                {
                    num = num * 16 + (cur - '0');
                    reader.skip();
                    continue;
                }
                else if (isalpha(cur))
                {
                    cur = tolower(cur);
                    if (cur <= 'f')
                    {
                        num = num * 16 + (cur - 'a' + 10);
                        reader.skip();
                        continue;
                    }
                }
                break;
            }
            __current = num;
        }
        else
        {
            reader.ungetc(first);
            __current = parse_number();
        }
    }
    else if (isalpha(reader.seek()))
    {
        string id = "";
        while (isalnum(reader.seek()) || reader.seek() == '_')
            id += reader.read();
        id += '\0';
        ReservedType t = resMatch.readFromStr(id.begin());
        if (t == Error)
            __current = insert_id(id.c_str());
        else
            __current = t;
    }
    else
    {
        bool checked = false;
        if (reader.seek() == '.')
        {
            reader.skip();
            if (isdigit(reader.seek()))
            {
                reader.ungetc('.');
                __current = parse_number();
                checked = true;
            }
            else
                reader.ungetc('.');
        }
        if (!checked)
        {
            SymbolType t = symMatch.readFromReader(reader);
            if (t == COMMENT)
            {
                string spec = "";
                while (!reader.eof() && reader.seek() != '\n')
                    spec += reader.read();
                if (spec.substr(0, 8) == " PROGRAM")
                    __current = Eof;
                else
                    read_next_token();
                return;
            }
            __current = t;
        }
    }
}
} // namespace Tokenize

namespace Process
{
const Token &curToken()
{
    return Tokenize::__current;
}
void skipToken()
{
    Tokenize::read_next_token();
}
struct Value
{
    ValueType type;
    union {
        bool boolean;
        double num;
        u32 func_id;
        u32 id;
    };
    string str;
    shared_ptr<map<Value, Value>> table;
    Value(ValueType type = _Nil) : type(type)
    {
    }
    Value(double num) : type(_Number), num(num)
    {
    }
    Value(bool boolean) : type(_Boolean), boolean(boolean)
    {
    }
    Value(string str) : type(_String), str(str)
    {
    }
    Value(char *str) : type(_String), str(str)
    {
    }
    Value(const char *str) : type(_String), str(str)
    {
    }
    Value(u32 id) : type(_Name), id(id)
    {
    }
    Value(shared_ptr<map<Value, Value>> table) : type(_Table), table(table)
    {
    }
    bool is_nil() const
    {
        return type == _Nil;
    }
    bool is_bool() const
    {
        return type == _Boolean;
    }
    bool is_num() const
    {
        return type == _Number;
    }
    bool is_str() const
    {
        return type == _String;
    }
    bool is_func() const
    {
        return type == _Function;
    }
    bool is_table() const
    {
        return type == _Table;
    }
    bool is_name() const
    {
        return type == _Name;
    }
    bool operator==(const Value &v) const
    {
        if (type != v.type)
            return false;
        if (is_bool())
            return boolean == v.boolean;
        if (is_num())
            return num == v.num;
        if (is_str())
            return str == v.str;
        if (is_func())
            return func_id == v.func_id;
        return true;
    }
    bool operator<(const Value &v) const
    {
        if (type != v.type)
            return type < v.type;
        if (is_bool())
            return boolean < v.boolean;
        if (is_num())
            return num < v.num;
        if (is_str())
            return str < v.str;
        if (is_func())
            return func_id < v.func_id;
        return true;
    }
    bool is_true_value() const
    {
        if (is_bool())
            return boolean;
        return type != _Nil;
    }
    string to_str() const
    {
        if (is_bool())
            return boolean ? "true" : "false";
        if (is_num())
        {
            static char buf[24];
            sprintf(buf, "%.14g", num);
            return buf;
        }
        if (is_str())
            return str;
        if (is_nil())
            return "nil";
        if (is_func())
            return "function";
        if (is_table())
            return "table";
        return string("[") + id_hash[id] + "]";
    }
    double table_len() const
    {
        double res = 0;
        auto &tb = *table;
        while (tb.find(res + 1) != tb.end() && !tb[res + 1].is_nil())
            res++;
        return res;
    }
    Value to_num()
    {
        if (is_num())
            return *this;
        if (is_str())
            try
            {
                return stod(str);
            }
            catch (...)
            {
            }
        return _Nil;
    }
};

namespace Memory
{
using Scope = unordered_map<u32, Value>;
vector<Scope> localScopes;
Scope globalScope;
Value &getMem(u32 identi)
{
    int ptr = (int)localScopes.size() - 1;
    while (ptr != -1)
    {
        Scope &s = localScopes[ptr];
        if (s.find(identi) != s.end())
            return s[identi];
        --ptr;
    }
    return globalScope[identi];
}
void init()
{
    localScopes.clear();
    globalScope.clear();
}
} // namespace Memory

struct Instruction
{
    InstrOp type;
    union {
        double num;
        char *str;
        u64 info;
    };
    Instruction(InstrOp type = NO_OP, u64 info = 0) : type(type), info(info)
    {
    }
    Instruction(double num) : type(PUSH_NUM), num(num)
    {
    }
    Instruction(char *str) : type(PUSH_STR), str(str)
    {
    }
};

u8 precedence(const Token &tk, bool first)
{
    if (tk == Or)
        return 2 + first;
    if (tk == And)
        return 4 + first;
    if (tk == LE || tk == GE || tk == LT || tk == GT || tk == EQ || tk == NE)
        return 6 + first;
    if (tk == DOT2)
        return 8 + first;
    if (tk == PLUS || tk == MINUS)
        return 10 + first;
    if (tk == MUL || tk == DIV || tk == MOD)
        return 12 + first;
    if (tk == Not || tk == HASHTAG || tk == NEGATIVE)
        return 14 + !first;
    if (tk == POWER)
        return 16 + !first;
    return 0;
}
bool isUnary(const Token &tk)
{
    return (tk == Not || tk == HASHTAG || tk == NEGATIVE);
}
bool isBinary(const Token &tk)
{
    return precedence(tk, 0) != 0 && !isUnary(tk);
}

struct ReturnState
{
    ReturnType type;
    Value carry;
};

struct AST
{
};

struct ControlAST : AST
{
    ControlAST() : AST()
    {
    }
    virtual ReturnState process() = 0;
    virtual ReturnState run()
    {
        Memory::localScopes.push_back({});
        ReturnState rs = process();
        Memory::localScopes.pop_back();
        return rs;
    }
};

struct FuncAST : AST
{
    vector<u32> names;
    unique_ptr<ControlAST> inner;
    Value run(vector<Value> argv)
    {
        for (u64 i = 0; i < names.size(); i++)
            Memory::localScopes.back()[names[i]] = argv[i];
        return inner->run().carry;
    }
};

struct FuncInfo
{
    bool isAST;
    function<Value(vector<Value>)> sys;
    shared_ptr<FuncAST> ast;
    FuncInfo() : isAST(true), ast(nullptr)
    {
    }
    FuncInfo(function<Value(vector<Value>)> sys) : isAST(false), sys(sys)
    {
    }
    FuncInfo(shared_ptr<FuncAST> ast) : isAST(true), ast(ast)
    {
    }
    FuncInfo(const FuncInfo &f) : isAST(f.isAST), sys(f.sys), ast(f.ast)
    {
    }
    Value call(vector<Value> v)
    {
        if (!isAST)
            return sys(v);
        vector<Memory::Scope> prevScope(1);
        prevScope.swap(Memory::localScopes);
        Value res = ast == nullptr ? _Nil : ast->run(v);
        prevScope.swap(Memory::localScopes);
        return res;
    }
} regFuncs[MAX_FUNCTION];
u32 function_cnt;
Value register_function(FuncInfo f)
{
    regFuncs[++function_cnt] = f;
    Value res(_Function);
    res.func_id = function_cnt;
    return res;
}
Value register_syscall(function<Value(vector<Value>)> sys)
{
    return register_function(FuncInfo(sys));
}

struct ExprAST : AST
{
    vector<Instruction> instr;
    vector<Value> v;

    u32 getVarSuffix()
    {
        u32 res = 0;
        while (curToken().type == Symbol)
        {
            if (curToken() == LBRACKET)
            {
                skipToken();
                getExpr();
                skipToken(), ++res;
            }
            else if (curToken().stype == DOT)
            {
                skipToken();
                instr.emplace_back(id_hash[curToken().id]);
                skipToken();
                ++res;
            }
            else
                break;
        }
        return res;
    }

    void getVarRef()
    {
        instr.emplace_back(PUSH_NAME, curToken().id);
        skipToken();
        u32 u = getVarSuffix();
        instr.emplace_back(DEREF_VAR, u);
    }

    void getSimpleVar()
    {
        if (curToken() == LPAREN)
        {
            skipToken();
            getExpr();
            skipToken();
        }
        else if (curToken().type == Number)
            instr.emplace_back(curToken().num), skipToken();
        else if (curToken().type == String)
            instr.emplace_back(curToken().str), skipToken();
        else if (curToken() == True)
            instr.emplace_back(PUSH_TRUE), skipToken();
        else if (curToken() == False)
            instr.emplace_back(PUSH_FALSE), skipToken();
        else if (curToken() == Nil)
            instr.emplace_back(PUSH_NIL), skipToken();
        else if (curToken() == LCURLY)
        {
            skipToken();
            instr.emplace_back(PUSH_TABLE), skipToken();
        }
        else
        {
            getVarRef();
            if (curToken() == LPAREN)
            {
                skipToken(); // (
                u32 args = 0;
                while (!(curToken() == RPAREN))
                {
                    if (args)
                        skipToken(); // ,
                    getExpr();
                    ++args;
                }
                instr.emplace_back(FUNC_CALL, args);
                skipToken(); // )
            }
        }
    }

    void getExpr()
    {
        vector<Token> ops;
        vector<u64> marks;

#define pop_stk()                                                                                                      \
    {                                                                                                                  \
        if (isUnary(ops.back()))                                                                                       \
            marks.pop_back(), instr.emplace_back(UNARY_OP, ops.back().stype);                                          \
        else                                                                                                           \
        {                                                                                                              \
            if (ops.back() == And || ops.back() == Or)                                                                 \
            {                                                                                                          \
                u64 a = marks[marks.size() - 2], b = marks[marks.size() - 1];                                          \
                instr[a] = {ops.back() == And ? JMP_IFN : JMP_IF, b - 1};                                              \
            }                                                                                                          \
            else                                                                                                       \
                instr.emplace_back(BINARY_OP, ops.back().stype);                                                       \
            marks.pop_back();                                                                                          \
            marks.pop_back();                                                                                          \
        }                                                                                                              \
        ops.pop_back();                                                                                                \
        marks.emplace_back(instr.size());                                                                              \
        instr.emplace_back();                                                                                          \
    }

        if (curToken() == MINUS)
            ops.emplace_back(NEGATIVE), skipToken();
        int state = 0;
        while (1)
        {
            if (state == 0)
            {
                if (isUnary(curToken()))
                    ops.push_back(curToken()), skipToken();
                else
                {
                    getSimpleVar();
                    state = 1;
                    marks.emplace_back(instr.size());
                    instr.emplace_back();
                }
            }
            else
            {
                if (isBinary(curToken()))
                {
                    while (!ops.empty() && precedence(ops.back(), 1) > precedence(curToken(), 0))
                        pop_stk();
                    ops.push_back(curToken());
                    skipToken();
                    state = 0;
                }
                else
                    break;
            }
        }
        while (!ops.empty())
            pop_stk();
        if (curToken() == ASSIGN)
        {
            Instruction &ins = instr[marks.back() - 1];
            ins.type = NO_OP;
            skipToken();
            getExpr();
            instr.emplace_back(ASSIGN_VAR, ins.info);
        }
    }

    Value calc()
    {
        vector<Value> simulate;
        u64 ptr = 0;
        while (ptr != instr.size())
        {
            switch (instr[ptr].type)
            {
            case NO_OP:
                break;
            case UNARY_OP: {
                Value x = simulate.back();
                simulate.pop_back();
                switch (instr[ptr].info)
                {
                case Not:
                    simulate.push_back(!x.is_true_value());
                    break;
                case HASHTAG: {
                    if (x.is_table())
                        simulate.push_back(x.table_len());
                    else if (x.is_str())
                        simulate.push_back(double(x.str.size()));
                    else
                        simulate.push_back(_Nil);
                    break;
                }
                case NEGATIVE: {
                    if (x.is_num())
                        simulate.push_back(-x.num);
                    else
                        simulate.push_back(_Nil);
                    break;
                }
                };
                break;
            }
            case BINARY_OP: {
                Value b = simulate.back();
                simulate.pop_back();
                Value a = simulate.back();
                simulate.pop_back();
                switch (instr[ptr].info)
                {
#define bind(c, pred, oper)                                                                                            \
    case c: {                                                                                                          \
        if (a.pred() && b.pred())                                                                                      \
            simulate.push_back(oper);                                                                                  \
        else                                                                                                           \
            simulate.push_back(_Nil);                                                                                  \
        break;                                                                                                         \
    }
                bind(PLUS, is_num, a.num + b.num) bind(MINUS, is_num, a.num - b.num) bind(MUL, is_num, a.num * b.num)
                    bind(DIV, is_num, a.num / b.num) bind(MOD, is_num, a.num - floor(a.num / b.num) * b.num)
                        bind(POWER, is_num, pow(a.num, b.num)) bind(DOT2, is_str, a.str + b.str) case LT:
                    simulate.push_back(a < b);
                    break;
                case GT:
                    simulate.push_back(b < a);
                    break;
                case LE:
                    simulate.push_back(!(b < a));
                    break;
                case GE:
                    simulate.push_back(!(a < b));
                    break;
                case EQ:
                    simulate.push_back(a == b);
                    break;
                case NE:
                    simulate.push_back(!(a == b));
                    break;
                default:
                    break;
                };
                break;
            }
            case PUSH_NUM:
                simulate.emplace_back(instr[ptr].num);
                break;
            case PUSH_NIL:
                simulate.emplace_back(_Nil);
                break;
            case PUSH_TRUE:
                simulate.emplace_back(true);
                break;
            case PUSH_FALSE:
                simulate.emplace_back(false);
                break;
            case PUSH_STR:
                simulate.emplace_back(string(instr[ptr].str));
                break;
            case PUSH_NAME:
                simulate.emplace_back((u32)instr[ptr].info);
                break;
            case PUSH_TABLE:
                simulate.emplace_back(create_shared<map<Value, Value>>());
                break;
            case FUNC_CALL: {
                vector<Value> argv;
                for (u64 i = instr[ptr].info; i; i--)
                    argv.push_back(simulate.back()), simulate.pop_back();
                reverse(argv.begin(), argv.end());
                Value func = simulate.back();
                simulate.back() = regFuncs[func.func_id].call(argv);
                break;
            }
            case DEREF_VAR: {
                Value v = Memory::getMem(simulate[simulate.size() - instr[ptr].info - 1].id);
                for (u64 i = instr[ptr].info; i; i--)
                    v = (v.is_table()) ? (*v.table)[simulate[simulate.size() - i]] : _Nil;
                for (u64 i = instr[ptr].info; i; i--)
                    simulate.pop_back();
                simulate.back() = v;
                break;
            }
            case ASSIGN_VAR: {
                Value mdf = simulate.back();
                simulate.pop_back();
                Value *v = &Memory::getMem(simulate[simulate.size() - instr[ptr].info - 1].id);
                for (u64 i = instr[ptr].info; i; i--)
                    v = (v != nullptr && v->is_table()) ? &(*(v->table))[simulate[simulate.size() - i]] : nullptr;
                for (u64 i = instr[ptr].info; i; i--)
                    simulate.pop_back();
                simulate.back() = (v == nullptr ? mdf : (*v) = mdf);
                break;
            }
            case JMP_IF:
                if (simulate.back().is_true_value())
                    ptr = instr[ptr].info;
                else
                    simulate.pop_back();
                break;
            case JMP_IFN:
                if (!simulate.back().is_true_value())
                    ptr = instr[ptr].info;
                else
                    simulate.pop_back();
                break;
            default:
                break;
            }
            ++ptr;
        }
        return simulate.back();
    }
};

struct ExprWrapAST : ControlAST
{
    ExprAST inner;
    ReturnState process()
    {
        return {RNormal, inner.calc()};
    }
    ReturnState run()
    {
        return process();
    }
};

struct LocalAssignAST : ControlAST
{
    u32 lhs;
    unique_ptr<ExprAST> rhs;
    ReturnState process()
    {
        Value res = rhs == nullptr ? _Nil : rhs->calc();
        Memory::localScopes.back()[lhs] = res;
        return {RNormal, res};
    }
    ReturnState run()
    {
        return process();
    }
};

struct BlockAST : ControlAST
{
    ReturnType ret;
    unique_ptr<ExprAST> retExpr;
    vector<unique_ptr<ControlAST>> stmts;
    ReturnState process()
    {
        for (auto &ele : stmts)
        {
            ReturnState rs = ele->run();
            if (rs.type != RNormal)
                return rs;
        }
        if (retExpr != nullptr)
            return {RReturn, retExpr->calc()};
        return {ret, _Nil};
    }
};

struct IfAST : ControlAST
{
    using Cond = pair<unique_ptr<ExprAST>, unique_ptr<ControlAST>>;
    vector<Cond> if_conds;
    unique_ptr<ControlAST> else_cond;
    ReturnState process()
    {
        for (const auto &ele : if_conds)
            if (ele.first->calc().is_true_value())
                return ele.second->run();
        if (else_cond != nullptr)
            return else_cond->run();
        return {RNormal, _Nil};
    }
};

struct ForAST : ControlAST
{
    u32 identi;
    unique_ptr<ExprAST> start, end, step;
    unique_ptr<ControlAST> inner;
    ReturnState process()
    {
        double sv = start->calc().to_num().num;
        double ev = end->calc().to_num().num;
        double stv = step == nullptr ? 1 : step->calc().to_num().num;
        for (; (stv > 0 ? sv <= ev : sv >= ev); sv += stv)
        {
            Memory::localScopes.back()[identi] = sv;
            ReturnState rs = inner->run();
            if (rs.type == RBreak)
                return {RNormal, _Nil};
            if (rs.type == RReturn)
                return rs;
        }
        return {RNormal, _Nil};
    }
};

struct ForInAST : ControlAST
{
    u32 identi;
    bool ipair;
    unique_ptr<ExprAST> table;
    unique_ptr<ControlAST> inner;
    ReturnState process()
    {
        Value tb = table->calc();
        vector<Value> vec;
        if (ipair)
        {
            double n = tb.table_len();
            for (double i = 1; i <= n; i++)
                vec.push_back(i);
        }
        else
            for (auto &ele : *tb.table)
                vec.push_back(ele.first);
        for (auto cur : vec)
        {
            Memory::localScopes.back()[identi] = cur;
            ReturnState rs = inner->run();
            if (rs.type == RBreak)
                return {RNormal, _Nil};
            if (rs.type == RReturn)
                return rs;
        }
        return {RNormal, _Nil};
    }
};

struct WhileAST : ControlAST
{
    unique_ptr<ExprAST> expr;
    unique_ptr<ControlAST> inner;
    ReturnState process()
    {
        while (expr->calc().is_true_value())
        {
            ReturnState rs = inner->run();
            if (rs.type == RBreak)
                return {RNormal, _Nil};
            if (rs.type == RReturn)
                return rs;
        }
        return {RNormal, _Nil};
    }
};

struct RepeatAST : ControlAST
{
    unique_ptr<ExprAST> until;
    unique_ptr<ControlAST> inner;
    // use inner.process() to avoid new scope
    ReturnState process()
    {
        bool flg = true;
        while (flg)
        {
            ReturnState rs = inner->process();
            if (rs.type == RBreak)
                return {RNormal, _Nil};
            if (rs.type == RReturn)
                return rs;
            flg = !until->calc().is_true_value();
        }
        return {RNormal, _Nil};
    }
};

unique_ptr<ExprAST> getExpr()
{
    unique_ptr<ExprAST> res = create_unique<ExprAST>();
    res->getExpr();
    while (curToken() == Eol)
        skipToken();
    return res;
}

unique_ptr<ControlAST> getBlock();

unique_ptr<ControlAST> getStmt()
{
    if (curToken() == Do)
    {
        skipToken(); // do
        auto res = getBlock();
        skipToken(); // end
        return res;
    }
    if (curToken() == If)
    {
        auto res = create_unique<IfAST>();
        bool first = true;
        while ((first && curToken() == If) || (!first && curToken() == Elseif))
        {
            skipToken();
            auto ques = getExpr();
            skipToken(); // then
            auto blk = getBlock();
            res->if_conds.emplace_back(move(ques), move(blk));
            first = false;
        }
        if (curToken() == Else)
            skipToken(), res->else_cond = getBlock();
        skipToken(); // end
        return res;
    }
    if (curToken() == For)
    {
        skipToken(); // for
        u32 identi = curToken().id;
        skipToken(); // [id]
        if (curToken() == In)
        {
            auto res = create_unique<ForInAST>();
            res->identi = identi;
            skipToken(); // in
            res->ipair = !strcmp(id_hash[curToken().id], "ipairs");
            skipToken();
            skipToken(); // (
            res->table = getExpr();
            skipToken(); // )
            skipToken(); // do
            res->inner = getBlock();
            skipToken(); // end
            return res;
        }
        else
        {
            auto res = create_unique<ForAST>();
            res->identi = identi;
            skipToken(); // =
            res->start = getExpr();
            skipToken(); // ,
            res->end = getExpr();
            if (curToken() == COMMA)
                skipToken(), res->step = getExpr();
            skipToken(); // do
            res->inner = getBlock();
            skipToken(); // end
            return res;
        }
    }
    if (curToken() == While)
    {
        auto res = create_unique<WhileAST>();
        skipToken(); // while
        res->expr = getExpr();
        skipToken(); // do
        res->inner = getBlock();
        skipToken(); // end
        return res;
    }
    if (curToken() == Repeat)
    {
        auto res = create_unique<RepeatAST>();
        skipToken(); // repeat
        res->inner = getBlock();
        skipToken(); // until
        res->until = getExpr();
        return res;
    }
    if (curToken() == Function)
    {
        skipToken(); // function
        auto res = create_shared<FuncAST>();
        u32 name = curToken().id;
        vector<u32> argv;
        skipToken(); // name
        skipToken(); // (
        bool first = true;
        while (curToken() != RPAREN)
        {
            if (!first)
                skipToken(); // ,
            first = false;
            argv.push_back(curToken().id);
            skipToken();
        }
        res->names = argv;
        skipToken(); // )
        res->inner = getBlock();
        Memory::globalScope[name] = register_function(res);
        skipToken(); // end
        return nullptr;
    }
    if (curToken() == Local)
    {
        auto res = create_unique<LocalAssignAST>();
        skipToken(); // local
        res->lhs = curToken().id;
        skipToken();
        if (curToken() == ASSIGN)
        {
            skipToken(); // =
            res->rhs = getExpr();
        }
        return res;
    }
    auto res = create_unique<ExprWrapAST>();
    (res->inner).getExpr();
    return res;
}

bool blockEnd()
{
    return (curToken() == Until || curToken() == End || curToken() == Eof || curToken() == Elseif ||
            curToken() == Else);
}

unique_ptr<ControlAST> getBlock()
{
    auto res = create_unique<BlockAST>();
    res->ret = RNormal;
    while (1)
    {
        if (curToken() == Eol)
        {
            skipToken();
            continue;
        }
        if (curToken() == Break)
        {
            res->ret = RBreak;
            skipToken();
            break;
        }
        if (curToken() == Return)
        {
            res->ret = RReturn;
            skipToken();
            if (!blockEnd())
                res->retExpr = getExpr();
            break;
        }
        if (blockEnd())
            break;
        auto nxt = getStmt();
        if (nxt != nullptr)
            res->stmts.push_back(move(nxt));
    }
    return res;
}

const double PI = acos(-1.0);

void init_module()
{
#define reg_simple(E, V) E = register_syscall([](vector<Value> args) -> Value { return V; });
    Value &mVal = Memory::getMem(insert_id("math"));
    mVal = create_shared<map<Value, Value>>();
    reg_simple((*mVal.table)["abs"], args[0].is_num() ? Value(abs(args[0].num)) : _Nil)
        reg_simple((*mVal.table)["floor"], args[0].is_num() ? Value(floor(args[0].num)) : _Nil)
            reg_simple((*mVal.table)["ceil"], args[0].is_num() ? Value(ceil(args[0].num)) : _Nil)
                reg_simple((*mVal.table)["sqrt"], args[0].is_num() ? Value(sqrt(args[0].num)) : _Nil);
    reg_simple((*mVal.table)["exp"], args[0].is_num() ? Value(exp(args[0].num)) : _Nil);
    reg_simple((*mVal.table)["log"], args[0].is_num() ? Value(log(args[0].num)) : _Nil);
    reg_simple((*mVal.table)["log10"], args[0].is_num() ? Value(log10(args[0].num)) : _Nil);
    (*mVal.table)["pi"] = PI;
    reg_simple((*mVal.table)["rad"], args[0].is_num() ? Value(args[0].num / 180 * PI) : _Nil);
    reg_simple((*mVal.table)["deg"], args[0].is_num() ? Value(args[0].num * 180 / PI) : _Nil);
    reg_simple((*mVal.table)["acos"], args[0].is_num() ? Value(acos(args[0].num)) : _Nil);
    reg_simple((*mVal.table)["asin"], args[0].is_num() ? Value(asin(args[0].num)) : _Nil);
    reg_simple((*mVal.table)["atan"], args[0].is_num() ? Value(atan(args[0].num)) : _Nil);
    reg_simple((*mVal.table)["atan2"],
               args[0].is_num() && args[1].is_num() ? Value(atan2(args[0].num, args[1].num)) : _Nil);
    reg_simple((*mVal.table)["cos"], args[0].is_num() ? Value(cos(args[0].num)) : _Nil);
    reg_simple((*mVal.table)["sin"], args[0].is_num() ? Value(sin(args[0].num)) : _Nil);
    reg_simple((*mVal.table)["tan"], args[0].is_num() ? Value(tan(args[0].num)) : _Nil);
    reg_simple((*mVal.table)["min"], (args[0] < args[1] ? args[0] : args[1]));
    reg_simple((*mVal.table)["max"], (args[0] < args[1] ? args[1] : args[0]));
    reg_simple(Memory::getMem(insert_id("print")), (puts(args[0].to_str().c_str()), _Nil));
    reg_simple(Memory::getMem(insert_id("tonumber")), args[0].to_num());
    reg_simple(Memory::getMem(insert_id("tostring")), args[0].to_str());
    Value &sVal = Memory::getMem(insert_id("string"));
    sVal = create_shared<map<Value, Value>>();
    (*sVal.table)["rep"] = register_syscall([](vector<Value> args) -> Value {
        if (!args[0].is_str() || !args[1].is_num())
            return _Nil;
        string res = "";
        for (double i = 1; i <= args[1].num; i++)
            res += args[0].str;
        return res;
    });
    (*sVal.table)["sub"] = register_syscall([](vector<Value> args) -> Value {
        if (!args[0].is_str() || !args[1].is_num())
            return _Nil;
        string b = args[0].str;
        int i = args[1].num;
        int j = -1;
        if (args.size() == 3)
        {
            if (args[2].is_num())
                j = args[2].num;
            else
                return _Nil;
        }
        if (i <= 0)
            i = b.size() + 1 + i;
        if (j <= 0)
            j = b.size() + 1 + j;
        return b.substr(i - 1, j - i + 1);
    });
    Value &tVal = Memory::getMem(insert_id("table"));
    tVal = create_shared<map<Value, Value>>();
    (*tVal.table)["concat"] = register_syscall([](vector<Value> args) -> Value {
        if (!args[0].is_table())
            return _Nil;
        string sep = "", res = "";
        if (args.size() == 2 && args[1].is_str())
            sep = args[1].str;
        double n = args[0].table_len();
        for (double i = 1; i <= n; i++)
            res += (i == 1 ? "" : sep) + (*args[0].table)[i].to_str();
        return res;
    });
    (*tVal.table)["sort"] = register_syscall([](vector<Value> args) -> Value {
        if (!args[0].is_table())
            return _Nil;
        double n = args[0].table_len();
        vector<Value> vec;
        for (double i = 1; i <= n; i++)
            vec.push_back((*args[0].table)[i]);
        if (args.size() == 2 && args[1].is_func())
            sort(vec.begin(), vec.end(), [&](const Value &a, const Value &b) {
                return regFuncs[args[1].func_id].call({a, b}).is_true_value();
            });
        else
            sort(vec.begin(), vec.end());
        for (double i = 1; i <= n; i++)
            ((*args[0].table))[i] = vec[i - 1];
        return _Nil;
    });
}

void clear()
{
    data_ptr = str_data;
    for (auto ele : used_hash_index)
        delete id_hash[ele], id_hash[ele] = nullptr;
    used_hash_index.clear();
    for (u32 i = 1; i <= function_cnt; i++)
        regFuncs[i] = FuncInfo();
    function_cnt = 0;
    Memory::init();
}

void run()
{
    reader = Reader();
    skipToken();
    u32 program_count = 0;
    while (!reader.eof())
    {
        init_module();
        skipToken();
        printf("Program %u:\n", ++program_count);
        unique_ptr<ControlAST> root = getBlock();
        root->run();
        puts("");
    }
}

} // namespace Process

int main()
{
    freopen("Stage3.in", "r", stdin);
    freopen("Stage3.out", "w", stdout);
    Process::run();
    return 0;
}