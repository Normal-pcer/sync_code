#include<bits/stdc++.h>
#define int long long
#define lson(root) (root << 1)
#define rson(root) (root << 1 | 1)
using namespace std;
struct node {int mn , mx;}tree[800010];
int n , x , y , a[200010] , ans;
void pushup(int root) {tree[root].mn = min(tree[lson(root)].mn , tree[rson(root)].mn) , tree[root].mx = max(tree[lson(root)].mx , tree[rson(root)].mx);}
void build(int root , int l , int r)
{
    if(l == r) return tree[root].mn = tree[root].mx = a[l] , void();
    int mid = (l + r) >> 1;
    build(lson(root) , l , mid);
    build(rson(root) , mid + 1 , r);
    pushup(root);
}
int query_mx(int root , int l , int r , int L , int R)
{
    if(L <= l && R >= r) return tree[root].mx;
    int mid = (l + r) >> 1 , ans = 0;
    if(L <= mid) ans = max(ans , query_mx(lson(root) , l , mid , L , R));
    if(R > mid) ans = max(ans , query_mx(rson(root) , mid + 1 , r , L , R));
    return ans;
}
int query_mn(int root , int l , int r , int L , int R)
{
    if(L <= l && R >= r) return tree[root].mn;
    int mid = (l + r) >> 1 , ans = INT_MAX;
    if(L <= mid) ans = min(ans , query_mn(lson(root) , l , mid , L , R));
    if(R > mid) ans = min(ans , query_mn(rson(root) , mid + 1 , r , L , R));
    return ans;
}
signed main()
{
//    froepen("a.in" , "r" , stdin);
//    freopen("a.out" , "w" , stdout);
    
    scanf("%lld%lld%lld" , &n , &x , &y);
    for(int i = 1 ; i <= n ; i ++) scanf("%lld" , &a[i]);
    build(1 , 1 , n);
    for(int i = 1 ; i <= n ; i ++)
    {
        int L = i , R = n;
        while(L < R)
        {
            int mid = (L + R) >> 1;
            if(query_mx(1 , 1 , n , i , mid) < x) L = mid + 1;
            else R = mid;
        }
        int pos1 = L;
        L = i , R = n;
        while(L < R)
        {
            int mid = (L + R + 1) >> 1;
            if(query_mx(1 , 1 , n , i , mid) <= x) L = mid;
            else R = mid - 1;
        }
        int pos2 = L;
        
        if(!(query_mx(1 , 1 , n , i , pos1) == x && query_mx(1 , 1 , n , i , pos2) == x)) pos1 = pos2 = n + 1;
        
//        pos1 - pos2;
        L = i , R = n;
        while(L < R)
        {
            int mid = (L + R) >> 1;
            if(query_mn(1 , 1 , n , i , mid) > y) L = mid + 1;
            else R = mid;
        }
        int pos3 = L;
        L = i , R = n;
        while(L < R)
        {
            int mid = (L + R + 1) >> 1;
            if(query_mn(1 , 1 , n , i , mid) >= y) L = mid;
            else R = mid - 1;
        }
        int pos4 = L;
        
        if(!(query_mn(1 , 1 , n , i , pos3) == y && query_mn(1 , 1 , n , i , pos4) == y)) pos3 = pos4 = n + 1;
        
//        pos3 - pos4;
        if(pos1 > pos3 && pos2 > pos4) swap(pos1 , pos3) , swap(pos2 , pos4);
        if(pos2 == n + 1 || pos4 == n + 1) continue;
        if(pos1 <= pos3 && pos4 <= pos2) ans += pos4 - pos3 + 1;
        else if(pos3 <= pos1 && pos2 <= pos4) ans += pos2 - pos1 + 1;
        else if(pos2 >= pos3) ans += pos2 - pos3 + 1;
//        printf("%lld %lld  %lld %lld\n" , pos1 , pos2 , pos3 , pos4);
    }
    printf("%lld" , ans);
    return 0;
}
