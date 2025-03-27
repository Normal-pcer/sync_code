#include<bits/stdc++.h>
#ifdef int
#define __defined_int_to_long_long
#undef int
#endif
#ifdef __linux__
#include <sys/stat.h>
#include <sys/mman.h>
#endif
#define USE_FREAD
// #undef USE_FREAD
// 取消注释上一行会使用 getchar() 替代 fread，可以不使用 EOF 结束读入，但是降低性能 
namespace lib{
#ifndef LIB_STRING
    using string=std::string;
#endif
#ifdef USE_FREAD
    template <const long long MAXSIZE, const long long MAX_ITEM_SZ=500>
#endif
    struct IO {
#ifdef USE_FREAD
#ifdef __linux__
        struct stat s;
        char *c;
#else
        char buf[MAXSIZE],*p1,*p2;
#endif// __linux__
        char pbuf[MAXSIZE],*pp;
#ifdef __linux__
        IO(): pp(pbuf) {
            fstat(0, &s);
            c = (char*)mmap(nullptr, s.st_size, 1, 2, 0, 0);
        }
#else // not __linux__
		IO(): p1(buf), p2(buf), pp(pbuf) {}
#endif // __linux
        ~IO() {  fwrite(pbuf,1,pp-pbuf,stdout);  }
#ifdef __linux__
		inline char gc() { return *c++; }
#else  // not __linux__
        inline char gc() {
            if (p1==p2) p2=(p1=buf)+fread(buf,1,MAXSIZE,stdin);
            return p1==p2?'\0':*p1++;
        }
#endif // __linux__
        inline void sync() { fwrite(pbuf,1,MAXSIZE,stdout); pp=pbuf; }
#else // not USE_FREAD
        inline void sync() {}
        inline char gc() {  return getchar();  }
#endif// USE_FREAD
        char floatFormat[10]="%.6f", doubleFormat[10]="%.6lf";
        inline bool blank(char ch) { return ch<=32 or ch==127; }
        inline bool isd(char x) {return (x>='0' and x<='9');}
        inline IO& setprecision(int d) {
            sprintf(floatFormat, "%%.%df", d); sprintf(doubleFormat, "%%.%dlf", d);
            return *this;
        }
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
        inline void push(const char &c) {
#ifdef USE_FREAD
            if (pp-pbuf==MAXSIZE) sync();
            *pp++=c;
#else  // not USE_FREAD
            putchar(c);
#endif
        }
        inline void write(const double x) {
#ifdef USE_FREAD
            if (pp-pbuf>=MAXSIZE-MAX_ITEM_SZ) sync();
            pp += sprintf(pp, doubleFormat, x);
#endif
#ifndef USE_FREAD
            printf(doubleFormat, x);
#endif
        }
        inline void write(const float x) {
#ifdef USE_FREAD
            if (pp-pbuf>=MAXSIZE-MAX_ITEM_SZ) sync();
            pp += sprintf(pp, floatFormat, x);
#endif
#ifndef USE_FREAD
            printf(floatFormat, x);
#endif
        }
        inline void write(const char c) {  push(c);  }
        inline void write(const string &s){  for (auto i:s)  push(i);  }
        inline void write(const char *s){  for (; *s; ++s) push(*s);  }
        template <class T>
        inline void write(T x) {
            static char sta[40]; int top=0;
            if (x<0) {
                push('-'),sta[top++]=(-(x%10))^48,x=-(x/10);
                if (x==0) { push(sta[--top]); return; }
            }
            do {  sta[top++]=x%10^48,x/=10;  } while (x);
            while (top) push(sta[--top]);
        }
        template <class T,class... Types>  inline void write(const T &x,const Types &...args){ write(x); write(' '); write(args...); }
        template <class... Types> inline void writeln(const Types &...args){  write(args...); write('\n');  }
        template<class T=int> inline T get() {  T x; read(x); return x;  }
        // 流式输入输出
        template <class T> inline IO& operator>>(T&x) {  read(x); return *this; }
        template <class T> inline IO& operator<<(const T&x) {  write(x); return *this; }
    };
    IO
#ifdef USE_FREAD
    <1048576>
#endif
    io;
}

#ifdef __defined_int_to_long_long
#undef __defined_int_to_long_long
#define int long long
#endif
using namespace lib;

