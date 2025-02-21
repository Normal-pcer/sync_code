#include<iostream>
#include<cstdio>
#include<cmath>
#include<algorithm>//头文件不多说
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
template <typename T>
inline void read(T&x){//快读，实测超级快读不搭配其他技巧也过不了
	int w=0;x=0;
	char ch = getchar();
	while(ch<'0' || ch>'9'){
		if(ch=='-') w=1;
		ch = getchar();
	}
	while(ch>='0' && ch<='9'){
		x = (x<<1)+(x<<3)+(ch^48);
		ch = getchar();
	}
	if(w) x=-x;
}
template <typename T,typename...Args>
inline void read(T&t,Args&...args){
	read(t);read(args...);
}
template <typename T>
inline T Max(T x,T y){//实测手写会比自带的max、if和三目运算快
  return (x > y ? x : y);
}
const int N = 6e6+10;
int n,m;
int P=1,DEP=0;
struct Tree{//用结构体内存访问比较连续
	ll mx; int pos;
	Tree(){
		mx = 0; pos = 0;
	}
	Tree(ll tmx,int tpos){
		mx = tmx; pos = tpos;
	}
	inline Tree operator + (const Tree&G) const{//实测重载运算符比外层调用函数快
		if(mx==G.mx) return Tree(mx,Max(pos,G.pos));
		else if(mx>G.mx) return Tree(mx,pos);
		else return G;
	}
}tr[N*3];
ll tag[N*3];
int main(){
// 	freopen("in.in","r",stdin);
// 	freopen("out.out","w",stdout);
	
	read(n,m);
	while(P<=n+1) P<<=1;
	for(int i=1,x;i<=n;++i){
		read(x); tr[i+P] = Tree(1ll*x,i);//直接读入省掉建树的常数
	}
	for(int i=P-1;i;--i) tr[i] = tr[i<<1]+tr[i<<1|1];//用重载运算符
	for(int i=1,opt,x,y;i<=m;++i){
		read(opt,x,y);
		if(opt==1){//把update内容搬到主函数里
			int l = 1+P-1,r = x+P+1; ll k = 1ll*y;
			while(l^1^r){
				if(~l&1) tr[l^1].mx+=k,tag[l^1]+=k;
				if(r&1) tr[r^1].mx+=k,tag[r^1]+=k;
				l>>=1;r>>=1;//直接更新，实测比写push_up函数快
				tr[l] = tr[l<<1]+tr[l<<1|1];
				tr[l].mx += tag[l];//标记永久化要加累加标记值
				tr[r] = tr[r<<1]+tr[r<<1|1];
				tr[r].mx += tag[r];
			}
			for(l>>=1; l ;l>>=1){//更新上传
				tr[l] = tr[l<<1]+tr[l<<1|1];
				tr[l].mx += tag[l];
			}
		}
		else{//把query内容搬到主函数里
			int l = 1+P-1,r = y+P+1; Tree resl,resr;
			while(l^1^r){
				if(~l&1) resl = resl+tr[l^1];//注意左右区间合并顺序
				if(r&1) resr = tr[r^1]+resr;
				l>>=1;r>>=1;
				resl.mx += tag[l];//累加标记值
				resr.mx += tag[r];
			}
			printf("%d\n",Max(0,x-(resl+resr).pos));//没有答案输出0
		}
	}

	// fclose(stdin);
 	// fclose(stdout);
	return 0;
}