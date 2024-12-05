#include<bits/stdc++.h>
using namespace std;
#define MAXN 50005
namespace XDS{
    #define Pair pair<int,double>
    #define MP(x,y) (make_pair(x,y))
    #define F(x) (x.first)
    #define S(x) (x.second)
    #define XDSsiz 3000005
	Pair merge(Pair x,Pair y){return (F(x)==F(y)?MP(F(x),S(x)+S(y)):(F(x)>F(y)?x:y));}  
    Pair a[XDSsiz];
    int lson[XDSsiz],rson[XDSsiz];
    int root,tot;
    void modify(int x,int y,int idx,Pair v,int &p,int k){
        if(!p)p=++tot;
		if(x==y){
        	if(!k)
            	a[p]=merge(a[p],v);
            else a[p]=v;
			return;
        }
        int mid=(x+y)>>1;
        if(idx<=mid)modify(x,mid,idx,v,lson[p],k);
        if(idx>mid)modify(mid+1,y,idx,v,rson[p],k);
        a[p]=merge(a[lson[p]],a[rson[p]]);
    }
    Pair query(int x,int y,int l,int r,int &p){
        if(!p)return MP(0,0);
		if(l<=x&&r>=y)
            return a[p];
        int mid=(x+y)>>1;
        Pair ans=MP(0,0);
        if(l<=mid)ans=query(x,mid,l,r,lson[p]);
        if(r>mid)ans=merge(ans,query(mid+1,y,l,r,rson[p]));
        return ans;
    }
}
int x[MAXN],y[MAXN],n;
double ans[MAXN];
int mx,mx1;
int idx[MAXN],b[MAXN];
Pair c[MAXN];
bool pd[MAXN];
Pair F[MAXN][2];
int OP;
bool cmp(int xx,int yy){
	if(x[xx]!=x[yy])return x[xx]<x[yy];
	return y[xx]<y[yy];
}
void init(){
	for(int i=1;i<=n;++i)
		x[i]=mx1-x[i]+1,y[i]=mx-y[i]+1;
	for(int i=1;i<=n;++i)
		idx[i]=OP==0?i:n-i+1;
}
void cdq(int l,int r){
	if(l==r)return;
	int mid=(l+r)>>1;
	cdq(l,mid);
	for(int i=mid+1;i<=r;++i)
		b[i]=idx[i];
	sort(idx+mid+1,idx+r+1,cmp);
	int tot1=l,tot2=mid+1;
	for(int i=l;i<=r;++i){
		if((x[idx[tot1]]<=x[idx[tot2]]&&tot1<=mid)||tot2>r){
			XDS::modify(1,mx,y[idx[tot1]],F[idx[tot1]][OP],XDS::root,0);
			tot1++;
		}
		else{
			Pair t=XDS::query(1,mx,1,y[idx[tot2]],XDS::root);
			F(t)++;
			F[idx[tot2]][OP]=XDS::merge(F[idx[tot2]][OP],t);
			tot2++;
		}
	}
	for(int i=l;i<=mid;++i)
		XDS::modify(1,mx,y[idx[i]],MP(0,0),XDS::root,1);
	for(int i=mid+1;i<=r;++i)
		idx[i]=b[i];
	cdq(mid+1,r);
	tot1=l,tot2=mid+1;
	for(int i=l;i<=r;++i){
		if((x[idx[tot1]]<=x[idx[tot2]]&&tot1<=mid)||tot2>r)
			b[i]=idx[tot1++];
		else
			b[i]=idx[tot2++];
	}
	for(int i=l;i<=r;++i)
		idx[i]=b[i];
}
int main()
{
    cin>>n;
    for(int i=1;i<=n;++i)
		scanf("%d%d",&x[i],&y[i]),mx1=max(mx1,x[i]),mx=max(mx,y[i]);
	init();
	for(int i=1;i<=n;++i)
		F[i][OP]=MP(1,1);
	cdq(1,n);
	OP^=1;
	init();
	for(int i=1;i<=n;++i)
		F[i][OP]=MP(1,1);
	cdq(1,n);
	double sum=0;
	int mxans=0;
	for(int i=1;i<=n;++i)
		mxans=max(mxans,F(F[i][0]));
	for(int i=1;i<=n;++i)
		if(F(F[i][0])==mxans)sum+=S(F[i][0]);
	for(int i=1;i<=n;++i)
		if(F(F[i][0])+F(F[i][1])-1==mxans)ans[i]=(S(F[i][0])*S(F[i][1]))*1.0/sum;
	printf("%d\n",mxans);
	for(int i=1;i<=n;++i)
		printf("%.8lf ",ans[i]);
}