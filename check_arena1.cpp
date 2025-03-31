#include<bits/stdc++.h>
using namespace std;
int dis[60][60],n,m;
bool G[60][60][65];
/*以上是变量说明部分，dis[i][j]表示i到j的路径/边的长度
G[i][j][k]表示，i到j是否存在一条长度为2^k的路径
如果有，为true，没有就是false*/ 
void init()
{
    memset(G,false,sizeof(G));
    memset(dis,10,sizeof(dis));
    scanf("%d%d",&n,&m);
    for(int i=1;i<=m;i++)
    {
        int x,y;
        scanf("%d%d",&x,&y);
        dis[x][y]=1;
        G[x][y][0]=true;
        /*初始化，x到y的路径（边）最短是1，也就是x到y存在
        一条长度为2^0的路径（边）*/ 
    }
}
void work()//此函数对G和dis做预处理 
{
    for(int k=1;k<=64;k++)
    //对于本题的数据，2^64已经足够。 
    for(int i=1;i<=n;i++)
    for(int t=1;t<=n;t++)
    for(int j=1;j<=n;j++)
    //枚举三个点
    if(G[i][t][k-1]&&G[t][j][k-1])
    /*如果i到t存在一条2^k-1长度的路径
    并且t到j存在一条2^k-1长度的路径
    就说明i到t，t到j都可以一秒到达，
    路程*2刚好是2的幂，也可以一秒到达*/ 
    {
        G[i][j][k]=true;
        //标记从i到j存在一条长度为2^k的路径 
        dis[i][j]=1;
        //i到j距离可以一秒到达 
    }
}
void floyd()
{
    for(int k=1;k<=n;k++)
    //这里的注意点：枚举中间点的循环放在最前面 
    for(int i=1;i<=n;i++)
    for(int j=1;j<=n;j++)
    dis[i][j]=min(dis[i][j],dis[i][k]+dis[k][j]);
    //松弛操作。 
}//Floyd图论求最短路。 
int main()
{
    init();
    work();
    floyd();
    printf("%d",dis[1][n]);
    return 0;
}