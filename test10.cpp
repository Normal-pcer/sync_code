
#if true
#include "./libs/debug_macros.hpp"

#endif
#include <bits/stdc++.h>
bool DEBUG_MODE=false;
#define debug if(DEBUG_MODE)
template <typename T> inline auto chkMax(T& base,const T& cmp) { return (base=max(base,cmp)); }
template <typename T> inline auto chkMin(T& base,const T& cmp) { return (base=min(base,cmp)); }
#define never if constexpr(0)
const int inf=0x3f3f3f3f; const long long infLL=0x3f3f3f3f3f3f3f3fLL; using ll=long long; using ull=unsigned long long;
const char endl='\n';
#define __lambda_1(expr) [&](){return expr;}
#define __lambda_2(a,expr) [&](auto a){return expr;}
#define __lambda_3(a,b,expr) [&](auto a,auto b){return expr;}
#define __lambda_4(a,b,c,expr) [&](auto a,auto b,auto c){return expr;}
#define __lambda_overload(a,b,c,d,e,args...) __lambda_##e
#define lambda(...) __lambda_overload(__VA_ARGS__,4,3,2,1)(__VA_ARGS__)
#define lam lambda
#define unreachable() (assert(false),__builtin_unreachable())
using namespace std;
#define endl "\n"
namespace lib{
#if __cplusplus > 201703LL
namespace ranges=ranges;
namespace views=views;
#endif
}
using namespace lib;
namespace IO {
struct EOFError: public exception {
const char *what() const throw() {
return "EOF when reading a char";
}
};
struct IntegerOverflowError: public exception {
const char *what() const throw() override {
return "Integer Overflow";
}
};
template <typename T> struct is_integral_or_int128 { constexpr static bool value=is_integral<T>::value; };
template <> struct is_integral_or_int128<__int128> { constexpr static bool value=true; };
template <> struct is_integral_or_int128<unsigned __int128> { constexpr static bool value=true; };
bool addOverflow(auto &x,auto y) {
return __builtin_add_overflow(x,y,&x);
}
bool subOverflow(auto &x,auto y) {
return __builtin_sub_overflow(x,y,&x);
}
bool mulOverflow(auto &x,auto y) {
return __builtin_mul_overflow(x,y,&x);
}
template <typename T,typename U>
bool leftShiftOverflow(T &x,U y) {
if (x==0) return false;
bool flag=__lg(x)+y >=numeric_limits<T>::digits;
return x <<=y,flag;
}
struct Scanner {
private:
char prev[2]={'\0','\0'};
int ungetFlag=0;
virtual char gc()=0;
static bool isDigit(char ch) { return '0' <=ch and ch <='9'; }
static bool isBlank(char ch) { return ch <=32 or ch==127; }
public:
char get() {
if (ungetFlag) {
return prev[--ungetFlag];
}
return (prev[1]=prev[0],prev[0]=gc());
}
Scanner &unget() {
if (ungetFlag==2) throw logic_error("Cannot unget twice");
ungetFlag++;
return *this;
}
template <typename T,typename enable_if<is_integral_or_int128<T>::value>::type* = nullptr,int base=10>
Scanner &read(T &x) {
bool sign=false; x=0; char ch=get();
for (; not isDigit(ch); ch=get()) {
if (ch=='-') sign=true;
}
if (sign) {
for (; isDigit(ch); ch=get()) {
if (mulOverflow(x,10)) throw IntegerOverflowError{};
if (subOverflow(x,ch ^ 48)) throw IntegerOverflowError{};
}
return unget(),*this;
} else {
for (; isDigit(ch); ch=get()) {
if (mulOverflow(x,10)) throw IntegerOverflowError{};
if (addOverflow(x,ch ^ 48)) throw IntegerOverflowError{};
}
return unget(),*this;
}
}
Scanner &read(char &x) {
for (x=get(); isBlank(x); x=get());
return *this;
}
Scanner &read(char *s) {
char ch=get();
for (; isBlank(ch); ch=get());
for (; not isBlank(ch); ch=get()) *s++=ch;
*s='\0';
return unget(),*this;
}
Scanner &read(string &s,int reserve=0) {
char ch=get();
s.clear(),s.reserve(reserve);
for (; isBlank(ch); ch=get());
for (; not isBlank(ch); ch=get()) s.push_back(ch); 
return unget(),*this;
}
template <typename T,typename enable_if<
is_integral_or_int128<T>::value || is_convertible<T,const char *>::value || is_convertible<T,string const &>::value
>::type* = nullptr
>
Scanner &operator>> (T &x) {
return read(x);
}
};
struct GetCharScanner: public Scanner {
private:
char gc_getchar() {
char ch=getchar();
if (ch !=EOF) return ch;
else {
throw EOFError{};
return '\0';
}
}
protected:
char gc() { return gc_getchar(); }
};
struct Printer {
virtual void put(char)=0;
virtual void flush() {}
template <typename T,typename enable_if<is_integral_or_int128<T>::value>::type* = nullptr>
Printer &write(T x) {
static char st[numeric_limits<T>::digits10+1];
char *top=st;
if (x < 0) {
put('-');
*top++=-(x % 10) ^ 48,x=-(x / 10);
if (x==0) {
put(*--top);
return *this;
}
}
do {
*top++=x % 10 ^ 48,x /=10;
} while (x);
while (top !=st) put(*--top);
return *this;
}
Printer &write(char ch) {
put(ch);
return *this;
}
Printer &write(const char *s) {
for (; *s; s++) put(*s);
return *this;
}
Printer &write(string const &s) {
for (auto ch: s) put(ch);
return *this;
}
template <typename T,typename enable_if<
is_integral_or_int128<T>::value || is_convertible<T,const char *>::value || is_convertible<T,string const &>::value
>::type* = nullptr>
Printer &operator<< (const T &x) {
if constexpr (
is_integral_or_int128<T>::value || is_convertible<T,const char *>::value || is_convertible<T,string const &>::value
) {
return write(x);
}
}
};
struct PutCharPrinter: public Printer {
void put(char ch) {
putchar(ch);
}
};
struct IO: public Scanner,public Printer {};
struct DefaultIO: public GetCharScanner,PutCharPrinter {};
DefaultIO io;
}
using IO::io;
using i32=int32_t; using i64=int64_t; using u32=uint32_t; using u64=uint64_t;
using i128=__int128; using u128=unsigned __int128;
using f32=float; using f64=double;
namespace FutureProgram {
struct StringScanner: public IO::Scanner {
string s;
size_t index;
bool eofFlag=false;
StringScanner(): s(),index(0) {}
StringScanner(string &&s): s(s),index(0) {}
StringScanner(string const &s): s(s),index(0) {}
char gc() override {
if (index < s.size()) eofFlag=false;
if (index==s.size()) {
if (eofFlag) throw IO::EOFError{};
else eofFlag=true;
return '\0';
}
return index==s.size()? '\0': s[index++];
}
} input;
namespace Trie {
class Trie {
public:
struct Node {
array<Node *,96> next;
int match;
};
deque<Node> nodes;
Node *root;
Trie(): nodes(),root(nullptr) {
nodes.push_back({});
root=&nodes.back();
}
Trie(initializer_list<string> &&init): nodes(),root(nullptr) {
nodes.push_back({});
root=&nodes.back();
for (const auto &s: init) insert(s);
}
int match(const string &s) const {
Node *cur=root;
for (auto ch: s) {
assert(ch > 32),ch-=32;
if (cur->next[ch]) cur=cur->next[ch];
else return 0;
}
return cur->match;
}
void insert(const string &s) {
Node *cur=root;
for (auto ch: s) {
assert(ch > 32),ch-=32;
cur->match++;
if (cur->next[ch]) cur=cur->next[ch];
else nodes.push_back({}),cur=cur->next[ch]=&nodes.back();
}
cur->match++;
}
};
}
using IdentifierIndexType=int;
class IdentifierMap {
struct IndexError: public exception {
const char *err=nullptr;
IndexError(const char *err): err(err) {}
const char *what() const noexcept override {
return err;
}
};
unordered_map<string,IdentifierIndexType> mappingStringToIndex;
unordered_map<int,string> mappingIndexToString;
public:
IdentifierMap();
IdentifierIndexType getIndex(string const &s,IdentifierIndexType default_val) {
auto it=mappingStringToIndex.find(s);
if (it !=mappingStringToIndex.end()) return it->second;
else return default_val;
}
IdentifierIndexType getIndex(string const &s) {
auto it=mappingStringToIndex.find(s);
if (it !=mappingStringToIndex.end()) return it->second;
else throw IndexError("IdentifierMap::getIndex: name not found");
}
const string &getString(IdentifierIndexType index) {
auto it=mappingIndexToString.find(index);
if (it !=mappingIndexToString.end()) return it->second;
else throw IndexError("IdentifierMap::getString: index out of range");
}
IdentifierIndexType insert(string const &s) {
auto index=mappingIndexToString.size()+1;
while (containsIndex(index)) index++;
auto [new_it,success]=mappingStringToIndex.insert({s,index});
if (success) mappingIndexToString.insert({index,s});
return new_it->second;
}
bool containsString(string const &s) {
return mappingStringToIndex.find(s) !=mappingStringToIndex.end();
}
bool containsIndex(IdentifierIndexType index) {
return mappingIndexToString.find(index) !=mappingIndexToString.end();
}
void join(IdentifierIndexType index,const string &s) {
assert(not containsIndex(index) and not containsString(s));
mappingStringToIndex.insert({s,index});
mappingIndexToString.insert({index,s});
}
};
namespace Keywords {
enum KeywordType: int {
None,If,While,For,Var,Def,Return,Else,Main,Using
};
void joinKeywords(IdentifierMap &idMap) {
idMap.join(If,"if");
idMap.join(While,"while");
idMap.join(For,"for");
idMap.join(Var,"var");
idMap.join(Def,"def");
idMap.join(Return,"return");
idMap.join(Else,"else");
idMap.join(Main,"main");
idMap.join(Using,"using");
}
}
IdentifierMap::IdentifierMap(): mappingStringToIndex(),mappingIndexToString() {
Keywords::joinKeywords(*this);
}
IdentifierMap identifierMap;
struct Program;
void compileError(string const &s) {
cerr << s << endl;
exit(-1);
}
class Tokenizer {
public:
static bool isBlank(char ch) {
return ch <=32 or ch==127;
}
static bool isIdentifierStart(char ch) {
return ('A' <=ch and ch <='Z') or ('a' <=ch and ch <='z') or ch=='_';
}
static bool isDigit(char ch) {
return '0' <=ch and ch <='9';
}
inline static Trie::Trie symbols {
"<=",">=","!=","==","<<",">>","<=>","&&","||","+=","-=","*=","/=","%=","|=","&=","^=","->","++","--",".."
};
struct ParseAble {
virtual void parse(IO::Scanner &io)=0;
friend IO::Scanner &operator>>(IO::Scanner &io,ParseAble &pa) {
return pa.parse(io),io;
}
};
struct DumpAble {
virtual void dump(IO::Printer &io) const=0;
friend IO::Printer &operator<<(IO::Printer &io,const DumpAble &da) {
return da.dump(io),io;
}
};
struct Identifier: public ParseAble,public DumpAble {
IdentifierIndexType name;
Identifier(): name() {}
Identifier(IdentifierIndexType name): name(name) {}
Identifier(const string &str_name) {
auto index=identifierMap.insert(str_name);
name=index;
}
void parse(IO::Scanner &io) {
string str_name; 
char ch=io.get();
for (; not isIdentifierStart(ch); ch=io.get());
for (; isIdentifierStart(ch) or isDigit(ch); ch=io.get()) {
str_name.push_back(ch);
}
io.unget();
auto index=identifierMap.insert(str_name);
name=index;
}
void dump(IO::Printer &io) const override {
io << name;
}
strong_ordering operator<=> (Identifier const &other) const {
return name <=> other.name;
}
bool operator==(string const &other) const {
return identifierMap.getString(name)==other;
}
bool operator==(IdentifierIndexType const &x) const {
return name==x;
}
};
struct Integer: public ParseAble,public DumpAble {
u64 value;
Integer(): value(0) {}
Integer(u64 value): value(value) {}
void parse(IO::Scanner &io) override {
value=0;
char ch=io.get();
try {
if (ch=='0') {
ch=io.get();
if (ch=='x' or ch=='X') {
for (ch=io.get(); isDigit(ch) or ('A' <=ch and ch <='F') or ('a' <=ch and ch <='f') or ch=='\''; ch=io.get()) {
if (ch=='\'') continue;
if (IO::leftShiftOverflow(value,4)) throw IO::IntegerOverflowError{};
auto cur=ch <='9'? ch ^ 48: (ch & 15)+9;
if (IO::addOverflow(value,cur)) throw IO::IntegerOverflowError{};
}
io.unget();
} else if (ch=='b' or ch=='B') {
for (ch=io.get(); ch=='0' or ch=='1' or ch=='\''; ch=io.get()) {
if (ch=='\'') continue;
if (IO::leftShiftOverflow(value,1)) throw IO::IntegerOverflowError{};
if (IO::addOverflow(value,ch ^ 48)) throw IO::IntegerOverflowError{};
}
io.unget();
} else if (ch=='o' or ch=='O') {
for (ch=io.get(); ('0' <=ch and ch <='7') or ch=='\''; ch=io.get()) {
if (ch=='\'') continue;
if (IO::leftShiftOverflow(value,3)) throw IO::IntegerOverflowError{};
if (IO::addOverflow(value,ch ^ 48)) throw IO::IntegerOverflowError{};
}
io.unget();
} else {
goto egg; 
}
} else {
egg:
for (; isDigit(ch) or ch=='\''; ch=io.get()) {
if (ch=='\'') continue;
if (IO::mulOverflow(value,10)) throw IO::IntegerOverflowError{};
if (IO::addOverflow(value,ch ^ 48)) throw IO::IntegerOverflowError{};
}
io.unget();
}
} catch (IO::IntegerOverflowError &) {
compileError("Number is too large");
}
}
void dump(IO::Printer &io) const override {
io << value;
}
};
struct Symbol: public ParseAble,public DumpAble {
string value;
Symbol(): value({}) {}
Symbol(string value): value(value) {}
void parse(IO::Scanner &io) {
value.clear();
char ch=io.get();
value.push_back(ch);
auto *cur=symbols.root->next[ch-32];
bool flag=false;
while (cur) {
ch=io.get(),flag=true;
if (isBlank(ch) or isDigit(ch) or isIdentifierStart(ch)) break;
if (cur->next[ch-32]) cur=cur->next[ch-32];
else break;
value.push_back(ch);
}
if (flag) io.unget();
}
void dump(IO::Printer &io) const override {
io << value;
}
};
struct Token {
enum Tag {
NoneTag,IdentifierTag,SymbolTag,KeywordTag,IntegerTag,StringTag,EndOfLineTag,FloatingPointTag
} tag=NoneTag;
variant<int,Identifier,Integer,Symbol> value=0;
friend IO::Printer &operator<<(IO::Printer &io,const Token &token) {
switch (token.tag) {
case Token::IdentifierTag:
return io << "Identifier_" << get<Identifier>(token.value);
case Token::IntegerTag:
return io << "Integer" << get<Integer>(token.value);
case Token::SymbolTag:
return io << "Symbol" << get<Symbol>(token.value);
case Token::EndOfLineTag:
return io << "EOL";
default:
return io << "Unknown";
}
}
};
static vector<Token> tokenize(IO::Scanner &io) {
vector<Token> tokens;
try {
while (true) {
char ch=io.get();
io.unget();
if (ch=='\0') {
break;
} if (ch=='\n') {
io.get();
} else if (isBlank(ch)) {
io.get();
} else if (isDigit(ch)) {
Integer integer;
io >> integer;
tokens.push_back({Token::IntegerTag,integer});
} else if (isIdentifierStart(ch)) {
Identifier identifier;
io >> identifier;
tokens.push_back({Token::IdentifierTag,identifier});
} else {
Symbol symbol;
io >> symbol;
if (symbol.value=="#") {
while (io.get() !='\n');
continue; 
}
tokens.push_back({Token::SymbolTag,symbol});
}
}
} catch (IO::EOFError &) {}
tokens.push_back({Token::EndOfLineTag});
return tokens;
}
};
using Token=Tokenizer::Token;
using Identifier=Tokenizer::Identifier;
using Symbol=Tokenizer::Symbol;
#define PARSER_DEBUG_MESSAGE
namespace AST {
using TokensType=const vector<Token>;
using TokenIterator=TokensType::const_iterator;
using TokensSubrange=ranges::subrange<TokenIterator>;
struct Node {
Node()=default;
Node(const Node &)=delete;
virtual ~Node()=default;
Node &operator=(const Node &)=delete;
};
template <typename T>
struct ParseResult {
T *node;
TokenIterator it;
template <typename U>
operator ParseResult<U> () const {
return {static_cast<U *>(node),it};
}
};
struct StatementNode: public Node {
enum Type {
NoneStatement,ExpressionEvaluateStatement,RunBlockStatement,IfStatement,WhileStatement,ForStatement,ReturnStatement,DeclareStatement,} type=NoneStatement;
StatementNode(Type type=NoneStatement): type(type) {}
virtual ~StatementNode()=default;
template <typename T>
static ParseResult<StatementNode> parse(const T &);
};
struct BlockNode: public Node {
enum Type {
NoneBlock,} type=NoneBlock;
vector<StatementNode *> statements;
BlockNode()=default;
BlockNode(Type type): type(type),statements() {}
virtual ~BlockNode() {
for (auto &statement: statements) delete statement;
}
template <typename T>
static ParseResult<BlockNode> parse(const T &src);
};
struct ExpressionNode: public Node {
enum Operator {
NoneOp,Bracket,Call,FunctionArgsBracket,Subscript,SubscriptBracket,SplitComma,UnaryAdd,UnarySub,Add,Sub,Mul,Div,Mod,Less,LessEqual,Greater,GreaterEqual,Equal,NotEqual,And,Or,Not,BitAnd,BitOr,BitXor,BitNot,BitShiftLeft,BitShiftRight,Assign,Range,MemberAccess,} op=NoneOp;
static constexpr const char *opNames[]={"NoneOp","Bracket","Call","FunctionArgsBracket","Subscript","SubscriptBracket","SplitComma","UnaryAdd","UnarySub","Add","Sub","Mul","Div","Mod","Less","LessEqual","Greater","GreaterEqual","Equal","NotEqual","And","Or","Not","BitAnd","BitOr","BitXor","BitNot","BitShiftLeft","BitShiftRight","Assign","Range","MemberAccess"};
ExpressionNode *left=nullptr,*right=nullptr;
ExpressionNode(Operator op=NoneOp): op(op) {}
virtual ~ExpressionNode();
struct OperatorInfo {
static constexpr const int priority_max=0x3f3f3f3f;
int priority=0; 
bool leftAssociative=false; 
};
static constexpr OperatorInfo infoOf(Operator op) {
switch (op) {
case Call: return {2,false};
case Subscript: return {2,false};
case SplitComma: return {17,false};
case UnaryAdd: return {3,true};
case UnarySub: return {3,true};
case Add: return {6,false};
case Sub: return {6,false};
case Mul: return {5,false};
case Div: return {5,false};
case Mod: return {5,false};
case Less: return {9,false};
case LessEqual: return {9,false};
case Greater: return {9,false};
case GreaterEqual: return {9,false};
case Equal: return {10,false};
case NotEqual: return {10,false};
case And: return {14,false};
case Or: return {15,false};
case Not: return {3,true};
case BitAnd: return {11,false};
case BitOr: return {13,false};
case BitXor: return {12,false};
case BitNot: return {3,true};
case BitShiftLeft: return {17,false};
case BitShiftRight: return {17,false};
case Assign: return {16,true};
case MemberAccess: return {2,false};
case Range: return {14,false};
default: return {OperatorInfo::priority_max,false};
}
}
template <typename T,typename PredType>
static ParseResult<ExpressionNode> parse(const T &,PredType &&);
template <typename T>
static ParseResult<ExpressionNode> parse(const T &);
};
struct ValueNode: public ExpressionNode {
enum Type {
NoneValue,Integer,FloatingPoint,String,Identifier
} type=NoneValue;
Token token;
ValueNode(Type type,Token token): 
ExpressionNode({ExpressionNode::NoneOp}),type(type),token(token) {}
ValueNode(const ValueNode &other): 
ExpressionNode({ExpressionNode::NoneOp}),type(other.type),token(other.token) {}
ValueNode(Token token): ExpressionNode({ExpressionNode::NoneOp}),type(NoneValue),token(token) {
type=[&]() {
switch (token.tag) {
case Token::IntegerTag: return Integer;
case Token::FloatingPointTag: return FloatingPoint;
case Token::StringTag: return String;
case Token::IdentifierTag: return Identifier;
default: return unreachable(),NoneValue;
}
}();
}
};
struct ExpressionEvaluateStatementNode: public StatementNode {
ExpressionNode *expr;
ExpressionEvaluateStatementNode(ExpressionNode *expr):
StatementNode(StatementNode::ExpressionEvaluateStatement),expr(expr) {}
~ExpressionEvaluateStatementNode() {
delete expr;
}
};
struct DeclareStatementNode: public StatementNode {
enum DeclareType {
Variable,Function 
} declareType;
Identifier typeName; 
DeclareStatementNode(DeclareType declareType,Identifier typeName): 
StatementNode(StatementNode::DeclareStatement),declareType(declareType),typeName(typeName) {}
DeclareStatementNode(const DeclareStatementNode &)=delete;
virtual ~DeclareStatementNode() {}
template <typename T>
static ParseResult<DeclareStatementNode> parse(const T &);
};
struct VariableDeclareStatementNode: public DeclareStatementNode {
struct SingleDeclare {
Identifier name;
ExpressionNode *initializer=nullptr;
vector<ExpressionNode *> arraySize; 
SingleDeclare(Identifier name,ExpressionNode *initializer=nullptr,vector<ExpressionNode *> arraySize={}): 
name(name),initializer(initializer),arraySize(arraySize) {}
SingleDeclare(const SingleDeclare &)=delete;
~SingleDeclare() {
delete initializer;
for (auto &size: arraySize) delete size;
}
};
vector<SingleDeclare *> declares;
VariableDeclareStatementNode(Identifier typeName): DeclareStatementNode(Variable,typeName) {};
~VariableDeclareStatementNode() {
for (auto &declare: declares) delete declare;
}
};
struct FunctionDeclareStatementNode: public DeclareStatementNode {
struct Argument {
Identifier typeName;
Identifier name;
};
FunctionDeclareStatementNode(Identifier typeName): DeclareStatementNode(Function,typeName) {}
Identifier name;
vector<Argument> args;
BlockNode *body;
~FunctionDeclareStatementNode() {
delete body;
}
};
struct RunBlockStatementNode: public StatementNode {
BlockNode *block;
RunBlockStatementNode(BlockNode *block): StatementNode(StatementNode::RunBlockStatement),block(block) {}
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
template <typename T,typename Out>
static ParseResult<Out> parse(const T &);
};
struct IfStatementNode: public ConditionalStatementNode {
BlockNode *elseBody=nullptr; 
IfStatementNode(): ConditionalStatementNode(StatementNode::IfStatement) {}
~IfStatementNode() {
delete elseBody;
}
template <typename T>
static ParseResult<IfStatementNode> parse(const T &src) {
return ConditionalStatementNode::parse<decltype(src),IfStatementNode>(src);
}
};
struct WhileStatementNode: public ConditionalStatementNode {
WhileStatementNode(): ConditionalStatementNode(StatementNode::WhileStatement) {}
template <typename T>
static ParseResult<WhileStatementNode> parse(const T &src) {
return ConditionalStatementNode::parse<decltype(src),WhileStatementNode>(src);
}
};
struct ForStatementNode: public StatementNode {
StatementNode *init;
ExpressionNode *condition,*step;
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
struct ReturnStatementNode: public StatementNode {
ExpressionNode *expr=nullptr;
ReturnStatementNode(): StatementNode(StatementNode::ReturnStatement) {}
~ReturnStatementNode() {
delete expr;
}
template <typename T>
static ParseResult<ReturnStatementNode> parse(const T &);
};

template <typename T>
ParseResult<DeclareStatementNode> DeclareStatementNode::parse(const T &src) {
DeclareStatementNode *res=nullptr;
auto it=src.begin();
assert(it->tag==Token::IdentifierTag);
auto type_name=get<Identifier>(it->value);
it++;
assert(it->tag==Token::IdentifierTag);
auto next_identifier=get<Identifier>(it->value);
it++;
auto try_read_array_size=[&]()-> vector<ExpressionNode *> {
vector<ExpressionNode *> res;
while (it->tag==Token::SymbolTag and get<Symbol>(it->value).value=="[") {
auto end_condition=[](auto const &op,auto const &stack)-> bool {
if (op=="]") {
bool flag=true;
for (auto it=stack.begin(); it !=stack.end(); it++) {
if (it->op==ExpressionNode::SubscriptBracket) {
flag=false;
break;
}
}
return flag;
}
return false;
};
auto [expr,end]=ExpressionNode::parse(TokensSubrange{++it,src.end()},end_condition);
it=end;
res.push_back(expr);
}
return res;
};
auto first_array_size=try_read_array_size();
ExpressionNode *first_initializer=nullptr;
auto try_read_initializer=[&]()-> ExpressionNode * {
if (it->tag==Token::SymbolTag and get<Symbol>(it->value).value=="=") {
auto end_condition=[](auto const &op,auto const &stack)-> bool {
if (op==";" or op=="\n") return true;
if (op==",") {
bool flag=true;
for (auto it=stack.begin(); it !=stack.end(); it++) {
if (it->op==ExpressionNode::Bracket) {
flag=false;
break;
}
}
return flag;
}
return false;
};
auto [expr,end]=ExpressionNode::parse(TokensSubrange{++it,src.end()},end_condition);
it=prev(end);
return expr;
}
return nullptr;
};
first_initializer=try_read_initializer();
if (it->tag==Token::SymbolTag and get<Symbol>(it->value).value==",") {
it++;
auto var_declare=new VariableDeclareStatementNode{type_name};
delete res,res=var_declare;
var_declare->declares.push_back(new VariableDeclareStatementNode::SingleDeclare{next_identifier,first_initializer,first_array_size});
while (true) {
assert(it->tag==Token::IdentifierTag);
auto name=get<Identifier>(it->value);
it++;
auto array_size=try_read_array_size();
auto init=try_read_initializer();
var_declare->declares.push_back(new VariableDeclareStatementNode::SingleDeclare{name,init,array_size});
if (it->tag==Token::SymbolTag and get<Symbol>(it->value).value==",") {
it++; continue;
} else {
break; 
}
}
} else if (it->tag==Token::SymbolTag and get<Symbol>(it->value).value=="(") {
assert(first_initializer==nullptr);
auto func_declare=new FunctionDeclareStatementNode{type_name};
delete res,res=func_declare;
func_declare->name=next_identifier;++it;
while (true) {
if (it->tag==Token::SymbolTag and get<Symbol>(it->value).value==")") {
break;
}
assert(it->tag==Token::IdentifierTag);
auto type_name=get<Identifier>(it->value); 
it++;
assert(it->tag==Token::IdentifierTag);
auto name=get<Identifier>(it->value);
it++;
func_declare->args.push_back({type_name,name});
if (it->tag==Token::SymbolTag and get<Symbol>(it->value).value==",") {
it++;
continue;
}
}
it++;
assert(it->tag==Token::SymbolTag and get<Symbol>(it->value).value=="{");
auto [body,end]=BlockNode::parse(TokensSubrange{++it,src.end()});
it=prev(end); 
func_declare->body=body;
} else {
auto var_declare=new VariableDeclareStatementNode{type_name};
delete res,res=var_declare;
var_declare->declares.push_back(new VariableDeclareStatementNode::SingleDeclare{next_identifier,first_initializer,first_array_size});
}
return {res,++it};
}

template <typename T>
ParseResult<ReturnStatementNode> ReturnStatementNode::parse(const T &src) {
auto res=new ReturnStatementNode;
auto it=src.begin();
if (it->tag==Token::SymbolTag and get<Symbol>(it->value).value==";") return {res,++it};
if (it->tag==Token::EndOfLineTag) return {res,it};
auto [expr,end]=ExpressionNode::parse(TokensSubrange{it,src.end()});
res->expr=expr;
it=end;
return {res,it};
}

template <typename T>
ParseResult<BlockNode> BlockNode::parse(T const &src) {
auto res=new BlockNode;
auto it=src.begin();
for (; it !=src.end();) {
if (it->tag==Token::SymbolTag and get<Symbol>(it->value).value=="}") return {res,++it};
auto [statement,next]=StatementNode::parse(TokensSubrange{it,src.end()});
if (statement !=nullptr) {
res->statements.push_back(statement);
}
it=next;
}
return {res,src.end()};
}

template <typename T>
ParseResult<StatementNode> StatementNode::parse(const T &src) {
PARSER_DEBUG_MESSAGE;
auto it=src.begin();
if (it->tag==Token::EndOfLineTag) {
it++;
return {nullptr,it};
}
if (it->tag==Token::SymbolTag and get<Symbol>(it->value).value=="{") {
auto [sub_block,next]=BlockNode::parse(TokensSubrange{++it,src.end()});
return {new RunBlockStatementNode{sub_block},next};
}
if (it->tag==Token::IdentifierTag and get<Identifier>(it->value)==Keywords::Using) {
it+=3;
assert(it->tag==Token::SymbolTag and get<Symbol>(it->value).value==";");
it++;
return {nullptr,it};
}
if (it->tag==Token::IdentifierTag and get<Identifier>(it->value)==Keywords::If) {
auto [if_statement,next]=IfStatementNode::parse(TokensSubrange{++it,src.end()});
it=next;
if (it !=src.end()) {
while (it !=src.end() and it->tag==Token::EndOfLineTag) it++;
if (it==src.end()) goto ignore_else;
if (it->tag==Token::IdentifierTag and get<Identifier>(it->value)==Keywords::Else) {
it++;
while (it->tag==Token::EndOfLineTag) it++;
if (it->tag==Token::SymbolTag and get<Symbol>(it->value).value=="{") {
auto [else_block,next]=BlockNode::parse(TokensSubrange{++it,src.end()});
if_statement->elseBody=else_block,it=next;
} else {
auto [else_statement,next]=StatementNode::parse(TokensSubrange{it,src.end()});
auto else_block=new BlockNode{};
else_block->statements.push_back(else_statement);
if_statement->elseBody=else_block,it=next;
}
}
}
ignore_else:
return {if_statement,it};
}
if (it->tag==Token::IdentifierTag and get<Identifier>(it->value)==Keywords::While) {
return WhileStatementNode::parse(TokensSubrange{++it,src.end()});
}
if (it->tag==Token::IdentifierTag and get<Identifier>(it->value)==Keywords::For) {
return ForStatementNode::parse(TokensSubrange{++it,src.end()});
}
if (it->tag==Token::IdentifierTag and get<Identifier>(it->value)==Keywords::Var) {
return VariableDeclareStatementNode::parse(TokensSubrange{++it,src.end()});
}
if (it->tag==Token::IdentifierTag and get<Identifier>(it->value)==Keywords::Def) {
return FunctionDeclareStatementNode::parse(TokensSubrange{++it,src.end()});
}
if (it->tag==Token::IdentifierTag and get<Identifier>(it->value)==Keywords::Return) {
return ReturnStatementNode::parse(TokensSubrange{++it,src.end()});
}
if (it->tag==Token::IdentifierTag and next(it) !=src.end() and next(it)->tag==Token::IdentifierTag) {
return VariableDeclareStatementNode::parse(TokensSubrange{it,src.end()});
}
if (it->tag==Token::SymbolTag and get<Symbol>(it->value).value==";") {
return {new StatementNode,++it};
}
auto [expr,next]=ExpressionNode::parse(TokensSubrange{it,src.end()});
return {new ExpressionEvaluateStatementNode{expr},next};
}

template <typename T,typename Out>
ParseResult<Out> ConditionalStatementNode::parse(const T &src) {
auto res=new Out;
auto it=src.begin();
 {
auto end_it=it;
auto cnt=0; 
for (; end_it !=src.end(); end_it++) {
if (end_it->tag==Token::SymbolTag) {
if (get<Symbol>(end_it->value).value=="(") {
cnt++;
} else if (get<Symbol>(end_it->value).value==")") {
cnt--;
if (cnt==0) {
end_it++;
break;
}
}
}
}
auto [condition,next]=ExpressionNode::parse(TokensSubrange{it,end_it});
res->condition=condition;
it=next;
}
 {
assert(it !=src.end());
if (it->tag==Token::SymbolTag and get<Symbol>(it->value).value=="{") {
auto [body,next]=BlockNode::parse(TokensSubrange{++it,src.end()});
res->body=body;
it=next;
} else {
auto [body,next]=StatementNode::parse(TokensSubrange{it,src.end()});
res->body=new BlockNode;
res->body->statements.push_back(body);
it=next;
}
}
return {res,it};
}

template <typename T>
ParseResult<ForStatementNode> ForStatementNode::parse(const T &src) {
auto res=new ForStatementNode;
auto it=src.begin();
assert(it !=src.end() and it->tag==Token::SymbolTag and get<Symbol>(it->value).value=="(");
it++;
{
auto [expr,next]=StatementNode::parse(TokensSubrange{it,src.end()});
res->init=expr;
it=next;
}
{
auto [expr,next]=ExpressionNode::parse(TokensSubrange{it,src.end()});
res->condition=expr,it=next;
}
{
auto end_condition=[](auto const &op,auto const &ops_stack)-> bool {
if (op==")") {
bool flag=true;
for (auto it=ops_stack.rbegin(); it !=ops_stack.rend(); it++) {
if (it->op==ExpressionNode::Bracket) {
flag=false;
}
}
if (flag) {
return true;
}
}
return false;
};
auto [expr,next]=ExpressionNode::parse(TokensSubrange{it,src.end()},end_condition);
res->step=expr,it=next;
}
{
if (it->tag==Token::SymbolTag and get<Symbol>(it->value).value=="{") {
auto [body,next]=BlockNode::parse(TokensSubrange{++it,src.end()});
res->body=body,it=next;
} else {
auto [body,next]=StatementNode::parse(TokensSubrange{it,src.end()});
res->body=new BlockNode;
res->body->statements.push_back(body);
it=next;
}
}
return {res,it};
}
ExpressionNode::~ExpressionNode() {
if (left) delete left;
if (right) delete right;
}
template <typename T>
ParseResult<ExpressionNode> ExpressionNode::parse(const T &src) {
return parse(src,0);
}
template <typename T,typename PredType>
ParseResult<ExpressionNode> ExpressionNode::parse(const T &src,PredType &&pred) {
struct StackValueType {
Operator op;
int args_remains; 
};
const auto &&end_condition=[&]() {
if constexpr(is_same_v<PredType,int>) {
return [&](string const &op,vector<StackValueType> const &ops) {
if (op=="," and (ops.empty() or ops.back().op==NoneOp)) return true;
if (op==";" or op=="\n") return true;
return false;
};
} else {
return pred;
}
}();
auto [postfix,it]=[&]() {
const auto inf=OperatorInfo::priority_max;
vector<StackValueType> ops {{NoneOp,1}}; 
auto it=src.begin();
struct PostfixValueType {
bool symbol=false;
variant<Operator,ValueNode> item;
};
vector<PostfixValueType> postfix;
auto add=[&](Operator type,int args_remains) {
if (infoOf(type).leftAssociative) {
while (not ops.empty() and infoOf(ops.back().op).priority < infoOf(type).priority) postfix.push_back({true,{ops.back().op}}),ops.pop_back();
} else {
while (not ops.empty() and infoOf(ops.back().op).priority <=infoOf(type).priority) postfix.push_back({true,{ops.back().op}}),ops.pop_back();
}
ops.push_back({type,args_remains});
};
for (; it !=src.end(); it++) {
auto &token=*it;
if (token.tag==Token::SymbolTag or token.tag==Token::EndOfLineTag) {
auto op=token.tag==Token::EndOfLineTag? "\n": get<Symbol>(token.value).value;
if (end_condition(op,ops)) {
it++;
break;
}
if (op=="(") {
if (not ops.empty() and ops.back().args_remains==0) {
add(Call,0);
ops.push_back({FunctionArgsBracket,1});
} else {
ops.back().args_remains--;
ops.push_back({Bracket,1});
}
} else if (op==")") {
while (infoOf(ops.back().op).priority !=inf) {
postfix.push_back({true,ops.back().op}),ops.pop_back();
}
if (ops.back().op==FunctionArgsBracket) {
bool flag=ops.back().args_remains==1;
ops.pop_back();
if (flag) postfix.push_back({false,ValueNode{ValueNode::NoneValue,Token{Token::NoneTag}}});
} else {
assert(ops.back().op==Bracket and ops.back().args_remains==0);
ops.pop_back();
}
} else if (op==",") {
while (not ops.empty() and infoOf(ops.back().op).priority < infoOf(SplitComma).priority) {
postfix.push_back({true,{ops.back().op}}),ops.pop_back();
}
ops.push_back({SplitComma,1});
} else if (op=="[") {
add(Subscript,0);
ops.push_back({SubscriptBracket,1});
} else if (op=="]") {
while (infoOf(ops.back().op).priority !=inf) {
postfix.push_back({true,ops.back().op}),ops.pop_back();
}
assert(ops.back().op==SubscriptBracket);
bool flag=ops.back().args_remains==1;
ops.pop_back();
if (flag) postfix.push_back({false,ValueNode{ValueNode::NoneValue,Token{Token::NoneTag}}});
} else if (op=="+") {
if (not ops.empty() and ops.back().args_remains !=0) {
ops.back().args_remains--;
add(UnaryAdd,1);
} else {
add(Add,1);
}
} else if (op=="-") {
if (not ops.empty() and ops.back().args_remains !=0) {
ops.back().args_remains--;
add(UnarySub,1);
} else {
add(Sub,1);
}
}
#define JOIN_BINARY_OP(op_type,op_str) else if (op==op_str) add(op_type,1);
JOIN_BINARY_OP(Mul,"*")
JOIN_BINARY_OP(Div,"/")
JOIN_BINARY_OP(Mod,"%")
JOIN_BINARY_OP(Less,"<")
JOIN_BINARY_OP(Greater,">")
JOIN_BINARY_OP(LessEqual,"<=")
JOIN_BINARY_OP(GreaterEqual,">=")
JOIN_BINARY_OP(Equal,"==")
JOIN_BINARY_OP(NotEqual,"!=")
JOIN_BINARY_OP(And,"&&")
JOIN_BINARY_OP(Or,"||")
JOIN_BINARY_OP(BitAnd,"&")
JOIN_BINARY_OP(BitOr,"|")
JOIN_BINARY_OP(BitXor,"^")
JOIN_BINARY_OP(BitShiftLeft,"<<")
JOIN_BINARY_OP(BitShiftRight,">>")
JOIN_BINARY_OP(Assign,"=")
JOIN_BINARY_OP(MemberAccess,".")
JOIN_BINARY_OP(Range,"..")
#undef JOIN_BINARY_OP
else if (op=="!") {
ops.back().args_remains--;
add(Not,1);
} else if (op=="~") {
ops.back().args_remains--;
add(BitNot,1);
} else {
cerr << "Unknown symbol: " << op << endl;
throw-1;
}
} else {
if (token.tag==Token::IdentifierTag) {
postfix.push_back({false,ValueNode{ValueNode::Identifier,token}});
} else if (token.tag==Token::IntegerTag) {
postfix.push_back({false,ValueNode{ValueNode::Integer,token}});
}
ops.back().args_remains--;
assert(ops.back().args_remains==0);
}
}
while (not ops.empty() and ops.size() !=(size_t)1) {
postfix.push_back({true,ops.back().op});
ops.pop_back();
}
return pair{postfix,it};
}();
vector<AST::ExpressionNode *> nodes_stack;
auto countOf=[&](Operator op) {
switch (op) {
case UnaryAdd: return 1;
case UnarySub: return 1;
case Not: return 1;
case BitNot: return 1;
default: return 2;
}
}; 
for (auto &x: postfix) {
if (x.symbol) {
auto count=countOf(get<Operator>(x.item));
ExpressionNode *l_son=nullptr,*r_son=nullptr;
if (count==2) {
r_son=nodes_stack.back();
nodes_stack.pop_back();
l_son=nodes_stack.back();
nodes_stack.pop_back();
} else {
l_son=nodes_stack.back();
nodes_stack.pop_back();
}
auto *node=new ExpressionNode{get<Operator>(x.item)};
node->left=l_son,node->right=r_son;
nodes_stack.push_back(node);
} else {
auto *node=new ValueNode{get<ValueNode>(x.item)};
nodes_stack.push_back(node);
}
}
assert(nodes_stack.size()==(size_t)1);
return {nodes_stack.back(),it};
}
}
struct Program {
AST::BlockNode *root;
IdentifierMap identifierMap;
Program(AST::BlockNode *root,IdentifierMap identifierMap) : root(root),identifierMap(identifierMap) {}
Program(Program &&other): root(other.root),identifierMap(move(other.identifierMap)) {
other.root=nullptr;
}
~Program() {
if (root !=nullptr) {
delete root;
}
}
};
namespace Interpreter {
struct ArrayObjectValue;
struct Object {
enum Type {
Struct,Int,Function,String,None,Array,BuiltinFunction,BuiltinIStream,BuiltinOStream,BuiltinEndl
} type;
variant<
nullptr_t,int,shared_ptr<string>,shared_ptr<Identifier>,shared_ptr<ArrayObjectValue>,AST::FunctionDeclareStatementNode *
> value;
Object(Type type=None): type(type),value(nullptr) {}
template <typename T>
Object(Type type,const T &value): type(type),value(value) {}
Object copy() {
if (type==Int) {
return Object{Int,get<int>(value)};
} else {
unreachable();
return Object{};
}
}
};
struct ArrayMeta;
struct TypeName {
enum Type {
Undefined,Int,Array,Function,Struct
} type;
variant<nullptr_t,shared_ptr<ArrayMeta>> meta;
TypeName(Type type=Undefined): type(type),meta(nullptr) {}
TypeName(Type type,const auto &meta): type(type),meta(meta) {}
TypeName(TypeName const &other): type(other.type),meta(other.meta) {}
TypeName &operator=(TypeName const &other) {
type=other.type;
meta=other.meta;
return *this;
}
};
struct ArrayMeta {
TypeName valueType;
int minIndex,maxIndex;
};
struct ArrayObjectValue {
TypeName type;
vector<Object> values;
ArrayObjectValue(TypeName const &type,vector<Object> values): type(type),values(values) {}
ArrayObjectValue(ArrayObjectValue const &other): type(other.type),values(other.values) {}
ArrayObjectValue(TypeName const &type): type(type),values() {
auto meta=get<shared_ptr<ArrayMeta>>(type.meta);
auto value_type=meta->valueType;
auto min=meta->minIndex,max=meta->maxIndex;
assert(min <=max);
values.resize(max-min+1);
if (value_type.type==TypeName::Int) {
for (auto &x: values) x=Object{Object::Int,0};
} else if (value_type.type==TypeName::Array) {
for (auto &x: values) x=Object{Object::Array,make_shared<ArrayObjectValue>(value_type)};
}
}
Object &operator[](int index) {
auto meta=get<shared_ptr<ArrayMeta>>(type.meta);
auto min=meta->minIndex,max=meta->maxIndex;
assert(index >=min and index <=max);
return values[index-min];
}
};
struct NameError: exception {
string message;
NameError(string name): message(
format("NameError: name '{}' is not defined",name)
) {}
const char *what() const noexcept override {
return message.c_str();
}
};
class Interpreter {
public:

struct Scope {
map<Identifier,Object> variables;
Scope *parent=nullptr; 
Scope(Scope *parent=nullptr): parent(parent) {}
Object &get(Identifier name) {
auto it=variables.find(name);
if (it !=variables.end()) return it->second;
else {
if (parent !=nullptr) return parent->get(name);
else throw NameError(identifierMap.getString(name.name));
}
}
Object *getPtr(Identifier name) {
auto it=variables.find(name);
if (it !=variables.end()) return &it->second;
else {
if (parent !=nullptr) return parent->getPtr(name);
else return nullptr;
}
}
void declare(Identifier name,TypeName type) {
assert(not variables.contains(name)); 
if (type.type==type.Int) {
variables.insert({name,Object{Object::Int,0}});
} else if (type.type==type.Array) {
auto value=make_shared<ArrayObjectValue>(TypeName{type});
variables.insert({name,Object(Object::Array,value)});
} else if (type.type==type.Function) {
variables.insert({name,Object{Object::Function}});
} else if (type.type==TypeName::Undefined) {
variables.insert({name,Object{}});
} else {
unreachable();
}
}
};
Program program;
vector<unique_ptr<Scope>> scopeStack; 
Object ret; 
bool returnFlag=false; 
Interpreter(Program &&);
void run();
Object evaluateExpression(AST::ExpressionNode *); 
Object *evaluateLeftValueExpression(AST::ExpressionNode *); 
TypeName evaluateType(Identifier const &); 
template <typename OutIterator>
OutIterator getFunctionArguments(AST::ExpressionNode *,OutIterator); 
void runBlock(AST::BlockNode *); 
template <typename StatementPointer>
void runDeclarationStatement(StatementPointer); 
template <typename StatementPointer>
void runStatement(StatementPointer); 
void enterScope(); 
void leaveScope(); 
Interpreter::Scope *topScope(); 
};
Interpreter::Interpreter(Program &&program): program(move(program)) {
scopeStack.push_back(make_unique<Scope>());
topScope()->variables.insert({{"putchar"},Object{Object::BuiltinFunction,make_shared<Identifier>("putchar")}});
}
void Interpreter::enterScope() {
scopeStack.push_back(make_unique<Scope>(topScope()));
}
void Interpreter::leaveScope() {
assert(not scopeStack.empty());
scopeStack.pop_back();
}
template <typename OutIterator>
OutIterator Interpreter::getFunctionArguments(AST::ExpressionNode *node,OutIterator out) {
if (node->op==AST::ExpressionNode::SplitComma) {
out=getFunctionArguments(node->left,out);
out=getFunctionArguments(node->right,out);
} else {
if (node->op==AST::ExpressionNode::NoneOp) {
auto value_node=dynamic_cast<AST::ValueNode *>(node);
if (value_node->token.tag==Token::NoneTag) return out;
}
*out++=evaluateExpression(node);
}
return out;
}
void Interpreter::run() {
enterScope();
topScope()->variables.insert({{"cin"},Object{Object::BuiltinIStream}});
topScope()->variables.insert({{"cout"},Object{Object::BuiltinOStream}});
topScope()->variables.insert({{"endl"},Object{Object::BuiltinEndl}});
for (auto &x: program.root->statements) {
runStatement(x);
if (returnFlag) return leaveScope(),void();
}
auto mainPtr=topScope()->getPtr(identifierMap.getIndex("main"));
if (mainPtr !=nullptr) runBlock(get<AST::FunctionDeclareStatementNode *>(mainPtr->value)->body);
leaveScope();
}
Object *Interpreter::evaluateLeftValueExpression(AST::ExpressionNode *node) {
if (node==nullptr) return nullptr;
if (node->op==node->NoneOp) {
auto value_node=dynamic_cast<AST::ValueNode *>(node);
if (value_node->token.tag==Token::IdentifierTag) {
auto identifier=get<Identifier>(value_node->token.value);
return &topScope()->get(identifier);
} else {
return nullptr;
}
} else if (node->op==node->Subscript) {
auto l_son=evaluateLeftValueExpression(node->left);
auto r_son=evaluateExpression(node->right);
assert(l_son->type==Object::Array and r_son.type==Object::Int);
auto &array=*get<shared_ptr<ArrayObjectValue>>(l_son->value);
return &array[get<int>(r_son.value)];
} else {
return nullptr;
}
}
Object Interpreter::evaluateExpression(AST::ExpressionNode *node) {
if (node==nullptr) return Object{Object::None};
if (node->op==node->NoneOp) {
auto value_node=dynamic_cast<AST::ValueNode *>(node);
assert(value_node);
if (value_node->token.tag==Token::IntegerTag) {
Object res(Object::Int);
res.value=static_cast<int>(get<Tokenizer::Integer>(value_node->token.value).value);
return res;
} else if (value_node->token.tag==Token::IdentifierTag) {
auto identifier=get<Identifier>(value_node->token.value);
return topScope()->get(identifier);
} else if (value_node->token.tag==Token::NoneTag) {
return Object{};
} else {
unreachable();
return Object{};
}
} else if (node->op==node->MemberAccess) {
return *evaluateLeftValueExpression(node);
} else if (node->op==AST::ExpressionNode::Subscript) {
return *evaluateLeftValueExpression(node);
} else if (node->op==AST::ExpressionNode::Assign) {
auto l_son_ptr=evaluateLeftValueExpression(node->left);
assert(l_son_ptr !=nullptr);
auto &l_son=*l_son_ptr;
auto r_son=evaluateExpression(node->right);
l_son=r_son.copy();
return l_son;
} else if (node->op==AST::ExpressionNode::BitShiftLeft) {
auto l_son=evaluateExpression(node->left);
auto r_son=evaluateExpression(node->right);
if (l_son.type==Object::BuiltinOStream) {
if (r_son.type==Object::Int) {
io << get<int>(r_son.value);
} else if (r_son.type==Object::BuiltinEndl) {
io << "\n";
io.flush();
} else {
unreachable();
}
return l_son;
}
unreachable();
} else if (node->op==AST::ExpressionNode::BitShiftRight) {
auto l_son=evaluateExpression(node->left);
auto r_son=evaluateLeftValueExpression(node->right);
if (l_son.type==Object::BuiltinIStream) {
input >> get<int>(r_son->value);
return l_son;
}
unreachable();
} else {
auto l_son=evaluateExpression(node->left);
auto r_son=evaluateExpression(node->right);
#define JOIN_INT_OP(op_name,symbol) else if (node->op==AST::ExpressionNode::op_name and l_son.type==Object::Int and r_son.type==Object::Int) { Object res(Object::Int); assert(l_son.type==l_son.Int and r_son.type==r_son.Int); res.value=static_cast<int>(get<int>(l_son.value) symbol get<int>(r_son.value)); return res; }
if (0) {}
JOIN_INT_OP(Add,+)
JOIN_INT_OP(Sub,-)
JOIN_INT_OP(Mul,*)
JOIN_INT_OP(Div,/)
JOIN_INT_OP(Mod,%)
JOIN_INT_OP(Less,<)
JOIN_INT_OP(Greater,>)
JOIN_INT_OP(LessEqual,<=)
JOIN_INT_OP(GreaterEqual,>=)
JOIN_INT_OP(Equal,==)
JOIN_INT_OP(NotEqual,!=)
JOIN_INT_OP(BitShiftLeft,<<)
JOIN_INT_OP(BitShiftRight,>>)
JOIN_INT_OP(BitOr,|)
JOIN_INT_OP(Or,||)
JOIN_INT_OP(And,&&)
#undef JOIN_INT_OP
else if (node->op==AST::ExpressionNode::UnaryAdd) {
assert(l_son.type==Object::Int);
return l_son;
} else if (node->op==AST::ExpressionNode::UnarySub) {
assert(l_son.type==Object::Int);
return Object{Object::Int,-get<int>(l_son.value)};
} else if (node->op==AST::ExpressionNode::Not) {
assert(l_son.type==Object::Int);
return Object{Object::Int,!get<int>(l_son.value)};
} else if (node->op==AST::ExpressionNode::Call) {
if (l_son.type==Object::BuiltinFunction) {
auto name=*get<shared_ptr<Identifier>>(l_son.value); 
if (name=="putchar") {
assert(r_son.type==Object::Int);
io << static_cast<char>(get<int>(r_son.value));
return Object{};
} else {
unreachable();
}
} else {
assert(l_son.type==Object::Function);
auto func=get<AST::FunctionDeclareStatementNode *>(l_son.value);
vector<Object> args;
getFunctionArguments(node->right,back_inserter(args));
assert(args.size()==func->args.size());
auto size=static_cast<int>(args.size());
enterScope();
for (auto i=0; i < size; i++) {
topScope()->declare(func->args[i].name,evaluateType(func->args[i].typeName));
topScope()->get(func->args[i].name)=args[i].copy();
}
runBlock(func->body);
leaveScope();
if (returnFlag) return returnFlag=false,ret;
else return Object{Object::Int,0}; 
}
} else if (node->op==AST::ExpressionNode::SplitComma) {
return Object{};
} else {
unreachable();
return Object{};
}
}
}
TypeName Interpreter::evaluateType(Identifier const &name) {
if (name=="int") {
return {TypeName::Int};
} else if (name=="bool") {
return {TypeName::Int};
} else if (name=="array") {
return {TypeName::Array};
} else if (name=="object") {
return {TypeName::Struct};
}
return {};
}
template <typename StatementPointer>
void Interpreter::runDeclarationStatement(StatementPointer node) {
auto decl_node_ptr=dynamic_cast<AST::DeclareStatementNode const *>(node);
if (decl_node_ptr->declareType==AST::DeclareStatementNode::Function) {
auto function_decl_statement=dynamic_cast<AST::FunctionDeclareStatementNode *>(node);
topScope()->declare(function_decl_statement->name,TypeName{TypeName::Function});
topScope()->get(function_decl_statement->name).value=function_decl_statement;
} else {
auto const &variable_declare_statement=dynamic_cast<AST::VariableDeclareStatementNode const &>(*node);
auto type=evaluateType(variable_declare_statement.typeName);
for (auto var: variable_declare_statement.declares) {
auto name=var->name;
auto real_type=type;
auto arr=var->arraySize;
while (not arr.empty()) {
auto array_size=evaluateExpression(arr.back());
assert(array_size.type==Object::Int);
real_type={
TypeName::Array,make_shared<ArrayMeta>(real_type,0,get<int>(array_size.value))
};
arr.pop_back();
}
topScope()->declare(name,real_type);
if (var->initializer !=nullptr) {
assert(type.type==TypeName::Int);
topScope()->get(name)=evaluateExpression(var->initializer).copy();
}
}
}
}
void Interpreter::runBlock(AST::BlockNode *block) {
enterScope();
ret=Object{Object::Int,0};
for (auto &x: block->statements) {
runStatement(x);
if (returnFlag) return leaveScope(),void();
}
leaveScope();
}
template <typename StatementPointer>
void Interpreter::runStatement(StatementPointer x) {
if (x->type==AST::StatementNode::ExpressionEvaluateStatement) {
auto expression_evaluate_statement=dynamic_cast<AST::ExpressionEvaluateStatementNode *>(x);
evaluateExpression(expression_evaluate_statement->expr);
} else if (x->type==AST::StatementNode::DeclareStatement) {
runDeclarationStatement(dynamic_cast<AST::DeclareStatementNode *>(x));
} else if (x->type==AST::StatementNode::RunBlockStatement) {
auto run_block_statement=dynamic_cast<AST::RunBlockStatementNode *>(x);
runBlock(run_block_statement->block);
if (returnFlag) return;
} else if (x->type==AST::StatementNode::IfStatement) {
auto if_statement=dynamic_cast<AST::IfStatementNode *>(x);
auto condition=evaluateExpression(if_statement->condition);
assert(condition.type==Object::Int);
if (get<int>(condition.value) !=0) {
runBlock(if_statement->body);
} else if (if_statement->elseBody !=nullptr) {
runBlock(if_statement->elseBody);
}
} else if (x->type==AST::StatementNode::WhileStatement) {
auto while_statement=dynamic_cast<AST::WhileStatementNode *>(x);
auto check=[&]() {
auto condition=evaluateExpression(while_statement->condition);
assert(condition.type==Object::Int);
return get<int>(condition.value) !=0;
};
while (check()) {
runBlock(while_statement->body);
if (returnFlag) return;
}
} else if (x->type==AST::StatementNode::ForStatement) {
auto for_statement=dynamic_cast<AST::ForStatementNode *>(x);
runStatement(for_statement->init);
if (returnFlag) return;
while (true) {
auto condition=evaluateExpression(for_statement->condition);
assert(condition.type==Object::Int);
if (get<int>(condition.value)==0) break;
runBlock(for_statement->body);
if (returnFlag) return;
evaluateExpression(for_statement->step);
}
} else if (x->type==AST::StatementNode::ReturnStatement) {
auto return_statement=dynamic_cast<AST::ReturnStatementNode *>(x);
ret=evaluateExpression(return_statement->expr);
returnFlag=true;
} else if (x->type==AST::StatementNode::NoneStatement) {
} else {
unreachable();
}
}
Interpreter::Scope *Interpreter::topScope() {
return scopeStack.back().get();
}
}
void solve() {
int N; io >> N;
vector<int> vec(N);
for (auto &x: vec) io >> x;
string input_str;
stringstream ss;
for (auto const &x: vec) ss << x << " ";
input_str=ss.str();
auto tokens=Tokenizer{}.tokenize(io);
auto ast=AST::BlockNode::parse(tokens).node;
auto interpreter=Interpreter::Interpreter(Program{ast,IdentifierMap{}});
input=StringScanner{move(input_str)};
interpreter.run();
}
}
int main(int argc,char const *argv[]) {
DEBUG_MODE=(argc-1) and not strcmp("-d",argv[1]);
FutureProgram::solve();
return 0;
}
