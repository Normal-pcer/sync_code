#include<bits/stdc++.h>
using namespace std;

const int maxn = 2e5+10;
struct node
{
	int mx1,mx2;
};
int a[maxn];
node tree[4*maxn];
int n,q;

void pushup(int p)
{
	tree[p].mx1 = max(tree[p<<1].mx1,tree[p << 1 | 1].mx1); 
}
void build(int p,int l,int r)
{
	if(l == r)
	{
		tree[p].mx1 = a[l];
		return;
	}
	int mid = (l+r) >> 1;
	build(p << 1,l,mid);
	build(p << 1 | 1,mid + 1,r);
	pushup(p);
}
void update(int p,int l,int r,int x,int k)
{
	if(l == r)
	{
		tree[p].mx1 = k;
		return;
	}
	int mid = (l + r) >> 1;
	if(x <= mid)	update(p << 1,l,mid,x,k);
	else	update(p << 1|1,mid+1,r,x,k);
	pushup(p);
}
void solve(int p,int l,int r,int L,int R)
{
	if(L <= l && R >= r)
	{
		swap(tree[p].mx1,tree[p].mx2);
		return;
	}
	
	int mid = (l + r) >> 1;
	if(L <= mid)	solve(p << 1,l,mid,L,R);
	if(R > mid)	solve(p << 1|1,mid+1,r,L,R);
	pushup(p);
}
int query(int p,int l,int r,int x)
{
	if(l == r)
	{
		return l;
	}
	int mid = (l + r) >> 1;
	if(tree[p << 1].mx1 >= x)	return query(p << 1,l,mid,x);
	else if(tree[p << 1|1].mx1 >= x)	return query(p << 1 | 1,mid+1,r,x);
	else	return n+1;
}

int main()
{
	scanf("%d%d",&n,&q);
	for(int i = 1;i <= n;i++)
		scanf("%d",&a[i]);
	a[n+1] = 0x3f3f3f3f;
	build(1,1,n+1);
	while(q--)
	{
		int op,l,r,x;
		scanf("%d%d%d",&op,&l,&r);
		if(op == 1)
		{
			update(1,1,n+1,l,r);
		}
		else
		{
			scanf("%d",&x);
			if(l != 1)	solve(1,1,n+1,1,l-1);
			if(r != n)	solve(1,1,n+1,r+1,n+1);
			printf("%d\n",query(1,1,n+1,x));
			if(l != 1)	solve(1,1,n+1,1,l-1);
			if(r != n)	solve(1,1,n+1,r+1,n+1);
		}
	}
	
	return 0;
}