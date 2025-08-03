#include<bits/stdc++.h>
using namespace std;
int n , T , tree[20000010] , a[110] , dp[10000010] , ans;
inline int lowbit(int x) {return x & -x;}
void update(int x , int val) {for(x ++ ; x <= 300 ; x += lowbit(x)) tree[x] = max(tree[x] , val);}
int query(int x) {int ans = 0; for(x ++ ; x >= 1 ; x -= lowbit(x)) ans = max(ans , tree[x]); return ans;}
int main()
{
//    freopen("data.in" , "r" , stdin);
    scanf("%d%d" , &n , &T);
    for(int i = 1 ; i <= n ; i ++) scanf("%d" , &a[i]);
    vector<int> v;
    for(int i = 1 ; i <= T ; i ++) for(int j = 1 ; j <= n ; j ++) v.push_back(a[j]);
    for(int i = 0 ; i < (int)v.size() ; i ++) dp[i] = query(v[i]) + 1 , update(v[i] , dp[i]);
    for(int i = 0 ; i < (int)v.size() ; i ++) ans = max(ans , dp[i]);
    printf("%d" , ans);
    return 0;
}