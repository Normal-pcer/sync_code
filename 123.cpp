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
#define ensure(con, otw) ((con)? (con): (otw))
#define check(v, con, otw) (((v) con)? v: otw)
#define optional(ptr) if(ptr)ptr
#define never if(0)
#define always if(1)
bool DEBUG_MODE=false;


const int maxN = 100005;
int N;
int A[maxN], B[maxN];
int bucket[maxN];  // 数字i对应a中标号
int C[maxN];  // b[i]在a中标号
int t[maxN], top=0;  // 长度为i的上升子序列可能的最小结尾数

int *lower(int *begin, int *end, int val) {
    while (end-begin-1) {
        int *mid=begin+((end-begin)>>1);
        if (*mid==val)  return mid;
        if (*mid>val)  end=mid;
        else  begin=mid;
    }
    return end;
}

int main() {
    initDebug;
    optimizeIO;
    std::cin >> N;
    upto(i, N)  scanf("%d", A+i);
    upto(i, N)  scanf("%d", B+i);
    upto(i, N)  bucket[A[i]] = i;
    upto(i, N)  C[i] = bucket[B[i]];
    // 此时b[i]在b中标号事实上为1, 2, 3, ..., n
    // 而在a中标号为c[1], c[2], c[3], ..., c[n]
    // 只需找出c的最长上升子序列
    upto(i, N) {
        if (t[top] < C[i])  t[++top] = C[i];
        else  *lower(t+1, t+top+1, C[i]) = C[i];
    }
    std::cout << top << std::endl;
    return 0;
}