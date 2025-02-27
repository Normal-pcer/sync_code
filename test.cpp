#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>

using namespace std;

using u8 = unsigned char;
using i16 = short;
using u16 = unsigned short;
using i32 = int;
using u32 = unsigned int;
using i64 = long long;
using u64 = unsigned long long;

struct Reader {
    char ch;

    Reader() {
        ch = getchar();
    }

    char seek() const { return ch; }

    void skip() {
        if (eof()) {
            throw runtime_error("Unexpected end of file.");
        }
        ch = getchar();
    }

    bool eof() const {
        return feof(stdin);
    }

    char read() {
        char res = seek();
        skip();
        return res;
    }

    void read(char &c) {
        c = read();
    }

    void skipWhitespace() {
      while (isspace(seek())) {
        skip();
      }
    }
};

Reader reader;

int input_n, input_a[512], input_cur;

enum TokenType {
    Reserved = 1, Number, Symbol, Name, Eof
};
enum ReservedType {
    Error = 0, Using, Int, If, Else, For, While, Return, Cin, Cout, Endl
};
enum SymbolType {
    ERROR = 0, HASHTAG, LPAREN, RPAREN, LBRACKET, RBRACKET, LCURLY, RCURLY,
    OR, XOR, AND, NOT, EQ, NE, LT, GT, LE, GE, ADD, SUB, MUL, DIV, MOD, NEG, SHL, SHR,
    SEMICOLON, COMMA, DOT, ASSIGN
};

enum InstructionType {
    _LEA, _LEAP, _IMM, _JMP, _JSR, _BZ, _BNZ, _ENT, _ADJ, _LEV, _LI, _SI, _PSH,
    _OR, _XOR, _AND, _NOT, _EQ, _NE, _LT, _GT, _LE, _GE,
    _ADD, _SUB, _MUL, _DIV, _MOD, _NEG, _SHL, _SHR,
    _SYSCALL,
    _PCHR, // 添加 _PCHR
    _EXIT
};

enum Syscall {
    SYS_EXIT = 0, SYS_PCHR, SYS_READ, SYS_WRITE
};

namespace VM {
    const int MEM_SIZE = 1 << 22;
    const int BYTE_CODE_LENGTH = 1 << 12;
    i64 mem[MEM_SIZE];
    i64 bytecode[BYTE_CODE_LENGTH];

