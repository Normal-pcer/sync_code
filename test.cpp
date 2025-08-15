//#pragma GCC optimize("Ofast")
#include<bits/stdc++.h>
#define rv std::views::reverse
#define in :
using namespace std;
using std::ranges::iota_view;
/*
注意下面的range实现的是分别是
[1,n]
[l,r]
是闭区间不要被误导
*/
auto range(int end) {
    return iota_view(1, end + 1);
}
auto range(int start, int end) {
return iota_view(start, end + 1);
}
const int N = 1005;
const int mo = 1e9 + 7;
int a[N], b[N], dp[N][N];
int inv[N];
long long s[N][N];  // 修改为 s[j][k] 的形式
int l[N], r[N];
int c[N][N];
int fp(long long x, int y) {
    long long res = 1;
    for (; y; y >>= 1, x = x * x % mo) {
        if (y & 1)res = res * x % mo;
    }
    return res;
}
int calc(int L, int m) {
    long long ans = 1;
    for (auto i in range(L, L + m - 1))ans = ans * i % mo;
    for (auto i in range(m))ans = ans * inv[i] % mo;
    return ans;
}
signed main() {
//    freopen("1_01.in", "r", stdin);
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    set<int> ss;
    //离散化区间
    vector<long long> v = {1};
    for (int i=1;i<= n;i++) {
        cin>>a[i]>>b[i];
        v.push_back(a[i]);
        v.push_back(b[i] + 1);
    }
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
    int cnt=0;
    for (int i=0;i<v.size()-1;++i) {
        int flag=1;
        for(auto j in range(n)){
            if(((a[j]<=v[i])&&(b[j]>=v[i+1]-1)))flag=0;//如果这段区间没有任何船能取到我们可以不记录
        }
        if(flag)continue;
        l[++cnt]=v[i];
        r[cnt]=v[i+1]- 1;
//        cout<<cnt<<" "<<l[cnt]<<" "<<r[cnt]<<"\n";
    }
//    cout<<cnt<<"\n";
    inv[1] = 1;
    for (int i in range(2, n)) {
        inv[i] = (long long)(mo - mo / i) * inv[mo % i] % mo;
    }
    for (auto i in range(cnt)) {
        // for (auto j in range(0, n)) {
        //     c[i][j] = calc(r[i] - l[i] + 1, j);//预处理组合数，因为L可能很大所以我们需要暴力算
        // }
    }

        
    for (auto i in range(cnt)) {
        auto L = r[i] - l[i] + 1;
        c[i][0] = 1;
        for (int j = 1; j <= n; j++) {
            c[i][j] = (long long)c[i][j - 1] * (L + j + 1) % mo;
            c[i][j] = (long long)c[i][j] * inv[j] % mo;
            assert(c[i][j] == calc(L, j));
        }
    }
    long long ans = 0;
    for (auto i in range(0, cnt))s[i][0] = 1;  // 初始化改为 s[i][0] = 1
    for (auto i in range(n)) {
        for (auto j in range(cnt)) {
            if (!((a[i] <= l[j]) && (b[i] >= r[j]))) continue;//不在这个区间肯定是0种方案
            int m = 1;
            for (auto k in range(0, i - 1)|rv) {
                dp[i][j] = (dp[i][j] + s[j - 1][k] * c[j][m]) % mo;  // 改为 s[j-1][k]
                m += (a[k] <= l[j]) && (b[k] >= r[j]);
            }
            ans += dp[i][j];//累加是因为可以后面的船全都没有取
            ans %= mo;
        }
        for (auto j in range(cnt)) {
//            cout<<format("dp[{}][{}]={}\n",i,j,dp[i][j]);
            s[j][i] = (s[j][i] + (j > 0 ? s[j - 1][i] : 0) + dp[i][j]) % mo;  // 改为 s[j][i]
        }
    }
    cout << ans << "\n";
}