#include "libs/debug_macros.hpp"
#include<bits/stdc++.h>
using namespace std;
int main()
{
	int a,o;
	cin>>a;
	std::vector<int> b(a+1),d(a+1);
	for(int i=1;i<=a;i++) cin>>b[i];
	for(int i=1;i<=a;i++)
	{	o=0;
		for(int j=i;j<=a;j++) { 
            if(b[j]>=b[i]) o++;
        }
		for(;d[o]==b[i];)
        {
            if(d[o]==b[i]) o--;
        }
		d[o]=b[i];
        std::cout << o << std::endl;
	}
	for(int i=a;i>0;i--) cout<<d[i]<<" ";
}