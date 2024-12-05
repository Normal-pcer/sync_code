/**
 * @link https://www.luogu.com.cn/problem/P11186?contestId=200686
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
#define update(base,op,modify...) base=op((base),##modify)
#define ensure(v, con, otw) (((v) con)? (v): (otw))
#define optional(ptr) if(ptr)ptr
#define never if constexpr(0)
#define always if constexpr(1)
#define bitOr(x,y) (((x)&(y))^(((x)^(y))|(~(x)&(y))))
#define Infinity 2147483647
#define putInt(n) printf("%d\n",(n))
#define compare(x,y,g,e,l) (((x)>(y))?(g):(((x)<(y))?(l):(e)))
bool DEBUG_MODE=false;
typedef long long ll; typedef unsigned long long ull;
inline void batchOutput(int *begin, int n, const char *format){upto(i, n)printf(format, begin[i]);printf("\n");} inline void batchOutput(int*begin, int n) {batchOutput(begin,n,"%3d ");}
#define batchOutput2d(b, r, c, fmt) upto(i,r){upto(j,c)printf(fmt,b[i][j]);printf("\n");}
namespace lib{}

#include <bits/stdc++.h>
namespace lib{
	// template <const size_t MAXSIZE>
	struct IO {
		// char buf[MAXSIZE],*p1,*p2;
		// char pbuf[MAXSIZE],*pp;
		// inline char gc() {
		// 	if (p1==p2) p2=(p1=buf)+fread(buf,1,MAXSIZE,stdin);
		// 	return p1==p2?' ':*p1++;
		// }
        inline char gc() {
			return getchar();
        }
		inline bool blank(char ch) { return ch==' ' or ch=='\n' or ch=='\r' or ch=='\t'; }
		inline bool isd(char x) {return (x>='0' and x<='9');}
        std::string input() {
            char c = gc();
            std::string res="";
            while((c&&!blank(c)) || c==' ') {
                res.push_back(c);
                c = gc();
            }
            return res;
        }
		template <class T>
		inline void scan(T &x) {
			double tmp=1;
			bool sign=0;
			x=0;
			char ch=gc();
			for (; not isd(ch); ch=gc()) if (ch=='-') sign=1;
			for (; isd(ch); ch=gc()) x=x*10+(ch^48);
			if (ch=='.') for (ch=gc(); isd(ch); ch=gc()) tmp*=.1f,x+=tmp*(ch^48);
			if (sign) x=-x;
		}
		inline void scan(char *s) {
			char ch=gc();
			for (; blank(ch); ch=gc());
			for (; not blank(ch); ch=gc()) *s++=ch;
			*s=0;
		}
		inline void scan(char &c) {
			for (c=gc(); blank(c); c=gc());
		}
		inline void scan(std::string &s){
			std::string().swap(s);
			char ch=gc();
			for (; blank(ch); ch=gc());
			for (; not blank(ch); ch=gc()) s.push_back(ch);
		}
		template <class T,class... Types>
		inline void scan(T &x,Types &...args){
			scan(x);
			scan(args...);
		}
		inline void push(const char &c) {
			// if (pp-pbuf==MAXSIZE) fwrite(pbuf,1,MAXSIZE,stdout),pp=pbuf;
			// *pp++=c;
            putchar(c);
		}
		template <class T>
		inline void write(T x) {
			if (x<0) x=-x,push('-');
			static char sta[40];
			int top=0;
			do {
				sta[top++]=x%10^48,x/=10;
			} while (x);
			while (top) push(sta[--top]);
		}
        inline void write(double x) {
            printf("%lf",x);
        }
        inline void write(float x) {
            printf("%f",x);
        }
        inline void write(char c) {
            push(c);
        }
		inline void write(std::string &s){
			for (auto i:s) push(i);
		}
        inline void write(const char *s){
			for (; *s; ++s) push(*s);
        }
		template <class T,class... Types>
		inline void write(T &x,Types &...args){
			write(x);
			write(args...);
		}

        template<class T=int>
        inline T read() {
            T x;
            scan(x);
            return x;
        }

        // 流式输入输出
		template <class T>
        inline IO& operator>>(T&x) {
            scan(x);
            return *this;
        }
        template <class T>
        inline IO& operator<<(T&x) {
            write(x);
            return *this;
        }
	};
	IO io;
    const char endl = '\n';
}

using namespace lib;

namespace Solution {
    enum OperatorType {
        constant = '0',       // const
        greater = '>',        // >
        less = '<',           // <
        question = '?',       // ?
        colon = ':',          // :
        x = 'x'               // x
    };

    int M, Q;
    const int _M = 1e5+5;
    std::string expr;

    struct SegmentTree {
        struct Node {
            int l, r;
            int t = Infinity;
        } t[_M<<2];
        #define ls (p<<1)
        #define rs (p<<1|1)

        void push_up() {}

        void push_down(int p) {
            if (t[p].t != Infinity) {
                t[ls].t = t[rs].t = t[p].t;
                t[p].t = Infinity;
            }
        }

        void build(int p, int l, int r) {
            t[p].l = l; t[p].r = r;
            if (l==r)  return;
            int m = (l+r)>>1;
            build(ls, l, m);
            build(rs, m+1, r);
        }

        void modify(int p, int l, int r, int val) {
            never log("modify %d %d %d %d\n", p, l, r, val);
            if (t[p].l>=l and t[p].r<=r) {
                t[p].t = val;
                return;
            }
            push_down(p);
            if (t[ls].r >= l)  modify(ls, l, r, val);
            if (t[rs].l <= r)  modify(rs, l, r, val);
        }

        int query(int p, int i) {
            if (t[p].l==i and t[p].r==i) {
                assert(t[p].t < Infinity);
                return t[p].t;
            }
            push_down(p);
            if (t[ls].r >= i)  return query(ls, i);
            if (t[rs].l <= i)  return query(rs, i);
            assert(false);
            return 0;
        }

        #undef ls
        #undef rs
    } ST;

    struct Node {
        char opera = '0';
        int number = 0;
        Node *left = nullptr;
        Node *right = nullptr;

        Node (bool op, int num) {
            if (op) {
                assert(num=='0' or num=='>' or num=='<' or num=='?' or num==':' or num=='x');
                opera = (char)num;
            } else {
                if (num >= 0) {  // 常数
                    opera = OperatorType::constant;
                } else {
                    opera = OperatorType::x;
                }
                number = num;
            }
        }

        void query_range(int l, int r) {
            if (l>r)  return;
            if (opera == OperatorType::constant) 
                ST.modify(1, l, r, number);
            else if (opera == OperatorType::colon) {
                assert(left && left->opera==OperatorType::question && left->left && left->right && right);
                int symbol = left->left->opera;
                assert(left->left->right->opera == OperatorType::constant);
                int cmp = left->left->right->number;

                int new_l = l, new_r = r;
                if (symbol == OperatorType::greater) {
                    chkMax(new_l, cmp+1);
                } else {
                    chkMin(new_r, cmp-1);
                }

                left->right->query_range(new_l, new_r);
                right->query_range(l, new_l-1);
                right->query_range(new_r+1, r);
            } else
                assert(false);
        }
    };

    Node *parse(std::string expr) {
        // 转为后缀表达式
        struct Temp {
            bool op;  // 是否为运算符
            int val;  // 数字，或运算符的 ascii
        };
        std::stack<char> s;  // 运算符栈
        std::vector<Temp> suf_expr;  // 后缀表达式

        bool digit_flag = false;

        from(i, 0, (int)expr.size()-1) {
            char c = expr[i];
            switch (c) {
            case '<':
            case '>':
                s.push( c );
                digit_flag = false;
                break;
            case '?':
                assert(not s.empty());
                suf_expr.push_back({true, s.top()});  s.pop();
                s.push(c);
                digit_flag = false;
                break;
            case ':':
                while (not s.empty() and s.top() != OperatorType::question) {
                    suf_expr.push_back({true, s.top()});  s.pop();
                }
                assert(not s.empty());
                suf_expr.push_back({true, s.top()});  s.pop();
                s.push(c);
                digit_flag = false;
                break;
            case 'x':
                suf_expr.push_back( {false, -1} );
                digit_flag = false;
                break;
            default:  // 数字
                Temp &back = suf_expr.back();
                if (digit_flag) {  // 如果它是一个数字的末几位
                    back.val *= 10;
                    back.val += c-'0';
                } else {
                    suf_expr.push_back( {false, c-'0'} );
                }
                digit_flag = true;
            }
        }
        while (not s.empty()) {
            suf_expr.push_back( {true, s.top()} );
            s.pop();
        }

        never for (auto &i: suf_expr) {
            if (i.op) {
                log("(true, %c) ", i.val)
            } else {
                log("(false, %d)", i.val)
            }
        }

        never printf("\n");

        // 解析后缀表达式
        std::stack<Node*> nodes;
        Node *new_l = nullptr;  // 新节点的左儿子
        Node *new_r = nullptr;  // 新节点的右儿子

        for (auto &i: suf_expr) {
            if (i.op == false) {  // 解析常数和未知数
                nodes.push(new Node(false, i.val));
            } else {
                new_r = nodes.top(); nodes.pop();
                new_l = nodes.top(); nodes.pop();
                nodes.push(new Node(true, i.val));
                nodes.top()->left = new_l;
                nodes.top()->right = new_r;
            }
        }

        return nodes.top();
    }

    void init() {
        io >> M >> Q;
        io >> expr;
        ST.build(1, 1, M);
    }

    void walk(Node* head) {
        if (head == nullptr)  return;
        if (head->opera == OperatorType::constant) {
            printf("%d", head->number);
        } else if (head->opera == OperatorType::x) {
            printf("x");
        } else {
            printf("%c", head->opera);
        }
        printf(" ");
        walk(head->left);
        walk(head->right);
    }

    void solve() {
        init();
        auto top = parse(expr);
        never walk(top);

        top->query_range(1, M);
        upto(_, Q) {
            int x = io.read<int>();
            int q = std::max(1, std::min(x, M));
            printf("%d\n", ST.query(1, q));
        }
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    debug {
        freopen("P11186.in", "r", stdin);
        freopen("P11186.out", "w", stdout);
    }
    Solution::solve();
    return 0;
}