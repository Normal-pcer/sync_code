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
#define int __int128
#define cin io
#define cout io
using namespace std;
const int N=5000;
struct edge{
	int to;
	int nxt;
	int w;
}e[200005];
int head[N],cur[N],lev[N],p[55][55],tot=1,n,m,stt,edd;
int dx[]={1,0,-1,0},dy[]={0,1,0,-1};
void add(int u,int v,int w){
	e[++tot].to=v;
	e[tot].nxt=head[u];
	e[tot].w=w;
	head[u]=tot;
}
int idx(int x,int y){
	return (x-1)*n+y;	
}
int init(int z){
	memset(e,0,sizeof(e));
	memset(head,0,sizeof(head));
	tot=1;
	stt=n*m+1;
	edd=n*m+2;
	for(int i=1;i<=n;i++){
		for(int j=1;j<=m;j++){
			if(p[i][j]>z)return 0;
			if((i+j)&1){
				add(stt,idx(i,j),z-p[i][j]);
				add(idx(i,j),stt,0);
				for(int k=0;k<3;k++){
					int ti=i+dx[k];
					int tj=j+dy[k];
					if(ti<=0||ti>n)continue;
					if(tj<=0||tj>m)continue;
					add(idx(i,j),idx(ti,tj),LLONG_MAX);
					add(idx(ti,tj),idx(i,j),LLONG_MAX);
				}
			}else{
				add(idx(i,j),edd,z-p[i][j]);
				add(edd,idx(i,j),0);
			}
		}
	}
	return 1;
}
int bfs(int s,int t){
	memset(lev,0,sizeof(lev));
	memcpy(cur,head,sizeof(cur));
	queue<int> q;
	q.push(s);
	lev[s]=1;
	while(!q.empty()){
		int u=q.front();
		q.pop();
		if(u==t)return 1;
		for(int i=head[u];i;i=e[i].nxt){
			if(!e[i].w)continue;
			int v=e[i].to;
			if(lev[v])continue;
			lev[v]=lev[u]+1;
			q.push(v);
		}
	}
	return 0;
}
int dfs(int u,int t,int mx){
	if(u==t)return mx;
	int tt=0;
	for(int &i=cur[u];i;i=e[i].nxt){
		if(!e[i].w)continue;
		int v=e[i].to;
		if(lev[v]!=lev[u]+1)continue;
		int nw=dfs(v,t,min(mx,e[i].w));
		if(nw==0)lev[v]=-10;
		tt+=nw;
		mx-=nw;
		e[i].w-=nw;
		e[i^1].w+=nw;
		if(!mx)break;
	}
	return tt;
}
int dinic(int s,int t){
	int ans=0;
	while(bfs(s,t)){
		while(int flow=dfs(s,t,LLONG_MAX)){
			ans+=flow;
		}
	}
	return ans;
}
int check(int mid,int rst){
	if(!init(mid))return 0;
	return dinic(stt,edd)==rst;
}
void solve(){
	cin>>n>>m;
	int sum1=0,sum2=0,sum0;
	int tt1=0,tt2=0;
	for(int i=1;i<=n;i++){
		for(int j=1;j<=m;j++){
			cin>>p[i][j];
			if((i+j)&1){
				sum1+=p[i][j];
				tt1++;
			}else{
				sum2+=p[i][j];
				tt2++;
			}			
		}
	}
	sum0=sum1+sum2;
	if(tt1!=tt2){
		int x=(sum1-sum2)/(tt1-tt2);
		if(check(x,tt1*x-sum1)){
			cout<<(n*m*x-sum0)/2<<"\n";
		}else{
			cout<<-1<<"\n";
		}
		return;
	}
	if(sum1!=sum2){
		cout<<-1<<"\n";
	}
	int l=0,r=1E14,ans=-1;
	while(l<=r){
		int mid=(l+r)>>1;
		if(check(mid,tt1*mid-sum1)){
			r=mid-1;
			ans=mid;
		}else{
			l=mid+1;
		}
	}
	if(ans==-1){
		cout<<-1<<"\n";
		return;
	}
	cout<<(m*n*ans-sum0)/2<<"\n";
}
signed main(){
//	freopen("1.in","r",stdin);
//	ios::sync_with_stdio(false);
//	cin.tie(0);
	int T;
	cin>>T;
	while(T--){
		solve();
	}
}
/*
1
4 4 
1 1 1 1
1 1 1 1
1 1 1 1
1 1 2 2
*/