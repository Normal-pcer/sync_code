#include<bits/stdc++.h>
namespace CYaRon
{
	void Error(const char *s){std::cerr<<s<<std::endl;exit(EXIT_FAILURE);}
	
	using siz_type=unsigned;
	using index_type=long long;
	using val_type=int;
	
	class Input;
	class Output;
	
	class Variable;
	class VarList;
	
	val_type read_expression(char);
	val_type& read_variable(char);
	bool read_con();
	bool read_vars();
	bool read_instruction();
	
	void jump();
	void run();
	
	class Variable:public std::vector<val_type>
    {
        private:
            int L,R;
        public:
            Variable(){}
            Variable(index_type __minimum_index,index_type __maximun_index):
                std::vector<val_type>(__maximun_index-__minimum_index+1,0),L(__minimum_index),R(__maximun_index){}
            val_type& operator[] (const index_type &x){if(x<L||x>R)throw "数组越界";return *(this->begin()+(x-L));}
    };
	class VarList:public std::unordered_map<std::string,Variable>
	{
		public:
			Variable& operator[] (const std::string &x)
			{
				auto it=this->find(x);
				if(it!=this->end())return it->second;
				throw "未定义变量";
			}
	}varlist;
	class Input
	{
		private:
			char buf[1000010];int t;
		public:
			Input(){buf[fread(buf,1,1000000,stdin)]=EOF;t=0;}
			char get(){return buf[t++];}
			int getpos(){return t;}
			void setpos(int _t){t=_t;}
	}In;
	class Output
	{
		private:
			char buf[1000010];int t;
		public:
			Output():t(){}
			~Output(){if(t)fwrite(buf,1,t,stdout);}
            void putc(char c){buf[t++]=c;}
			void put(int a)
			{
				char buf[20],len=0;
                if(a<0)putc('-'),a=-a;
                do buf[len++]=a%10+'0',a/=10; while(a);
                while(len)putc(buf[--len]);putc(' ');
			}
	}Out;
	val_type read_expression(char end='\n')
	{
		char ch=In.get();
		val_type sum=0;
		bool vis=false,type=false,typ1=false;
		std::string s;
		while(ch!=end)
		{
			if(ch==EOF)throw "非法字符";
			if(isgraph(ch)&&!isdigit(ch)&&!isalpha(ch)&&ch!='['&&ch!='+'&&ch!='-')throw "非法字符";
			if(typ1&&isgraph(ch)&&ch!='+'&&ch!='-')throw "非法字符";
			if(isgraph(ch)&&!vis)
				vis=true,type=isalpha(ch);
			if(isdigit(ch))
				if(!type)sum=sum*10+ch-'0';
				else s+=ch;
			else if(isalpha(ch))
				if(!type)throw "非法字符";
				else s+=ch;
			else if(ch=='[')
				if(!type)throw "非法字符";
				else sum=read_expression(']'),typ1=true;
			else if(ch=='+')
				return (type?varlist[s][sum]:sum)+read_expression(end);
			else if(ch=='-')
				return (type?varlist[s][sum]:sum)-read_expression(end);
			ch=In.get();
		}
		return type?varlist[s][sum]:sum;
	}
	val_type& read_variable(char end)
	{
		char ch=In.get();
		std::string s;int x=0;
		bool type=false;
		while(ch!=end)
		{
			if(ch==EOF)throw "非法字符";
			if(isgraph(ch)&&!isdigit(ch)&&!isalpha(ch)&&ch!='[')throw "非法字符";
			if(type&&isgraph(ch))throw "非法字符";
			if(isgraph(ch)&&ch!='[')
				s+=ch;
			else if(ch=='[')
				x=read_expression(']'),type=true;
			ch=In.get();
		}
		return varlist[s][x];
	}
	bool read_vars()
	{
		char ch;
		do ch=In.get(); while(!isgraph(ch));
		if(ch=='}')return false;
		std::string s,s1;
		int l=0,r=0;
		do s+=ch,ch=In.get(); while(isalpha(ch));
		while(ch!=':')ch=In.get();
		do ch=In.get(); while(!isgraph(ch));
		do s1+=ch,ch=In.get(); while(isalpha(ch));
		if(s1[0]=='a')
		{
			while(ch!=',')ch=In.get();
			l=read_expression('.');
			ch=In.get();
			r=read_expression(']');
		}
		varlist.emplace(s,Variable(l,r));
		return true;
	}
	bool read_con()
	{
		std::string s;char ch;
		do ch=In.get(); while(!isgraph(ch));
		do s+=ch,ch=In.get(); while(isgraph(ch)&&ch!=',');
		while(ch!=',')ch=In.get();
		int x=read_expression(','),y=read_expression();
		switch(s[0]*200+s[1])
		{
			case 'l'*200+'t':return x<y;break;
			case 'g'*200+'t':return x>y;break;
			case 'l'*200+'e':return x<=y;break;
			case 'g'*200+'e':return x>=y;break;
			case 'e'*200+'q':return x==y;break;
			case 'n'*200+'e':return x!=y;break;
			default: throw "someting wrong";
		}
	}
	void jump(){int x=1;char ch;while(x){ch=In.get();if(ch=='{')++x;else if(ch=='}')--x;}}
	bool read_instruction()
	{
		char ch;
		do ch=In.get(); while((~ch)&&!isgraph(ch));
		if(ch==EOF||ch=='}')return false;
		if(ch=='#'){while(ch!='\n')ch=In.get();return true;}
		std::string s;
		do ch=In.get(); while(!isgraph(ch));
		do s+=ch,ch=In.get(); while(isgraph(ch));
		switch(s[0])
		{
			case 'v':while(read_vars());break;
			case 'y':Out.put(read_expression());break;
			case 's':{int &x=read_variable(',');x=read_expression();break;}
			case 'i':if(read_con())run();else jump();break;
			case 'h':
			{
				int &x=read_variable(','),l=read_expression(','),r=read_expression(),pos=In.getpos();
				bool flg=false;
				for(x=l;x<=r;x++)
				In.setpos(pos),run(),flg=true;
				if(!flg)jump();
				break;
			}
			case 'w':{int pos=In.getpos();while(read_con())run(),In.setpos(pos);jump();break;}
			default:throw "错误语句";
		}
		return true;
	}
	void run(){while(read_instruction());}
}
int main()
{
	try
    {
        CYaRon::run();
    }
    catch(const char *e)
    {
        CYaRon::Error(e);
    }
	return 0;
}
#if false
P3695代码（原版）