     int run(i64 start) {
        i64 *pc = (i64*)start, *sp = mem + MEM_SIZE, *bp = sp, ax, tmp;
        *--sp = _EXIT;
        *--sp = _PSH;
        tmp = (i64)sp;
        *--sp = tmp;

        while (true) {
            i64 ins = *pc++;

            switch (ins) {
                case _LEA: ax = (i64)(bp + *pc++); break;
                case _LEAP: tmp = ax * (*pc++); ax = (*sp++) + tmp; break;
                case _IMM: ax = *pc++; break;
                case _JMP: pc = (i64*)*pc; break;
                case _JSR: *--sp = (i64)(pc + 1); pc = (i64*)(*pc); break;
                case _BZ: pc = ax ? pc + 1 : (i64*)(*pc); break;
                case _BNZ: pc = ax ? (i64*)(*pc) : pc + 1; break;
                case _ENT: *--sp = (i64)bp; bp = sp; break;
                case _ADJ: if (*pc < 0) memset(sp + *pc, 0, sizeof(i64) * -(*pc)); sp = sp + *pc++; break;
                case _LEV: sp = bp; bp = (i64*)*sp++; pc = (i64*)*sp++; break;
                case _LI:
                    if (ax < 0 || (u64)ax >= (u64)MEM_SIZE * sizeof(i64)) {  // 修正类型比较
                      throw runtime_error("Memory access violation (LI).");
                    }
                    ax = *(i64*)ax;
                    break;
                case _SI:
                   if ((*sp) < 0 || (u64)(*sp) >= (u64)MEM_SIZE * sizeof(i64)) { // 修正类型比较
                        throw runtime_error("Memory access violation (SI).");
                    }
                    *(i64*)*sp++ = ax;
                    break;
                case _PSH: *--sp = ax; break;
                case _OR: ax = *sp++ || ax; break;
                case _XOR: ax = bool(*sp++) ^ bool(ax); break;
                case _AND: ax = *sp++ && ax; break;
                case _NOT: ax = !ax; break;
                case _EQ: ax = *sp++ == ax; break;
                case _NE: ax = *sp++ != ax; break;
                case _LT: ax = *sp++ < ax; break;
                case _GT: ax = *sp++ > ax; break;
                case _LE: ax = *sp++ <= ax; break;
                case _GE: ax = *sp++ >= ax; break;
                case _ADD: ax = *sp++ + ax; break;
                case _SUB: ax = *sp++ - ax; break;
                case _MUL: ax = *sp++ * ax; break;
                case _DIV:
                    if (ax == 0) {
                        throw runtime_error("Division by zero.");
                    }
                    ax = *sp++ / ax;
                    break;
                case _MOD:
                    if (ax == 0) {
                        throw runtime_error("Modulo by zero.");
                    }
                    ax = *sp++ % ax; break;
                case _NEG: ax = -ax; break;
                case _SHL:
                    *--sp = SYS_WRITE;
                    *--sp = 1;
                    *--sp = (i64)&ax;
                    *--sp = sizeof(i64);
                    *global_ptr++ = _SYSCALL;  // 使用全局 ptr
                    break;
                case _SHR:
                    *--sp = SYS_READ;
                    *--sp = 0;
                    *--sp = ax;
                    *--sp = sizeof(i64);
                    *global_ptr++ = _SYSCALL; // 使用全局 ptr
                    break;

                case _SYSCALL: {
                    Syscall syscall_num = (Syscall)*sp++;
                    switch (syscall_num) {
                      case SYS_EXIT:
                        return (int)ax;
                      case SYS_PCHR:
                        putchar(ax);
                        sp++;
                        break;
                      case SYS_READ: {
                        i64 fd = *sp++;
                        i64 buf = *sp++;
                        i64 count = *sp++;
                        if (fd == 0) {
                            if (count == sizeof(i64)) {
                              if(input_cur < input_n){
                                *(i64*)buf = input_a[input_cur++];
                                ax = sizeof(i64);
                              }
                              else {
                                ax = 0;
                              }

                            }
                            else {
                              ax = 0;
                            }
                        } else {
                            ax = -1;
                        }
                        break;
                      }
                      case SYS_WRITE: {
                        i64 fd = *sp++;
                        i64 buf = *sp++;
                        i64 count = *sp++;
                        if (fd == 1) {
                          if (count == sizeof(i64)) {
                            printf("%lld", *(i64*)buf);
                             ax = sizeof(i64);
                          } else if (count == 1) {
                             putchar(*(char*)buf);
                             ax = 1;
                          } else{
                            ax = 0;
                          }

                        } else {
                          ax = -1;
                        }
                        break;
                      }
                      default:
                        throw runtime_error("Unknown syscall.");
                    }
                    break;
                }
                case _PCHR: // 添加 _PCHR 的处理
                  putchar(ax);
                  break;
                case _EXIT:
                  return (int)ax;
                default:
                  throw runtime_error("Invalid instruction.");
            }
        }
    }
};

const u32 ID_HASH_LENGTH = 1 << 12;
char* id_hash[ID_HASH_LENGTH];

u32 easy_hash(const char *str) {
    u32 res = 0;
    while (*str != '\0')
        res = res * 331 + *(str++);
    return res;
}

u32 insert_id(const char *str) {
    u32 hsh = easy_hash(str) & (ID_HASH_LENGTH - 1);
    while (id_hash[hsh] != nullptr && strcmp(id_hash[hsh], str))
        hsh = (hsh + 1) & (ID_HASH_LENGTH - 1);
    if (id_hash[hsh] == nullptr) {
        char* strr = new char[strlen(str) + 1];
        strcpy(strr, str);
        id_hash[hsh] = strr;
    }
    return hsh;
}

