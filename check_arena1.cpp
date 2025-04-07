#include <bits/stdc++.h>
using namespace std;
#define validx(a) ((a) >= 1 && (a) <= s)
int r, s, sx;
char c[301][301];

// 一个 node 代表 DP 的一个状态
struct node {
	int len;
	unsigned seq[10];
	// seq 记录括号序列，每个二进制位上 0 代表 '('，1代表 ')'
	// 用 10 个 32 位整数可以存下至多 320 个括号。
	// 不用 std::bitset，因为它没有提供比较运算符。

	void init(int x, unsigned y) {
		len = x;
		memset(seq, 0, sizeof seq);
		seq[0] = y;
	}
	void set(unsigned x) {
		seq[x/32] |= 1u<<x%32;
	}
} nd[2][301][152];

// 比较括号序列的大小。
bool cmpSeq(const node &a, const node &b) {
	for(int i = (a.len-1)/32+1; i >= 0; --i) {
		if(a.seq[i] != b.seq[i]) return a.seq[i] < b.seq[i];
	}
	return 0;
};

ostream &operator<<(ostream &out, const node &x) {
	out<<x.len<<"\n";
	for(int i = x.len-1; i >= 0; --i)
		out<<(x.seq[i/32] & 1u<<i%32 ? ')' : '(');
	return out;
}

int main() {
	// ifstream cin("r:/test/retro.in");
	ios::sync_with_stdio(0);
	cin>>r>>s;
	memset(nd, -1, sizeof nd);
	for(int i = 1; i <= r; i++)
		for(int j = 1; j <= s; j++) {
			cin>>c[i][j];
		}
	// sx = 'M' 所在的横坐标
	sx = find(c[r]+1, c[r]+s+1, 'M') - c[r];
	for(int j = 1; j <= s; j++)
		if(c[1][j] == ')') {
			nd[1][j][1].init(1, 1);
		} else if(c[1][j] != '(') {
			nd[1][j][0].init(0, 0);
		}
	// 将 DP 转移中大量重复的代码写成函数，节省码量。
	// 注意：为了方便，这个函数中没有向 seq 添加新的括号。
	auto dpUpt = [](const node &cur, node &nxt, int df) -> bool {
		if(cur.len+df > nxt.len || (cur.len+df == nxt.len && cmpSeq(cur, nxt))) {
			nxt = cur;
			nxt.len = cur.len+df;
			return 1;
		}
		return 0;
	};
	for(int i = 1; i < r; ++i) {
		// 及时清空滚动数组。
		memset(nd[i+1 & 1], -1, sizeof nd[0]);
		for(int j = 1; j <= s; j++) {
			// 注意：非首行的炸弹作为起点，只能在滚动到此时进行初始化。
			if(c[i+1][j] == '*') nd[i+1 & 1][j][0].init(0, 0);
			// 只处理序列存在的节点。
			for(int k = 0; k <= r/2 && k <= i; ++k) if(nd[i & 1][j][k].len >= 0) {
				// 注意：为了方便，在新一轮 DP 开始前向 seq 中添加括号。
				// 因为二进制用 0 代表左括号，所以只用管右括号。
				if(c[i][j] == ')') {
					nd[i & 1][j][k].set(nd[i & 1][j][k].len-1);
				}
				for(int dx: {-1, 0, 1}) if(validx(j+dx)) {
					if(c[i+1][j+dx] == ')') {
						dpUpt(nd[i & 1][j][k], nd[i+1 & 1][j+dx][k+1], 1);
					} else if(c[i+1][j+dx] == '(') {
						if(k > 0)
							dpUpt(nd[i & 1][j][k], nd[i+1 & 1][j+dx][k-1], 1);
					} else if(c[i+1][j+dx] != '*') {
						dpUpt(nd[i & 1][j][k], nd[i+1 & 1][j+dx][k], 0);
					}
				}
			}
		}
	}
	cout<<nd[r & 1][sx][0]<<endl;
	return 0;
}