using ll = long long;
#define lc (rt<<1)
#define rc (rt<<1|1)
#define mid ((l+r)>>1)
#define cin io
#define cout io
#define endl '\n'
using namespace std;
const int N=1E5+5;
int a[N];
struct tree{
	ll res;
	int num[3];
	int lz[3];
	ll trs[3][3];
}tr[N*4];
int lh[4];
ll lt[4][4];
void trans(tree& x,array<int,3> t){
	for(int i=0;i<3;i++){
		lh[i]=0;
		for(int j=0;j<3;j++){
			lt[i][j]=0;
		}
	}
	for(int i=0;i<3;i++){
		lh[t[i]]+=x.num[i];
		for(int j=0;j<3;j++){
			lt[t[i]][t[j]]+=x.trs[i][j];
		}
	}
	for(int i=0;i<3;i++){
		x.num[i]=lh[i];
		for(int j=0;j<3;j++){
			x.trs[i][j]=lt[i][j];
		}
	}
	x.res=x.trs[1][0]+x.trs[2][0]+x.trs[2][1];
}
void pushup(int rt){
	for(int i=0;i<3;i++){
		tr[rt].num[i]=tr[lc].num[i]+tr[rc].num[i];
		for(int j=0;j<3;j++){
			tr[rt].trs[i][j]=tr[lc].trs[i][j]+tr[rc].trs[i][j]+tr[lc].num[i]*tr[rc].num[j];
		}
	}
	tr[rt].res=tr[rt].trs[1][0]+tr[rt].trs[2][0]+tr[rt].trs[2][1];
}
void pushdown(int rt){
	if(tr[rt].lz[0]!=-1){
		trans(tr[lc],{tr[rt].lz[0],tr[rt].lz[1],tr[rt].lz[2]});
		trans(tr[rc],{tr[rt].lz[0],tr[rt].lz[1],tr[rt].lz[2]});	
		for(int i=0;i<3;i++){
			tr[lc].lz[i]=tr[rc].lz[i]=tr[rt].lz[i];
		}
	}
	for(int i=0;i<3;i++){	
		tr[rt].lz[i]=-1;
	}
}
void build(int rt,int l,int r){
	tr[rt].lz[0]=tr[rt].lz[1]=tr[rt].lz[2]=-1;
	if(l==r){
		tr[rt].num[a[l]]++;
		return;
	}
	build(lc,l,mid);
	build(rc,mid+1,r);
	pushup(rt);
}
void update(int rt,int l,int r,int L,int R,array<int,3> t){
	if(L<=l&&R>=r){
		trans(tr[rt],t);
		for(int i=0;i<3;i++){
			tr[rt].lz[i]=t[i];
		}
		return;
	}
	pushdown(rt);
	if(L<=mid)update(lc,l,mid,L,R,t);
	if(R>mid)update(rc,mid+1,r,L,R,t);
	pushup(rt);
}
tree query(int rt,int l,int r,int L,int R,int s1,int s2){
	if(L<=l&&R>=r){
		tree node;
		node.res=tr[rt].res+s1*tr[rt].num[0]+s2*tr[rt].num[0]+s2*tr[rt].num[1];
		for(int i=0;i<3;i++){
			node.num[i]=tr[rt].num[i];
		}
		return node;
	}
	pushdown(rt);
	tree node;
	node.res=0;
	for(int i=0;i<3;i++){
		node.num[i]=0;
	}
	if(L<=mid){
		auto tt=query(lc,l,mid,L,R,s1,s2);
		node.res+=tt.res;
		for(int i=0;i<3;i++){
			node.num[i]+=tt.num[i];
		}
	}
	if(R>mid){
		auto tt=query(rc,mid+1,r,L,R,s1+node.num[1],s2+node.num[2]);
		node.res+=tt.res;
		for(int i=0;i<3;i++){
			node.num[i]+=tt.num[i];
		}		
	}
	return node;
}
int main(){
	int n,m;
	cin>>n>>m;
	for(int i=1;i<=n;i++){
		cin>>a[i];
	}
	build(1,1,n);
	for(int i=1;i<=m;i++){
		int op,l,r;
		cin>>op>>l>>r;
		if(op==1){
			cout<<query(1,1,n,l,r,0,0).res<<"\n";
		}else{
			int x,y,z;
			cin>>x>>y>>z;
			update(1,1,n,l,r,{x,y,z});
		}
	}
}
/*
5 5
2 1 0 2 1
2 1 1 1 1 1
1 1 5
2 1 5 2 0 1
1 2 3
1 2 4
*/