namespace TokenFlow {
    template <typename T>
    struct Match {
        const static int MAX_NODE = 128;
        u8 transition[MAX_NODE][256];
        T info[MAX_NODE];
        u8 cnt;

        Match(const vector<pair<const char*, T>> &vec) {
            cnt = 0;
            memset(transition, 0, sizeof(transition));
            for (auto &ele : vec) {
                const char *ptr = ele.first;
                u8 cur = 0;
                while (*ptr != '\0') {
                    u8 &nxt = transition[cur][(u8)(*(ptr++))];
                    if (nxt == 0) nxt = ++cnt;
                    cur = nxt;
                }
                info[cur] = ele.second;
            }
        }

        T readFromReader() const {
            u8 cur = 0, temp = 0;
            while ((temp = transition[cur][(u8)reader.seek()])) {
                cur = temp;
                reader.skip();
            }
            return info[cur];
        }

        template <typename T2>
        T readFromStr(T2 str) const {
            u8 cur = 0, temp = 0;
            while (*str != '\0') {
                temp = transition[cur][(u8)(*(str++))];
                if (temp == 0) return info[0];
                cur = temp;
            }
            return info[cur];
        }
    };

    const Match<ReservedType> resMatch(vector<pair<const char*, ReservedType>>{
        {"using", Using}, {"int", Int}, {"for", For}, {"while", While}, {"if", If},
        {"else", Else}, {"cin", Cin}, {"cout", Cout}, {"endl", Endl}, {"return", Return}
    });
    const Match<SymbolType> symMatch(vector<pair<const char*, SymbolType>>{
        {"+", ADD}, {"-", SUB}, {"*", MUL}, {"/", DIV}, {"%", MOD}, {"^", XOR}, {"#", HASHTAG},
        {"==", EQ}, {">", GT}, {"<", LT}, {">=", GE}, {"<=", LE}, {"!=", NE}, {"&&", AND}, {"||", OR}, {"!", NOT},
        {"(", LPAREN}, {")", RPAREN}, {"[", LBRACKET}, {"]", RBRACKET}, {"{", LCURLY}, {"}", RCURLY},
        {";", SEMICOLON}, {",", COMMA}, {".", DOT}, {"=", ASSIGN}, {"<<", SHL}, {">>", SHR}
    });

    struct Token {
        TokenType type;
        union {
            ReservedType rtype;
            i64 num;
            SymbolType stype;
            u32 id;
        };
        Token(TokenType type): type(type) {}
        Token(ReservedType rtype): type(Reserved), rtype(rtype) {}
        Token(i64 num): type(Number), num(num) {}
        Token(SymbolType stype): type(Symbol), stype(stype) {}
        Token(u32 id): type(Name), id(id) {}
        bool operator == (const Token &tk) const {
            if (type != tk.type)
                return false;
            if (type == Reserved)
                return rtype == tk.rtype;
            if (type == Symbol)
                return stype == tk.stype;
            return true;
        }
        bool operator != (const Token &tk) const {
            return !operator==(tk);
        }
    };

    Token __current(Eof);
     void read_next_token() {
        reader.skipWhitespace();

        if (reader.eof()) {
            __current = Eof;
            return;
        }

        if (isdigit(reader.seek())) {
            i64 num = 0;
            bool negative = false;
            if (reader.seek() == '-') {
                negative = true;
                reader.skip();
            }
            while (isdigit(reader.seek())) {
                num = num * 10 + (reader.read() - '0');
            }
            __current = negative ? -num : num;

        } else if (isalpha(reader.seek()) || reader.seek() == '_') {
            string str = "";
            while (isalnum(reader.seek()) || reader.seek() == '_') {
                str += reader.read();
            }
            auto tp = resMatch.readFromStr(str.c_str());
            if (tp == Error) {
                __current = insert_id(str.c_str());
            } else if (tp == Using) {
                while (!reader.eof() && reader.seek() != '\n') {
                    reader.skip();
                }
                read_next_token();
                return;
            } else {
                __current = tp;
            }
        } else {
            auto tp = symMatch.readFromReader();
            if (tp == HASHTAG) {
                while (!reader.eof() && reader.seek() != '\n') {
                    reader.skip();
                }
                read_next_token();
            } else {
                __current = tp;
            }
        }
    }
};

