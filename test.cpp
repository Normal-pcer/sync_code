#include <cstdio>
#include <vector>
#include <cctype>
#include <cstring>
#include <string>
#include <cmath>
using namespace std;

using u8 = unsigned char;
using i16 = short;
using u16 = unsigned short;
using i32 = int;
using u32 = unsigned int;

string cur_token_content;

struct Reader {
  char ch, unget;
  Reader() {
    unget = '\0';
    ch = getchar();
  }
  char seek() { return unget ? unget : ch; }
  void skip() { if (unget) unget = '\0'; else cur_token_content += ch, ch = getchar(); }
  bool eof() { return ch == '\0' && unget == '\0'; }
  void read(char &c) { c = seek(); skip(); }
  char read() { char res; return read(res), res; }
  void ungetc(char c) { unget = c; }
};

const u32 MAX_DATA_SIZE = 32 * 1024;
const u32 ID_HASH_LENGTH = 512 * 1024;
char str_data[MAX_DATA_SIZE];
char* id_hash[ID_HASH_LENGTH];
u32 easy_hash(const char *str) {
  u32 res = 0;
  while (*str != '\0')
    res = res * 331 + *(str ++);
  return res;
}

enum TokenType {
  Reserved = 1, Number, String, Symbol, Name, Eol
};
enum ReservedType {
  Error = 0, And = 64, Break, Do, Else, Elseif, End, False, For, Function,
  If, In, Local, Nil, Not, Or, Repeat, Return, Then, True, Until, While
};
enum SymbolType {
  ERROR = 0, PLUS = 128, MINUS, MUL, DIV, MOD, POWER, HASHTAG, EQ, GT, LT, GE, LE, NE,
  LPAREN, RPAREN, LBRACKET, RBRACKET, LCURLY, RCURLY,
  SEMICOLON, COLON, COMMA, DOT, DOT2, DOT3, ASSIGN, COMMENT
};

template <typename T>
struct Match {
  const static int MAX_NODE = 128;
  u8 transition[MAX_NODE][256];
  T info[MAX_NODE];
  u8 cnt;
  Match(const vector<pair<const char*, T>> &vec) {
    cnt = 0;
    for (auto &ele: vec) {
      const char *ptr = ele.first;
      u8 cur = 0;
      while (*ptr != '\0') {
        u8 &nxt = transition[cur][(u8)(*(ptr ++))];
        if (nxt == 0) nxt = ++ cnt;
        cur = nxt;
      }
      info[cur] = ele.second;
    }
  }
  T readFromReader(Reader& r) const {
    u8 cur = 0, temp = 0;
    while ((temp = transition[cur][(u8)r.seek()]))
      cur = temp, r.skip();
    return info[cur];
  }
  template <typename T2>
  T readFromStr(T2 str) const {
    u8 cur = 0, temp = 0;
    while (*str != '\0') {
      temp = transition[cur][(u8)(*(str ++))];
      if (temp == 0) return info[0];
      cur = temp;
    }
    return info[cur];
  }
};

const Match<ReservedType> resMatch(vector<pair<const char*, ReservedType>>{
  {"and", And}, {"break", Break}, {"do", Do}, {"else", Else}, {"elseif", Elseif},
  {"end", End}, {"false", False}, {"for", For}, {"function", Function}, {"if", If},
  {"in", In}, {"local", Local}, {"nil", Nil}, {"not", Not}, {"or", Or},
  {"repeat", Repeat}, {"return", Return}, {"then", Then}, {"true", True}, {"until", Until},
  {"while", While} 
});
const Match<SymbolType> symMatch(vector<pair<const char*, SymbolType>>{
  {"+", PLUS}, {"-", MINUS}, {"*", MUL}, {"/", DIV}, {"%", MOD}, {"^", POWER}, {"#", HASHTAG},
  {"==", EQ}, {">", GT}, {"<", LT}, {">=", GE}, {"<=", LE}, {"~=", NE},
  {"(", LPAREN}, {")", RPAREN}, {"[", LBRACKET}, {"]", RBRACKET}, {"{", LCURLY}, {"}", RCURLY},
  {";", SEMICOLON}, {":", COLON}, {",", COMMA}, {".", DOT}, {"..", DOT2}, {"...", DOT3}, {"=", ASSIGN}, {"--", COMMENT}
});

struct Token {
  TokenType type;
  union {
    ReservedType rtype;
    double num;
    char* str;
    SymbolType stype;
    u32 id;
  };
  Token(TokenType type): type(type) {}
  Token(ReservedType rtype): type(Reserved), rtype(rtype) {}
  Token(double num): type(Number), num(num) {}
  Token(char* str): type(String), str(str) {}
  Token(SymbolType stype): type(Symbol), stype(stype) {}
  Token(u32 id): type(Name), id(id) {}
};

vector<Token> tokens;

