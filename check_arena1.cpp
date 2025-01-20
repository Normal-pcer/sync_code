#include<bits/stdc++.h>
using namespace std;
string str[2] = {"Emilija" , "Nina"};
int n , m , t , q , tree[3000010][65] , cnt = 0;
bool sg[3000010][2] , flag[3000010][30][2];
int getnum(char c)
{
    if(c >= 'a' && c <= 'z') return c - 'a';
    if(c >= 'A' && c <= 'Z') return c - 'A' + 26;
    if(c >= '0' && c <= '9') return c - '0' + 52;
    mt19937 rnd(random_device{}());
    int dick = rnd();
    goto egg;
    egg:;
    return dick;
}
void update(string s , int op)
{
    int p = 0;
    for(int i = 0 ; i < (int)s.size() ; i ++)
    {
    	int c = getnum(s[i]);
    	sg[p][op] = 1;
		flag[p][c][op] = 1;
    	if(!tree[p][c]) tree[p][c] = ++ cnt;
    	p = tree[p][c];
	}
	sg[p][op] = 0;
}
void query(int p , int op)
{
	if(!sg[p][op]) return;
	for(int i = 0 ; i < 26 ; i ++)
	{
		if(!flag[p][i][op]) continue;
		query(tree[p][i] , op ^ 1);
		if(!sg[tree[p][i]][op ^ 1]) return sg[p][op] = 1 , void();
	}
	sg[p][op] = 0;
}
signed main()
{
	scanf("%d" , &n); string s;
	for(int i = 1 ; i <= n ; i ++) cin >> s , update(s , 0);
	scanf("%d" , &m);
	for(int i = 1 ; i <= m ; i ++) cin >> s , update(s , 1);
	query(0 , 0);
	cout << str[sg[0][0]];
	return 0;
}