const TokenFlow::Token& tk() { return TokenFlow::__current; }
void nxt() { TokenFlow::read_next_token(); }

int* declare[ID_HASH_LENGTH];
i64 address[ID_HASH_LENGTH];
bool global[ID_HASH_LENGTH];
int SINGLE_VAR[1] = {1};

int* get_array_def_params() {
    vector<int> res;
    while (tk() == LBRACKET) {
        nxt();
        if (tk().type != Number) {
           throw runtime_error("Expected number in array size definition.");
        }
        res.push_back(tk().num);
        nxt();
        if (tk() != RBRACKET) {
          throw runtime_error("Expected ']' in array size definition.");
        }
        nxt();
    }
    res.push_back(1);
    for (int i = (int)res.size() - 2; i >= 0; i--)
        res[i] *= res[i + 1];
    int *p = new int[res.size()];
    for (int i = 0; i < (int)res.size(); i++)
        p[i] = res[i];
    return p;
}

i64 *global_ptr = VM::bytecode; // 全局字节码指针
int stk_tp = 0;
struct RecoverVar {
    vector<tuple<u32, i64, bool, int*>> rec;
    void cover(u32 i, int loc, int* arr = SINGLE_VAR) {
        rec.push_back({i, address[i], global[i], declare[i]});
        address[i] = loc;
        declare[i] = arr;
        global[i] = false;
    }
    void recover(int params = 0) {
        int U = -params;
        for (auto &ele: rec) {
            u32 id = get<0>(ele);
            U += declare[id][0];
            address[id] = get<1>(ele);
            global[id] = get<2>(ele);
            declare[id] = get<3>(ele);
        }
        if (U != 0)
            *global_ptr++ = _ADJ, *global_ptr++ = U, stk_tp += U;
    }
};

int precedence(SymbolType s) {
  switch (s) {
    case SHL: case SHR: return 1;
    case ASSIGN: return 2;
    case OR: return 3;
    case AND: return 4;
    case XOR: return 5;
    case EQ: case NE: return 6;
    case LE: case GE: case LT: case GT: return 7;
    case ADD: case SUB: return 8;
    case MUL: case DIV: case MOD: return 9;
    case NEG: case NOT: return 10;
    default: return 0;
  }
}