pair<bool, double> parse_number(Reader &reader) {
  double res = 0;
  while (isdigit(reader.seek()))
    res = res * 10 + (reader.read() - '0');
  if (reader.seek() == '.') {
    reader.skip();
    double power = 0.1;
    while (isdigit(reader.seek())) {
      res += power * (reader.read() - '0');
      power *= 0.1;
    }
  }
  if (reader.seek() == 'E' || reader.seek() == 'e') {
    reader.skip();
    double flg = 1, exp = 0;
    if (reader.seek() == '+' || reader.seek() == '-')
      flg = reader.read() == '-' ? -1 : 1;
    bool good = false;
    while (isdigit(reader.seek()))
      exp = exp * 10 + (reader.read() - '0'), good = true;
    if (!good)
      return {false, 0};
    return {true, res * pow(10, exp * flg)};
  }
  return {true, res};
}

bool tokenize(Reader &reader) {
  char *data_ptr = str_data;
  while (!reader.eof()) {
    cur_token_content = "";
    if (reader.seek() == '\n') {
      tokens.push_back(Eol);
      reader.skip();
    }
    else if (isspace(reader.seek())) {
      reader.skip();
      continue;
    }
    else if (reader.seek() == '\'' || reader.seek() == '"') {
      tokens.push_back(data_ptr);
      char open = reader.read();
      while (!reader.eof() && reader.seek() != open) {
        *data_ptr = reader.read();
        if (*data_ptr == '\\') {
          char nxt = reader.seek();
          if (nxt == 'n') *data_ptr = '\n';
          else if (nxt == '\\') ;
          else if (nxt == '\'') *data_ptr = '\'';
          else if (nxt == '"') *data_ptr = '"';
          else return false;
          reader.skip();
        }
        ++ data_ptr;
      }
      if (reader.eof()) return false;
      ++ data_ptr;
      reader.skip();
    }
    else if (isdigit(reader.seek())) {
      char first = reader.read();
      if (first == '0' && tolower(reader.seek()) == 'x') {
        bool bad = true;
        double num = 0;
        reader.skip();
        while (1) {
          char cur = reader.seek();
          if (isdigit(cur)) {
            num = num * 16 + (cur - '0');
            bad = false;
            reader.skip(); continue;
          }
          else if (isalpha(cur)) {
            cur = tolower(cur);
            if (cur <= 'f') {
              num = num * 16 + (cur - 'a' + 10);
              bad = false;
              reader.skip(); continue;
            }
          }
          break;
        }
        if (bad) return false;
        tokens.push_back(num);
      }
      else {
        reader.ungetc(first);
        auto ele = parse_number(reader);
        if (!ele.first) return false;
        tokens.push_back(ele.second);
      }
    }
    else if (isalpha(reader.seek())) {
      string id = "";
      while (isalnum(reader.seek()) || reader.seek() == '_')
        id += reader.read();
      id += '\0';
      ReservedType t = resMatch.readFromStr(id.begin());
      if (t == Error) {
        u32 hsh = easy_hash(id.c_str()) & (ID_HASH_LENGTH - 1);
        while (id_hash[hsh] != nullptr && strcmp(id_hash[hsh], id.c_str()))
          hsh = (hsh + 1) & (ID_HASH_LENGTH - 1);
        if (id_hash[hsh] == nullptr) {
          char* str = new char[id.size()];
          memcpy(str, id.c_str(), id.size());
          id_hash[hsh] = str;
        }
        tokens.push_back(hsh);
      }
      else
        tokens.push_back(t);
    }
    else {
      bool checked = false;
      if (reader.seek() == '.') {
        reader.skip();
        if (isdigit(reader.seek())) {
          reader.ungetc('.');
          auto ele = parse_number(reader);
          if (!ele.first) return false;
          tokens.push_back(ele.second);
          checked = true;
        }
        else
          reader.ungetc('.');
      }
      if (!checked) {
        SymbolType t = symMatch.readFromReader(reader);
        if (t == ERROR)
          return false;
        if (t == COMMENT) {
          while (!reader.eof() && reader.seek() != '\n')
            reader.skip();
          continue;
        }
        tokens.push_back(t);
      }
    }
    if (tokens.back().type == Reserved)
      printf("[RESERVED] ");
    else if (tokens.back().type == Number)
      printf("[NUMBER] ");
    else if (tokens.back().type == String)
      printf("[STRING] ");
    else if (tokens.back().type == Symbol)
      printf("[SYMBOL] ");
    else if (tokens.back().type == Name)
      printf("[NAME] ");
    else if (tokens.back().type == Eol)
      printf("[EOL]");
    puts(tokens.back().type == Eol ? "" : cur_token_content.c_str());
  }
  return true;
}

int main() {
  Reader reader;
  if (!tokenize(reader))
    puts("Unable to tokenize to input.");
}