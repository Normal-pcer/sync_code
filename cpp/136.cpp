#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=500500;
inline ll read(){
    ll x=0,f=1;
    char c=getchar();
    while(c<'0'||c>'9'){
        if(c=='-')
            f=-1;
        c=getchar();
    }
    while(c>='0'&&c<='9'){
        x=(x<<1)+(x<<3)+(c^48);
        c=getchar();
    }
    return x*f;
}
inline void write(ll x){
	if(x<0){
		putchar('-');
		x=-x;
	}
	if(x>9)
	  write(x/10);
	putchar(x%10+'0');
}
void Swap(ll &a,ll &b){
	ll t=a;
	a=b;
	b=t;
}
struct Node{
	ll l,r;
	ll t,k; //t 为区间取反，k 为区间长度 
	ll M1,M2; //M1 为 0 的区间最长、M2 为 1 的区间最长 
	ll z1,y1;
	ll z2,y2;
}X[N<<2];
ll n,m,op,x,y;
ll a[N];
string s;
void SWAP(ll k){ //交换 1 和 0 的信息 
	Swap(X[k].M1,X[k].M2);
	Swap(X[k].z1,X[k].z2);
	Swap(X[k].y1,X[k].y2);
}
void pushup(ll k){ //区间合并 
	if(X[k<<1].k==X[k<<1].z1)
	  X[k].z1=X[k<<1].k+X[k<<1|1].z1;
	else
	  X[k].z1=X[k<<1].z1;
	if(X[k<<1|1].k==X[k<<1|1].y1)
	  X[k].y1=X[k<<1|1].k+X[k<<1].y1;
	else
	  X[k].y1=X[k<<1|1].y1;
	if(X[k<<1].k==X[k<<1].z2)
	  X[k].z2=X[k<<1].k+X[k<<1|1].z2;
	else
	  X[k].z2=X[k<<1].z2;
	if(X[k<<1|1].k==X[k<<1|1].y2)
	  X[k].y2=X[k<<1|1].k+X[k<<1].y2;
	else
	  X[k].y2=X[k<<1|1].y2;
	X[k].M1=max(max(X[k<<1].M1,X[k<<1|1].M1),X[k<<1].y1+X[k<<1|1].z1);
	X[k].M2=max(max(X[k<<1].M2,X[k<<1|1].M2),X[k<<1].y2+X[k<<1|1].z2);
}
void push_down(ll k){ //懒标记下传 
	if(X[k].t){
		X[k<<1].t=!X[k<<1].t;
		X[k<<1|1].t=!X[k<<1|1].t;
		SWAP(k<<1);
		SWAP(k<<1|1);
		X[k].t=0;
	}
}
void build(ll k,ll l,ll r){ //初始化 
	X[k].l=l,X[k].r=r;
	X[k].t=0;
	X[k].k=r-l+1;
	if(l==r){
		X[k].M1=X[k].z1=X[k].y1=(a[l]==0);
		X[k].M2=X[k].z2=X[k].y2=(a[l]==1);
		return ;
	}
	ll mid=(l+r)>>1;
	build(k<<1,l,mid);
	build(k<<1|1,mid+1,r);
	pushup(k);
}
void updata(ll k,ll l,ll r){ //区间翻转 
	if(X[k].l==l&&X[k].r==r){
		X[k].t=!X[k].t;
		SWAP(k);
		return ;
	}
	push_down(k);
	ll mid=(X[k].l+X[k].r)>>1;
	if(r<=mid)
	  updata(k<<1,l,r);
	else if(l>mid) 
	  updata(k<<1|1,l,r);
	else{
		updata(k<<1,l,mid);
		updata(k<<1|1,mid+1,r);
	}
	pushup(k);
}
Node search(ll k,ll l,ll r){ //查询 
	if(X[k].l==l&&X[k].r==r)
	  return X[k];
	push_down(k);
	ll mid=(X[k].l+X[k].r)>>1;
	if(r<=mid)
	  return search(k<<1,l,r);
	else if(l>mid)
	  return search(k<<1|1,l,r);
	else{
		Node x=search(k<<1,l,mid),y=search(k<<1|1,mid+1,r),z;
		if(x.z2==x.k)
		  z.z2=x.k+y.z2;
		else
		  z.z2=x.z2;
		if(y.y2==y.k)
		  z.y2=y.k+x.y2;
		else
		  z.y2=y.y2;
		z.k=x.k+y.k;
		z.M2=max(max(x.M2,y.M2),x.y2+y.z2);
		return z;
	}
}
int main(){
    freopen("135.in", "r", stdin);
    freopen("136.out", "w", stdout);
	n=read(),m=read();
	cin>>s;
	for(int i=1;i<=n;i++)
	  a[i]=s[i-1]-'0';
	build(1,1,n);
	while(m--){
		op=read(),x=read(),y=read();
		if(op==1)
		  updata(1,x,y);
		else
		  write(search(1,x,y).M2),putchar('\n');
	}
	return 0;
}