void get_expr();
bool nxt_endl = false;
u32 syscall_putchar, syscall_getchar;
void get_unit() {
    if (tk() == LPAREN) {
        nxt();
        get_expr();
        if (tk() != RPAREN) {
            throw runtime_error("Expected ')'.");
        }
        nxt();
    } else if (tk().type == Name) {
        u32 name = tk().id;
        nxt();
        if (tk() == LPAREN) {
            nxt();
            int num_args = 0;
            while (tk() != RPAREN) {
                if (num_args > 0) {
                  if(tk() != COMMA){
                    throw runtime_error("Expected ',' in function arguments.");
                  }
                  nxt();
                }
                get_expr();
                *global_ptr++ = _PSH;
                num_args++;
            }
            nxt();

            if (name == syscall_putchar) {
                *global_ptr++ = _SYSCALL;
            } else if (name == syscall_getchar) {
                *global_ptr++ = _SYSCALL;
            }
            else {
                *global_ptr++ = _JSR;
                *global_ptr++ = address[name];
                if (num_args > 0) {
                    *global_ptr++ = _ADJ;
                    *global_ptr++ = num_args;
                }
            }

        }
        else {
            if (global[name])
                *global_ptr++ = _IMM, *global_ptr++ = (i64)(VM::mem) + address[name] * sizeof(i64);
            else
                *global_ptr++ = _LEA, *global_ptr++ = address[name];
            int num = 0;
            while (tk() == LBRACKET) {
                *global_ptr++ = _PSH;
                nxt();
                get_expr();
                if (tk() != RBRACKET) {
                  throw runtime_error("Expected ']' for array access.");
                }
                nxt();
                *global_ptr++ = _LEAP; *global_ptr++ = declare[name][++num] * sizeof(i64);
            }
            *global_ptr++ = _LI;
        }
    } else if (tk().type == Number)
        *global_ptr++ = _IMM, *global_ptr++ = tk().num, nxt();
    else if (tk() == Endl)
        nxt_endl = true, *global_ptr++ = _IMM, *global_ptr++ = 10, nxt();
    else if (tk() == Cin) {
      nxt();
      if (tk() != SHR) {
          throw runtime_error("Expected '>>' after 'cin'.");
      }
      nxt();
      if(tk().type != Name){
        throw runtime_error("Expected variable name after 'cin >>'.");
      }
      u32 name = tk().id;
      nxt();

      if (global[name])
          *global_ptr++ = _IMM, *global_ptr++ = (i64)(VM::mem) + address[name] * sizeof(i64);
      else
          *global_ptr++ = _LEA, *global_ptr++ = address[name];

      int num = 0;
      while (tk() == LBRACKET) {
          *global_ptr++ = _PSH;
          nxt();
          get_expr();
          if (tk() != RBRACKET) {
            throw runtime_error("Expected ']' for array access after cin.");
          }
          nxt();
          *global_ptr++ = _LEAP; *global_ptr++ = declare[name][++num] * sizeof(i64);
      }

      *global_ptr++ = _SHR;
      *global_ptr++ = _PSH;

    }
    else if (tk() == Cout) {
        nxt();
        if (tk() != SHL) {
            throw runtime_error("Expected '<<' after 'cout'.");
        }
        nxt();
        get_expr();
        *global_ptr++ = _SHL;
        *global_ptr++ = _PSH;
    }
    else
    {
      throw runtime_error("Invalid expression.");
    }
}

void get_expr() {
    vector<SymbolType> stk;
    int state = 0, temp = 0, temp2 = 0;

    if (tk().type == Name) {
      get_unit();
      if (tk() == ASSIGN) {
        *(global_ptr - 1) = _PSH;
        nxt();
        get_expr();
        *global_ptr++ = _SI;
        return;
      }
      state = 1;
    }

    #define pop() {\
      if (nxt_endl)\
          *global_ptr++ = _PCHR, nxt_endl = false;\
      else if (stk.back() == SHR)\
          *global_ptr++ = _SHR;\
      else\
          *global_ptr++ = stk.back() - OR + _OR;\
      stk.pop_back();\
    }
    while (true) {
        if (state == 0) {
            if (tk() == NOT) stk.push_back(NOT), nxt();
            else if (tk() == SUB) stk.push_back(NEG), nxt();
            else if (tk() == ADD) nxt();
            else get_unit(), state = 1;
        } else {
            if (tk().type == Symbol && (temp = precedence(tk().stype))) {
                while (!stk.empty() && (temp2 = precedence(stk.back())) >= temp)
                    pop();
                stk.push_back(tk().stype);
                nxt();
                *global_ptr++ = _PSH;
                state = 0;
            } else break;
        }
    }
    while (!stk.empty())
        pop();
}

RecoverVar rv;

void get_stmt();

void get_stmts() {
    RecoverVar nrv;
    swap(rv, nrv);
    while (tk() != RCURLY) {
      get_stmt();
    }

    swap(rv, nrv);
    nrv.recover();
}

