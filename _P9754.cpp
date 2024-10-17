#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

ll curraddr;

struct T{
	string name;
	ll siz,align;
	// 结构体类型名称 
	// 结构体类型占用空间 
	// 结构体类型内存对齐规则 
	vector<tuple<T*,string> > ch;
	T(){
		siz=align=0;
		ch.clear();
	}
	T(string a,ll b,ll c){
		name=a;
		siz=b,align=c;
		ch.clear();
	}
};

struct S{
	string name;
	T type;
	ll addr;
	// 结构体名称
	// 结构体类型
	// 结构体起始地址 
	S(){
		name=addr=0;
	}
};

int cnt;
map<string,ll> mp;
map<string,S> strs;
T types[100000];
string ts[100000],n[100000];

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	types[++cnt]=T("byte",1,1);
	mp["byte"]=cnt;
	types[++cnt]=T("short",2,2);
	mp["short"]=cnt;
	types[++cnt]=T("int",4,4);
	mp["int"]=cnt;
	types[++cnt]=T("long",8,8);
	mp["long"]=cnt;
	int q;
	cin>>q;
	while (q--){
		int op;
		cin>>op;
		if (op==1){
			string s;
			int k;
			cin>>s>>k;
			T t;
			t.name=s;
			ll curr=0;
			for (int i=1;i<=k;i++){
				cin>>ts[i]>>n[i];
				int id=mp[ts[i]];
				t.ch.push_back(make_tuple(&types[id],n[i]));
				t.align=max(t.align,types[id].align);
				if (curr%types[id].align) curr=(curr/types[id].align+1)*types[id].align;
				curr+=types[id].siz;
			}
			if (curr%t.align) curr=(curr/t.align+1)*t.align;
			t.siz=curr;
			types[++cnt]=t;
			mp[s]=cnt;
			cout<<t.siz<<" "<<t.align<<"\n";
		}else if (op==2){
			string t,n;
			cin>>t>>n;
			if (curraddr%types[mp[t]].align) curraddr=(curraddr/types[mp[t]].align+1)*types[mp[t]].align;
			S s;
			s.addr=curraddr;
			s.name=n;
			s.type=types[mp[t]];
			strs[n]=s;
			curraddr+=s.type.siz;
			cout<<s.addr<<"\n";
		}else if (op==3){
			string s;
			cin>>s;
			queue<string> q;
			while (s.find(".")!=string::npos){
				q.push(s.substr(0,s.find(".")));
				s=s.substr(s.find(".")+1);
			}
			if (not s.empty()) q.push(s);
			ll addr=strs[q.front()].addr;
			T now=strs[q.front()].type;
			q.pop();
			while (not q.empty()){
				bool flag=true;
				for (auto [nxt,n]:now.ch){
					if (addr%nxt->align) addr=(addr/nxt->align+1)*nxt->align;
					if (n==q.front()){
						q.pop();
						now=*nxt;
						flag=false;
						break;
					}else{
						addr+=nxt->siz;
					}
				}
				if (flag){
					cout<<"ERR\n";
					break;
				}
			}
			cout<<addr<<"\n";
		}else if (op==4){
			ll tar;
			string ans;
			cin>>tar;
			if (tar>=curraddr){
				cout<<"ERR\n";
				continue;
			}
			for (auto [ignore,i]:strs){
				if (i.addr<=tar and tar<i.addr+i.type.siz){
					ans=i.name;
					tar-=i.addr;
					break;
				}
			}
			if (ans.empty()){
				cout<<"ERR\n";
			}else{
//				cout<<ans;
				T now=strs[ans].type;
				ll st=0,ed=0;
				while (not now.ch.empty()){
//					cout<<ans<<"**\n";
					bool flag=true;
					for (auto [nxt,n]:now.ch){
//						cout<<n<<"*\n";
						st=ed;
						if (st%nxt->align) st=(st/nxt->align+1)*nxt->align;
						if (st>tar){
							ans+="ERR";
							flag=true;
							break;
						}
						ed=st+nxt->siz;
//						cout<<tar<<" "<<st<<" "<<ed<<"\n";
						if (st<=tar and tar<ed){
							ans.push_back('.');
							ans+=n;
//							cout<<"."<<n;
//							cout<<ans<<"\n";
							now=*nxt;
							flag=false;
							break;
						}
					}
					if (flag){
						ans+="ERR";
						break;
					}
				}
//				cout<<"\n";
				if (ans.find("ERR")==string::npos){
					cout<<ans<<"\n";
				}else{
					cout<<"ERR\n";
				}
			}
		}
	}
	return 0;
}