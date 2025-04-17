#include<bits/stdc++.h>
#define size size_________
using namespace std;
int fa[100010];
int findfa(int x)
{
	return (fa[x]==x)?x:(x=fa[x]=findfa(fa[x]));
}//并查集模板
template< typename T > inline void read(T &x)//快读
{
    char c=getchar();x=0;int f=0;
    for(;!isdigit(c);c=getchar()) f|=(c=='-');
    for(;isdigit(c);c=getchar()) x=((x<<3)+(x<<1)+(c^48));
    x=f?-x:x;
}
int size[100010];
set<int>vis[100010];//用于存储敌对关系
int n,m,q;
int main()
{
	read(n);
	for(int i=0;i<=n;i++)
	{
		fa[i]=i;
		size[i]=1;
	}//初始化并查集
	read(m);
	read(q);
	while(m--)
	{
		int x,y;
		read(x);
		read(y);//存入敌对的点对
		vis[x].insert(y);
		vis[y].insert(x);
	}
	while(q--)
	{
		int x,y;
		read(x);
		read(y);
		int mlf=findfa(x);//找到点对的祖先
		int hcy=findfa(y);
		if(size[mlf]<size[hcy])//启发式合并
		{
			if(vis[mlf].find(hcy)!=vis[mlf].end())//如果在敌对的点对中找到了对方，说明不会联盟
			{
				puts("no!");
				goto egg;
			}
			for (auto i : vis[mlf])//遍历set
			{
				vis[hcy].insert(i);//联盟
				vis[i].erase(mlf);//节省空间
				vis[i].insert(hcy);//联盟
			}
			fa[mlf]=hcy;//合并并查集
			size[hcy]+=size[mlf];//累加size
		}
		else//同上
		{
			if(vis[hcy].find(mlf)!=vis[hcy].end())
			{
				puts("no!");
				goto egg;
			}
			for (auto i : vis[hcy])
			{
				vis[mlf].insert(i);
				vis[i].erase(hcy);
				vis[i].insert(mlf);
			}
			fa[hcy]=mlf;
			size[mlf]+=size[hcy];
		}
		puts("agree.");
		egg:;
	}
	return 0;
}