void get_basic_stmt() {
  if (tk() == Int) {
      int o = 0;
      do {
          nxt();
          if (tk().type != Name) {
              throw runtime_error("Expected variable name after 'int'.");
          }
          u32 name = tk().id;
          nxt();
          int *v = get_array_def_params();
          o -= v[0];
          stk_tp -= v[0];
          rv.cover(name, stk_tp, v);
      } while (tk() == COMMA);
        if(tk() != SEMICOLON){
           throw runtime_error("Expected ';' after variable declaration.");
        }
      *global_ptr++ = _ADJ;
      *global_ptr++ = o;
  } else {
    get_expr();
  }
}

void get_stmt() {
    if (tk() == SEMICOLON)
        nxt();
    else if (tk() == LCURLY) {
        nxt();
        get_stmts();
        if (tk() != RCURLY) {
          throw runtime_error("Expected '}'.");
        }
        nxt();
    } else if (tk() == If) {
        nxt();
        if (tk() != LPAREN) {
           throw runtime_error("Expected '(' after 'if'.");
        }
        nxt();
        get_expr();
        if (tk() != RPAREN) {
            throw runtime_error("Expected ')' after if condition.");
        }
        nxt();
        *global_ptr++ = _BZ;
        i64 *step_ptr = global_ptr++;
        get_stmt();
        if (tk() == Else) {
            *global_ptr++ = _JMP;
            i64 *end_ptr = global_ptr++;
            *step_ptr = (i64)global_ptr;
            nxt();
            get_stmt();
            *end_ptr = (i64)global_ptr;
        } else
            *step_ptr = (i64)global_ptr;
    } else if (tk() == For) {
        RecoverVar nrv;
        swap(rv, nrv);
        nxt();
        if (tk() != LPAREN) {
          throw runtime_error("Expected '(' after 'for'.");
        }
        nxt();
        if (tk() != SEMICOLON) get_basic_stmt();
         if(tk() != SEMICOLON){
          throw runtime_error("Expected ';' in for loop.");
        }
        nxt();
        i64 start_ptr = (i64)global_ptr;
        if (tk() == SEMICOLON) *global_ptr++ = _IMM, *global_ptr++ = 1;
        else get_expr();
        *global_ptr++ = _BNZ;
        i64 *stmt_ptr = global_ptr++;
        *global_ptr++ = _JMP;
        i64 *end_ptr = global_ptr++;

        if(tk() != SEMICOLON){
          throw runtime_error("Expected ';' in for loop.");
        }
        nxt();
        i64 effect_ptr = (i64)global_ptr;
        if (tk() != RPAREN) get_basic_stmt();
        if(tk() != RPAREN){
           throw runtime_error("Expected ')' in for loop.");
        }
        nxt();
        *global_ptr++ = _JMP;
        *global_ptr++ = start_ptr;
        *stmt_ptr = (i64)global_ptr;
        get_stmt();
        *global_ptr++ = _JMP;
        *global_ptr++ = effect_ptr;
        *end_ptr = (i64)global_ptr;
        swap(rv, nrv);
        nrv.recover();
    } else if (tk() == While) {
        nxt();
        if (tk() != LPAREN) {
           throw runtime_error("Expected '(' after 'while'.");
        }
        nxt();
        i64 start_ptr = (i64)global_ptr;
        get_expr();
        if(tk() != RPAREN){
          throw runtime_error("Expected ')' after while condition.");
        }
        nxt();
        *global_ptr++ = _BZ;
        i64 *end_ptr = global_ptr++;
        get_stmt();
        *global_ptr++ = _JMP;
        *global_ptr++ = start_ptr;
        *end_ptr = (i64)global_ptr;
    } else if (tk() == Return) {
        nxt();
        get_expr();
        if(tk() != SEMICOLON){
          throw runtime_error("Expected ';' after return statement.");
        }
        nxt();
        *global_ptr++ = _LEV;
    }
    else{
      get_basic_stmt();
      if (tk() != SEMICOLON) {
        throw runtime_error("Expected ';'.");
      }
      nxt();
    }

}