#include<bits/stdc++.h>
namespace CYaRon
{
	void Error(const char *s)
	{
		std::cerr<<s<<std::endl;
		exit(EXIT_FAILURE);
	}
	using siz_type=unsigned;
	using index_type=long long;
	using val_type=int;
	class Variable
	{
		private:
			val_type *data;
			siz_type len;
			index_type L,R;
		public:
			Variable(){data=NULL;}
			Variable(siz_type __len):len(__len),L(0),R(0){data=new int[1]();}
			Variable(index_type __minimum_index,index_type __maximun_index):
				len(__maximun_index-__minimum_index+1),L(__minimum_index),R(__maximun_index){data=new int[len]();}
			void set(int l,int r){L=l;R=r;len=r-l+1;data=new int[len]();}
			~Variable(){if(data)delete[] data;}
			val_type& operator[] (const index_type &x){if(x<L||x>R)throw "数组越界";return data[x-L];}
	};
	class VarList:public std::unordered_map<std::string,Variable>
	{
		public:
			Variable& operator[] (const std::string &x)
			{
				auto it=this->find(x);
				if(it!=this->end())return it->second;
				throw "未定义变量";
			}
	}varlist;
	class Input
	{
		private:
			char buf[1000010];int t;
		public:
			Input(){buf[fread(buf,1,1000000,stdin)]=EOF;t=0;}
			char get(){return buf[t++];}
			int getpos(){return t;}
			void setpos(int _t){t=_t;}
	}In;
	class Output
	{
		private:
			char buf[1000010];int t;
		public:
			Output():t(){}
			~Output(){if(t)fwrite(buf,1,t,stdout);}
            void putc(char c){buf[t++]=c;}
			void put(int a)
			{
				char buf[20],len=0;
                if(a<0)putc('-'),a=-a;
                do buf[len++]=a%10+'0',a/=10; while(a);
                while(len)putc(buf[--len]);putc(' ');
			}
	}Out;
	val_type read_expression(char end='\n')
	{
		char ch=In.get();
		val_type sum=0;
		bool vis=false,type=false,typ1=false;
		std::string s;
		while(ch!=end)
		{
			if(ch==EOF)throw "非法字符";
			if(isgraph(ch)&&!isdigit(ch)&&!isalpha(ch)&&ch!='['&&ch!='+'&&ch!='-')throw "非法字符";
			if(typ1&&isgraph(ch)&&ch!='+'&&ch!='-')throw "非法字符";
			if(isgraph(ch)&&!vis)
				vis=true,type=isalpha(ch);
			if(isdigit(ch))
				if(!type)sum=sum*10+ch-'0';
				else s+=ch;
			else if(isalpha(ch))
				if(!type)throw "非法字符";
				else s+=ch;
			else if(ch=='[')
				if(!type)throw "非法字符";
				else sum=read_expression(']'),typ1=true;
			else if(ch=='+')
				return (type?varlist[s][sum]:sum)+read_expression(end);
			else if(ch=='-')
				return (type?varlist[s][sum]:sum)-read_expression(end);
			ch=In.get();
		}
		return type?varlist[s][sum]:sum;
	}
	val_type& read_variable(char end)
	{
		char ch=In.get();
		std::string s;int x=0;
		bool type=false;
		while(ch!=end)
		{
			if(ch==EOF)throw "非法字符";
			if(isgraph(ch)&&!isdigit(ch)&&!isalpha(ch)&&ch!='[')throw "非法字符";
			if(type&&isgraph(ch))throw "非法字符";
			if(isgraph(ch)&&ch!='[')
				s+=ch;
			else if(ch=='[')
				x=read_expression(']'),type=true;
			ch=In.get();
		}
		return varlist[s][x];
	}
	bool read_vars()
	{
		char ch;
		do ch=In.get(); while(!isgraph(ch));
		if(ch=='}')return false;
		std::string s,s1;
		int l=0,r=0;
		do s+=ch,ch=In.get(); while(isalpha(ch));
		while(ch!=':')ch=In.get();
		do ch=In.get(); while(!isgraph(ch));
		do s1+=ch,ch=In.get(); while(isalpha(ch));
		if(s1[0]=='a')
		{
			while(ch!=',')ch=In.get();
			l=read_expression('.');
			ch=In.get();
			r=read_expression(']');
		}
		varlist.emplace(s,Variable());
		varlist[s].set(l,r);
		return true;
	}
	bool read_con()
	{
		std::string s;char ch;
		do ch=In.get(); while(!isgraph(ch));
		do s+=ch,ch=In.get(); while(isgraph(ch)&&ch!=',');
		while(ch!=',')ch=In.get();
		int x=read_expression(','),y=read_expression();
		switch(s[0]*200+s[1])
		{
			case 'l'*200+'t':return x<y;break;
			case 'g'*200+'t':return x>y;break;
			case 'l'*200+'e':return x<=y;break;
			case 'g'*200+'e':return x>=y;break;
			case 'e'*200+'q':return x==y;break;
			case 'n'*200+'e':return x!=y;break;
			default: throw "someting wrong";
		}
	}
	void jump(){int x=1;char ch;while(x){ch=In.get();if(ch=='{')++x;else if(ch=='}')--x;}}
	void run();
	bool read_instruction()
	{
		char ch;
		do ch=In.get(); while((~ch)&&!isgraph(ch));
		if(ch==EOF||ch=='}')return false;
		if(ch=='#'){while(ch!='\n')ch=In.get();return true;}
		std::string s;
		do ch=In.get(); while(!isgraph(ch));
		do s+=ch,ch=In.get(); while(isgraph(ch));
		switch(s[0])
		{
			case 'v':while(read_vars());break;
			case 'y':Out.put(read_expression());break;
			case 's':{int &x=read_variable(',');x=read_expression();break;}
			case 'i':if(read_con())run();else jump();break;
			case 'h':
			{
				int &x=read_variable(','),l=read_expression(','),r=read_expression(),pos=In.getpos();
				bool flg=false;
				for(x=l;x<=r;x++)
				In.setpos(pos),run(),flg=true;
				if(!flg)jump();
				break;
			}
			case 'w':{int pos=In.getpos();while(read_con())run(),In.setpos(pos);jump();break;}
			default:throw "错误语句";
		}
		return true;
	}
	void run(){while(read_instruction());}
}
int main()
{
	try
    {
        CYaRon::run();
    }
    catch(const char *e)
    {
        CYaRon::Error(e);
    }
	return 0;
}
#endif