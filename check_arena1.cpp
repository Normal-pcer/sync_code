#include<bits/stdc++.h>
#define ll long long
#define db double
using namespace std;
inline int read() {
	int x = 0; bool op = false;
	char c = getchar();
	while(!isdigit(c))op |= (c == '-'), c = getchar();
	while(isdigit(c))x = (x << 1) + (x << 3) + (c ^ 48), c = getchar();
	return op ? -x : x;
}
const int N = 210;
int n, ans, res;
struct Node {
	int x, y;
	bool operator < (const Node &tmp) const {
		if(x ^ tmp.x)return x < tmp.x;
		else return y < tmp.y;
	}
	Node(int x = 0, int y = 0):x(x), y(y){}
}a[N];
set<int> s;
void solve() {
	sort(a + 1, a + 1 + n);
	for(int i = 1; i < n; i++) {
		s.clear(); s.insert(a[i].y);
		for(int j = i + 1; j <= n; j++) {
			s.insert(a[j].y);
			vector<int> vec(s.begin(), s.end()); //从题解里面学到的奇怪操作，相当于把s中的元素复制到了vec里 
			int side = a[j].x - a[i].x;
			int mini = max(a[i].y, a[j].y) - side, maxi = min(a[i].y, a[j].y);
			if(mini > maxi)continue; //如果下界在上界之上，直接跳过 
			int len = vec.size(), l = 0, r = -1;
			while(r + 1 < len && vec[r + 1] < mini + side)r++; 
			//找到最大的r，使得vec[r]<mini+side
			//之所以要这么找，是因为可能存在上界在[mini+side,maxi+side]之中但是vec[r]不在这个区间之中的情况
			//那么包含它且不包含vec[r+1]的条件也有所转变，即上界在[mini+side,vec[r+1]-1]之间。 
			while(l < len && vec[l] < mini)l++; //找到最小的l，使vec[l]>=mini 
			for(; r < len && (r < 0 || vec[r] <= maxi + side); r++) {//我比较喜欢这种双指针的写法，即确定r枚举l 
				if(r < 0)continue; //r<0直接跳过 
				int tl = max(vec[r] - side, mini);
				int tr = min((r + 1 < len) ? vec[r + 1] - side : maxi + 1, maxi + 1) - 1;
				//如果r+1不存在，那么把上界设为maxi+1（因为后面有一个-1所以不能设为maxi）
				//此时正方形下边界的范围为[tl,tr] 
				while(l < len && vec[l] < tl)l++;//首先提出vec[l]在下边界以下的情况 
				if((vec[r] < mini + side && vec[r + 1] > mini + side) || vec[r] >= mini + side)ans++;
				//细节：此时r增加了1，不管l能不能移动，它都是一种新的情况，需要统计答案
				//当然其中也有不合法的答案，比如说vec[r]在下边界一下，但是上边界根本不可能移动到mini+side以上
				//这种情况就是ver[r+1]正好等于mini+side
				if(vec[r] - vec[l] == side)res++; //去重 
				while(l + 1 < len && vec[l] < tr) {ans++; l++; if(vec[r] - vec[l] == side)res++;}//统计答案+去重 
			}
		}
	}
	return ;
}
int main() {
	n = read(); ans = n + 1; 
	for(int i = 1; i <= n; i++) {int x = read(), y = read(); a[i] = Node(x, y);}
	solve(); 
	for(int i = 1; i <= n; i++)swap(a[i].x, a[i].y);
	solve();
	printf("%d", ans - res / 2);
	return 0;
}