void get_func(u32 name) {
    address[name] = (i64)global_ptr;
    global[name] = true;
    *global_ptr++ = _ENT;
    nxt();
    RecoverVar nrv;
    vector<u32> params;
    while (tk() != RPAREN) {
        if (!params.empty()){
          if(tk() != COMMA){
            throw runtime_error("Expected ',' between function parameters.");
          }
          nxt();
        }

        if(tk() != Int){
          throw runtime_error("Expected 'int' for function parameter type.");
        }
        nxt();
        if (tk().type != Name) {
          throw runtime_error("Expected parameter name.");
        }
        params.push_back(tk().id);
        nxt();
    }
    if(tk() != RPAREN){
       throw runtime_error("Expected ')' after function parameters.");
    }
    nxt();
    int n = params.size();
    for (int i = 0; i < n; i++)
        nrv.cover(params[i], n + 1 - i);
    swap(rv, nrv);
     if (tk() != LCURLY) {
       throw runtime_error("Expected '{' for function body.");
     }
    get_stmt();
    *global_ptr++ = _IMM;
    *global_ptr++ = 0;
    *global_ptr++ = _LEV;
    swap(rv, nrv);
    nrv.recover(n);
}

void get_func_and_var() {
    int bss = 0;
    while (tk() != Eof) {
      if (tk() != Int) {
        throw runtime_error("Expected 'int' at global scope.");
      }
      nxt();
      if (tk().type != Name) {
        throw runtime_error("Expected variable or function name.");
      }
      u32 name = tk().id;
      nxt();
      if (tk() == LPAREN) {
          get_func(name);
      }
      else {
        declare[name] = get_array_def_params();
        address[name] = bss;
        bss += declare[name][0];
        global[name] = true;
        while (tk() == COMMA) {
            nxt();
            if(tk().type != Name){
               throw runtime_error("Expected variable name after ','.");
            }
            name = tk().id;
            nxt();
            declare[name] = get_array_def_params();
            address[name] = bss;
            bss += declare[name][0];
            global[name] = true;
        }
        if (tk() != SEMICOLON) {
          throw runtime_error("Expected ';' after global variable declaration.");
        }
        nxt();
      }
    }
}

int main() {
    input_cur = 0;
    cin >> input_n;
    if(input_n > 512){
      cerr << "Too many input numbers." << endl;
      return 1;
    }
    for (int i = 0; i < input_n; i++) {
        cin >> input_a[i];
    }

    syscall_putchar = insert_id("putchar");
    syscall_getchar = insert_id("getchar");
    u32 syscall_main = insert_id("main");

    address[syscall_putchar] = (i64)global_ptr;
    *global_ptr++ = _ENT;
    *global_ptr++ = _IMM; *global_ptr++ = SYS_PCHR;
    *global_ptr++ = _PSH;
    *global_ptr++ = _SYSCALL;
    *global_ptr++ = _IMM; *global_ptr++ = 0; *global_ptr++ = _LEV;

    address[syscall_getchar] = (i64)global_ptr;
    *global_ptr++ = _ENT;
    *global_ptr++ = _IMM; *global_ptr++ = SYS_READ;
    *global_ptr++ = _PSH;
    *global_ptr++ = _SYSCALL;
    *global_ptr++ = _IMM; *global_ptr++ = 0; *global_ptr++ = _LEV;

    nxt();
    try {
        get_func_and_var();
        VM::run(address[syscall_main]);
    } catch (const runtime_error& error) {
        cerr << "Runtime error: " << error.what() << endl;
        return 1;
    }

    for (u32 i = 0; i < ID_HASH_LENGTH; ++i) { // 修正循环变量类型
        delete[] id_hash[i];
    }
    for (u32 i = 0; i < ID_HASH_LENGTH; ++i) { // 修正循环变量类型
        if(declare[i] != nullptr && declare[i] != SINGLE_VAR)
            delete[] declare[i];
    }

    return 0;
}
