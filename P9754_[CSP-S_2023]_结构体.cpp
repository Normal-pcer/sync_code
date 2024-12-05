/**
 * @link https://www.luogu.com.cn/problem/P9754
 */

#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=(n);i++)
#define from(i,b,e) for(int i=(b);i<=(e);i++)
#define rev(i,e,b) for(int i=(e);i>=(b);i--)

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
    const std::string endl = "\n";
}
using namespace lib;

using std::string;

namespace Solution {

    int N_operationsCount;
    const size_t INF_LL = 0x7fffffffffffffff;

    // 标识符
    struct Identifier;

    // 类型
    struct Type {
        string name;
        std::vector<Identifier> members;
        size_t occupied_size;
        size_t align_size;
    };
    std::map<string, Type> types;

    struct Identifier {
        Type *type;
        string name;
    };

    struct Element {
        Identifier *id;
        size_t address;

    };

    std::map<size_t, Element> memory;  // 地址到元素的映射

    size_t alignAddress(size_t addr, size_t align) {
        if (addr%align!=0) 
            switch (align) {
                case 1:  return addr;
                case 2:  return addr-(addr&1)+2;
                case 4:  return addr-(addr&3)+4;
                case 8:  return addr-(addr&7)+8;
                default:  assert(false);
            }
        else 
            return addr;
    }

    inline void defineType(Type t) {
        types[t.name] = t;
    }

    
    void init() {
        io >> N_operationsCount;

        // 定义基本类型
        auto Byte = Type{"byte", {}, 1, 1}; defineType(Byte);
        auto Short = Type{"short", {}, 2, 2}; defineType(Short);
        auto Int = Type{"int", {}, 4, 4}; defineType(Int);
        auto Long = Type{"long", {}, 8, 8}; defineType(Long);
    }

    /**
     * 从标准输入中读入相应数据，并且定义一个新的类型
     */
    void read_defineType() {
        string typeName = io.read<string>();
        int memberCount = io.read<int>();

        auto newType = Type{typeName, {}, 0, 0};

        from(_, 1, memberCount) {
            string memberTypeName, memberName;
            io >> memberTypeName >> memberName;
            
            Type *memberTypePtr = &types.at(memberTypeName);
            newType.members.push_back(Identifier{memberTypePtr, memberName});
        }

        size_t max_align = 0;  // 对齐字节数
        for (auto &member: newType.members) {
            chkMax(max_align, member.type->align_size);
        }
        newType.align_size = max_align;

        size_t size = 0;  // 占用字节数
        for (auto &member: newType.members) {
            size = alignAddress(size+member.type->occupied_size, member.type->align_size);
        }
        // 对齐类型大小
        size = alignAddress(size, max_align);
        newType.occupied_size = size;

        defineType(newType);

        printf("%lld %lld\n", newType.occupied_size, newType.align_size);
    }

    /**
     * 读入并创建指定元素
     */
    void read_defineElement() {
        string typeName = io.read<string>();
        auto &type = types[typeName];
        string name = io.read<string>();
        auto id = new Identifier{&type, name};

        // 为这个元素分配地址
        // 寻找最后一个元素
        if (not memory.empty()) {
            auto &last = *(memory.rbegin());
            size_t nextAddr = last.second.address + last.second.id->type->occupied_size;
            nextAddr = alignAddress(nextAddr, type.align_size);

            assert(memory.find(nextAddr) == memory.end());
            auto newElement = Element{id, nextAddr};
            memory.insert({nextAddr, newElement});
            printf("%lld\n", nextAddr);
        } else {
            auto newElement = Element{id, 0};
            memory.insert({0ULL, newElement});
            printf("0\n");
        }
    }

    /**
     * 读取 father 的一个成员，返回地址偏移量
     */
    size_t read_accessMember(Type *father) {
        std::string name = "";
        name.reserve(10);
        char c = getchar();
        while ('a'<=c && c<='z') {
            name += c;
            c = getchar();
        }

        size_t offset = 0;
        for (auto &id: father->members) {
            offset = alignAddress(offset, id.type->align_size);
            if (id.name == name) {
                if (c == '.') {
                    return offset + read_accessMember(id.type);
                } else {
                    return offset;
                }
            }
            offset += alignAddress(id.type->occupied_size, id.type->align_size);
        }
        assert(false);
        return 0;
    }

    /**
     * 读入并访问一个元素，输出它的地址
     */
    void read_accessElement() {
        std::string name = "";
        name.reserve(10);
        char c = getchar();
        while ('a'<=c && c<='z') {
            name += c;
            c = getchar();
        }
        if (c == '.') {
            // 继续向下访问
            for (auto &reg: memory) {
                if (reg.second.id->name == name) {
                    size_t ans = reg.second.address + read_accessMember(reg.second.id->type);
                    printf("%lld\n", ans);
                    return;
                }
            }
        } else {
            for (auto &reg: memory) {
                if (reg.second.id->name == name) {
                    printf("%lld\n", reg.second.address);
                    return;
                }
            }
        }
        assert(false);
    }

    /**
     * 在一个类型中，访问给定偏移量的成员，返回这个成员的名字
     */
    string accessAddressInType(Type *father, size_t offset) {
        // 特判基本类型
        if (father->members.empty()) {
            return "";
        }

        // 对于每一个成员
        size_t pointer = 0;
        for (auto &member: father->members) {
            pointer = alignAddress(pointer, member.type->align_size);
            if (pointer > offset) {  // 空位
                return "ERR";
            }
            if (pointer+member.type->occupied_size > offset) {
                // 地址被这个成员占用
                string res = accessAddressInType(member.type, offset-pointer);
                if (res == "ERR")  return "ERR";
                else if (res == "")  return member.name;
                else  return member.name + '.' + res;
            }
            pointer += member.type->occupied_size;
        }

        // assert(false);
        return "ERR";
    }

    /**
     * 读入并访问一个地址
     */
    void read_accessAddress() {
        auto addr = io.read<size_t>();
        // 对于内存中的每一个元素
        size_t pointer = 0;
        for (auto &pair: memory) {
            size_t cur_size = pair.second.id->type->occupied_size;
            pointer = alignAddress(pointer, pair.second.id->type->align_size);
            if (pointer > addr) {
                printf("ERR\n");
                return;
            }
            if (pointer+cur_size > addr) {
                // 地址被 pair 对应的元素占用
                string ans = accessAddressInType(pair.second.id->type, addr-pointer);
                if (ans == "ERR")
                    printf("ERR\n");
                else if (ans == "")
                    printf("%s\n", pair.second.id->name.c_str());
                else
                    printf("%s\n", (pair.second.id->name + '.' + ans).c_str());
                return;
            }
            pointer += cur_size;
        }
        // assert(false);
        printf("ERR\n");  // 访问了后部的空位
    }

    void solve() {
        init();

        from(opIndex, 1, N_operationsCount) {
            int operationType = io.read<int>();
            if (operationType == 1)  read_defineType();
            else if (operationType == 2)  read_defineElement();
            else if (operationType == 3)  read_accessElement();
            else if (operationType == 4)  read_accessAddress();
            else  assert(false);
        }
    }
}

int main(int argc, char const *argv[]) {
    initDebug;
    debug {
        freopen("P9754.in", "r", stdin);
        freopen("P9754.out", "w", stdout);
    }
    Solution::solve();
    return 0;
}