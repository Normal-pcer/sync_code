#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;
vector<vector<int>> v(1010,vector<int>(1010,0));
void solve(){
    int n, m;
    cin >> n >> m;
    while(m--) {
        int x, y, a, b;
        cin >> x >> y >> a >> b;
        v[x][y]++;
        v[x][b + 1]--;
        v[a + 1][y]--;
        v[a + 1][b + 1]++;
    }
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            v[i][j] += v[i - 1][j] + v[i][j - 1] - v[i - 1][j - 1];
            cout << v[i][j] << ' ';
        }
        cout << "\n";
    }
}
int main(){
    ios::sync_with_stdio(false); 
    cin.tie(0);
    cout.tie(0);
    solve();
    return 0;
}
