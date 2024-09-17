#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=(n);i++)
#define from(i,b,e) for(int i=(b);i<=(e);i++)
#define reverse(i,e,b) for(int i=(e);i>=(b);i--)
#define main() main(int argc, char const *argv[])
#define optimizeIO std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);
#define chkMax(base,cmp) base=std::max((base),(cmp))
#define chkMin(base,cmp) base=std::min((base),(cmp))
#define update(base,op,modify) base=op((base),(modify))
#define ensure(con,v) if(!(con))v=false;
#define optional(ptr) if(ptr)ptr
#define never if(0)
#define always if(1)
#define rollback(v, bak) ((v)?(v):(bak))
bool DEBUG_MODE=false;


const int maxN = 103, maxK = 6;
int N, K, P;
long long dp[maxN][maxK][maxK][maxK][maxK][maxK];  // 前 i 种方案，对应属性不低于下标
int C[maxN], A[maxN][maxK];

int main() {
    initDebug;
    scanf("%d%d%d", &N, &K, &P);
    upto(i, N) {
        scanf("%d", C+i);
        upto(j, K)  scanf("%d", &A[i][j]);
    }
    memset(dp,0x3f,sizeof(dp));
	dp[0][0][0][0][0][0] = 0;
	for(int i = 1;i <= N;i++){
		from(j, 0, (K>=1?P :0)){
			from(k, 0, (K>=2?P :0)){
				from(l, 0, (K>=3?P :0)){
					from(m, 0, (K>=4?P :0)){
						from(n, 0, (K>=5?P :0)){
							chkMin(dp[i][j][k][l][m][n],dp[i-1][j][k][l][m][n]);  // 从上一个i继承
							from(o, std::max(0,j-A[i][1]), j){
								from(p, std::max(0,k-A[i][2]), k){
									from(q, std::max(0,l-A[i][3]), l){
										from(r, std::max(0,m-A[i][4]), m){
											from(s, std::max(0,n-A[i][5]), n){  // 可以加到点j, k, ..., n
												chkMin(dp[i][j][k][l][m][n],dp[i - 1][o][p][q][r][s] + C[i]);   // 比较
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
    long long t = dp[N][(K>= 1?P :0)][(K>=2?P :0)][(K>=3?P :0)][(K>=4?P :0)][(K>=5?P :0)];
	printf("%lld\n", t==0x3f3f3f3f3f3f3f3f?-1 :t);
    return 0;
}