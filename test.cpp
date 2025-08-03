#pragma GCC target("avx,avx2,fma")
#include<bits/stdc++.h>
#include<immintrin.h>

using ui=unsigned;
using ll=long long;
using ull=unsigned long long;
using i128=__int128;
using u128=unsigned __int128;
using m256d=__m256d;

#define load(x) _mm256_loadu_pd((double*)(x))
#define store(x,y) _mm256_storeu_pd((double*)(x),(y))

inline m256d mul2cp(const m256d &x,const m256d &y){return _mm256_fmaddsub_pd(_mm256_movedup_pd(x),y,_mm256_permute_pd(x,15)*_mm256_permute_pd(y,5));}
inline m256d make22cp(double x,double y){return _mm256_set_pd(y,x,y,x);}
inline m256d make2(const std::complex<double> &x){return make22cp(x.real(),x.imag());}
inline void mul_n(const std::complex<double> *a,const std::complex<double> &b,const int n,std::complex<double> *c){
	m256d x=make2(b);int i=0;
	for (;i+1<n;i+=2) store(c+i,mul2cp(load(a+i),x));
	for (;i!=n;i++) c[i]=a[i]*b;
}
inline void mul_n(const std::complex<double> *a,const std::complex<double> *b,const int n,std::complex<double> *c){
	int i=0;
	for (;i+1<n;i+=2) store(c+i,mul2cp(load(a+i),load(b+i)));
	for (;i!=n;i++) c[i]=a[i]*b[i];
}

namespace FFT{
	const double pi=std::acos(-1);
	std::vector<std::complex<double>> rt={{1,0},{0,1}};
	void init(int n){
		int nrt=rt.size();
		if (nrt>=(n>>1)) return;
		rt.resize(n>>1);
		for (;nrt<(n>>1);nrt<<=1){
			auto w=std::polar(1.,pi/(nrt<<1));
			auto p=rt.data();
			mul_n(p,w,nrt,p+nrt);
		}
	}
	inline void dif(std::complex<double> *a,int n){
		if (n<=1) return;
		for (int m=n>>1;m>1;m>>=1){
			for (int j=0,k=0;j<n;j+=m+m,k++){
				m256d t=make2(rt[k]);
				for (int i=j;i<j+m;i+=2){
					m256d x=load(a+i),y=mul2cp(load(a+m+i),t);
					store(a+i,x+y),store(a+m+i,x-y);
				}
			}
		}
		for (int j=0,k=0;j<n;j+=2,k++){
			auto x=a[j],y=a[j+1]*rt[k];
			a[j]=x+y,a[j+1]=x-y;
		}
	}
	inline void dit_without_normalize(std::complex<double> *a,int n){
		if (n<=1) return;
		for (int j=0,k=0;j<n;j+=2,k++){
			auto x=a[j],y=a[j+1];
			a[j]=x+y,a[j+1]=(x-y)*rt[k];
		}
		for (int m=2;m<n;m<<=1){
			for (int j=0,k=0;j<n;j+=m+m,k++){
				m256d t=make2(rt[k]);
				for (int i=j;i<j+m;i+=2){
					m256d x=load(a+i),y=load(a+i+m);
					store(a+i,x+y),store(a+i+m,mul2cp(x-y,t));
				}
			}
		}
		std::reverse(a+1,a+n);
	}
	inline void conv(std::complex<double> *a,std::complex<double> *b,int n){
		init(n),dif(a,n),dif(b,n);
		const double r=1./n,qr=0.25*r;
		a[0]={a[0].real()*b[0].real()+a[0].imag()*b[0].imag(),a[0].real()*b[0].imag()+a[0].imag()*b[0].real()};
		a[0]*=r,a[1]*=b[1]*r;
		for (int k=2,m=3;k<n;k+=k,m+=m){
			for (int i=k,j=i+k-1;i<m;i++,j--){
				auto a1=a[i]+std::conj(a[j]),a2=a[i]-std::conj(a[j]);
				auto b1=b[i]+std::conj(b[j]),b2=b[i]-std::conj(b[j]);
				auto u=a1*b1+a2*b2*((i&1)?rt[i>>1]:-rt[i>>1]),v=a1*b2+a2*b1;
				a[i]=(u+v)*qr,a[j]=std::conj(u-v)*qr;
			}
		}
		dit_without_normalize(a,n);
	}
	inline void conv_sqr(std::complex<double> *a,int n){
		init(n),dif(a,n);
		const double r=1./n,qr=0.25*r;
		a[0]={a[0].real()*a[0].real()+a[0].imag()*a[0].imag(),a[0].real()*a[0].imag()*2.};
		a[0]*=r,a[1]*=a[1]*r;
		for (int k=2,m=3;k<n;k+=k,m+=m){
			for (int i=k,j=i+k-1;i<m;i++,j--){
				auto a1=a[i]+std::conj(a[j]),a2=a[i]-std::conj(a[j]);
				auto u=a1*a1+a2*a2*((i&1)?rt[i>>1]:-rt[i>>1]),v=a1*a2*2.;
				a[i]=(u+v)*qr,a[j]=std::conj(u-v)*qr;
			}
		}
		dit_without_normalize(a,n);
	}
}

template<int _r=10,int _b=100000000,int _d=8,int _fb=10000>
class basic_BigInt{
public:
	inline static constexpr int radix=_r,base=_b,digit=_d,FFTbase=_fb;

	bool sign;
	std::vector<int> a;
	basic_BigInt(){sign=false,a.resize(1);}
	basic_BigInt(ll x){
		a.clear();
		if(x<0) sign=true,x=-x;
		else sign=false;
		do{
			a.push_back(x%base);
			x/=base;
		}while(x);
	}

	inline void fix(){
		while (not a.empty() and a.back()==0) a.pop_back();
		if (a.empty()) a.push_back(0),sign=false;
		// assert(std::find_if(a.begin(),a.end(),[](int x){return x>=base or x<0;})==a.end());
	}

	basic_BigInt(const std::string &buf){
		assert(std::count(buf.begin(),buf.end(),'-')<=1);
		sign=false;
		a.clear();
		for (int i=buf.length()-1,j=base;i>=0;i--){
			if (buf[i]=='-') sign=true;
			else if (isdigit(buf[i])){
				if (j==base) a.push_back(0),j=1;
				a.back()+=j*(buf[i]^48);
				j*=10;
			}
		}
		fix();
	}

	operator std::string() const{
		std::stringstream buf;
		if (sign) buf<<"-";
		if constexpr (radix==10){
			buf<<a.back();
			for (int i=a.size()-2;i>=0;i--) buf<<std::setw(digit)<<std::setfill('0')<<a[i];
		}else{
			auto output=[&](int x,bool f){
				std::string s;do{s.push_back(x%radix+48);x/=radix;}while(x);std::reverse(s.begin(),s.end());
				if (not f){buf<<std::setw(digit)<<std::setfill('0');}buf<<s;
			};
			output(a.back(),true);
			for (int i=a.size()-2;i>=0;i--) output(a[i],false);
		}
		return buf.str();
	}
	ull to_ull() const{
		ull ans=0;
		for (int i=a.size()-1;i>=0;i--) ans*=base,ans+=a[i];
		return ans;
	}

	friend std::istream& operator >> (std::istream &in,basic_BigInt &a){
		std::string buf;
		in>>buf;
		a=basic_BigInt(buf);
		return in;
	}
	friend std::ostream& operator << (std::ostream &out,const basic_BigInt &a){
		if (a.sign) out<<"-";
		if constexpr (radix==10){
			out<<a.a.back();
			for (int i=a.a.size()-2;i>=0;i--) out<<std::setw(digit)<<std::setfill('0')<<a.a[i];
		}else{
			auto output=[&](int x,bool f){
				std::string s;do{s.push_back(x%radix+48);x/=radix;}while(x);std::reverse(s.begin(),s.end());
				if (not f){out<<std::setw(digit)<<std::setfill('0');}out<<s;
			};
			output(a.a.back(),true);
			for (int i=a.a.size()-2;i>=0;i--) output(a.a[i],false);
		}
		return out;
	}

	inline int _ucmp(const basic_BigInt &b) const{
		if (a.size()!=b.a.size()) return a.size()<b.a.size()?-1:1;
		for (int i=a.size()-1;i>=0;i--) if (a[i]!=b.a[i]) return a[i]<b.a[i]?-1:1;
		return 0;
	}
	inline int cmp(const basic_BigInt &b) const{
		if (not sign and b.sign) return 1;
		if (sign and not b.sign) return -1;
		return _ucmp(b);
	}
	inline bool operator == (const basic_BigInt &b) const{return cmp(b)==0;}
	inline bool operator != (const basic_BigInt &b) const{return cmp(b)!=0;}
	inline bool operator > (const basic_BigInt &b) const{return cmp(b)==1;}
	inline bool operator < (const basic_BigInt &b) const{return cmp(b)==-1;}
	inline bool operator >= (const basic_BigInt &b) const{return cmp(b)>=0;}
	inline bool operator <= (const basic_BigInt &b) const{return cmp(b)<=0;}

	basic_BigInt operator - () const{basic_BigInt ans(*this);ans.sign^=1;return ans;}
private:
	basic_BigInt& _uadd(const basic_BigInt &b){
		a.resize(std::max(a.size(),b.a.size())+1);
		int t=0;
		for (ui i=0;i<a.size()-1;i++){
			if ((a[i]+=(i>=b.a.size()?0:b.a[i])+t)>=base) a[i]-=base,t=1;
			else t=0;
		}
		a.back()=t;
		fix();
		return *this;
	}
	basic_BigInt& _usub(const basic_BigInt &b){ // *this>=b
		int t=0;
		for (ui i=0;i<a.size();i++){
			if ((a[i]-=(i>=b.a.size()?0:b.a[i])+t)<0) a[i]+=base,t=1;
			else t=0;
		}
		assert(t==0);
		fix();
		return *this;
	}
public:
	basic_BigInt& operator += (const basic_BigInt &b){
		if (sign==b.sign) return _uadd(b);
		if (_ucmp(b)>=0) return _usub(b);
		return *this=basic_BigInt(b)._usub(*this);
	}
	basic_BigInt& operator -= (const basic_BigInt &b){
		if (sign!=b.sign) return _uadd(b);
		if (_ucmp(b)>=0) return _usub(b);
		*this=basic_BigInt(b)._usub(*this);
		this->sign^=1;
		return *this;
	}
	basic_BigInt& operator *= (int k){
		if (k==0){
			a.resize(1);
			sign=false,a[0]=0;
			return *this;
		}
		if (k==1) return *this;
		if (k==-1){
			sign^=1;
			return *this;
		}
		ll tmp=std::abs(k),t=0;
		assert(tmp<base);
		a.resize(a.size()+1);
		for (ui i=0;i<a.size()-1;i++){
			t+=a[i]*tmp;
			a[i]=t%base;
			t/=base;
		}
		a.back()=t;
		sign^=(k<0);
		fix();
		return *this;
	}
	basic_BigInt& operator *= (const basic_BigInt &b){
		if (a.size()<=16 or b.a.size()<=16){
			std::vector<int> tmp(a.size()+b.a.size());
			for (ui i=0;i<a.size();i++){
				for (ui j=0;j<b.a.size();j++){
					ll u=ll(a[i])*b.a[j]+tmp[i+j];
					tmp[i+j]=u%base;
					tmp[i+j+1]+=u/base;
				}
			}
			a.swap(tmp);
			sign^=b.sign;
			fix();
			return *this;
		}else{
			int len=1<<(std::__lg(a.size()+b.a.size()-1)+1);
			std::vector<std::complex<double>> f(len),g(len);
			for (ui i=0;i<a.size();i++) f[i]={double(a[i]%FFTbase),double(a[i]/FFTbase)};
			for (ui i=0;i<b.a.size();i++) g[i]={double(b.a[i]%FFTbase),double(b.a[i]/FFTbase)};
			FFT::conv(f.data(),g.data(),len);
			ll tmp=0;
			a.resize(a.size()+b.a.size());
			for (int i=0;i<int(a.size());i++){
				tmp+=(ll(f[i].real()+0.5)+ll(f[i].imag()+0.5)*FFTbase);
				a[i]=tmp%base;
				tmp/=base;
			}
			sign^=b.sign;
			fix();
			return *this;
		}
	}
	basic_BigInt sqr() const{
		if (a.size()<=16){
			basic_BigInt ans;
			ans.a.resize(a.size()<<1);
			for (ui i=0;i<a.size();i++){
				for (ui j=0;j<a.size();j++){
					ll u=ll(a[i])*a[j]+ans.a[i+j];
					ans.a[i+j]=u%base;
					ans.a[i+j+1]+=u/base;
				}
			}
			ans.sign=false;
			ans.fix();
			return ans;
		}else{
			int len=1<<(std::__lg((a.size()<<1)-1)+1);
			std::vector<std::complex<double>> f(len);
			for (ui i=0;i<a.size();i++) f[i]={double(a[i]%FFTbase),double(a[i]/FFTbase)};
			FFT::conv_sqr(f.data(),len);
			ll tmp=0;
			basic_BigInt ans;
			ans.a.resize(a.size()<<1);
			for (int i=0;i<int(ans.a.size());i++){
				tmp+=(ll(f[i].real()+0.5)+ll(f[i].imag()+0.5)*FFTbase);
				ans.a[i]=tmp%base;
				tmp/=base;
			}
			ans.sign=false;
			ans.fix();
			return ans;
		}
	}
	basic_BigInt& operator /= (int k){
		assert(k!=0);
		if (k==1) return *this;
		if (k==-1){
			sign^=1;
			return *this;
		}
		const int tmp=std::abs(k);
		ll t=0;
		for (int i=a.size()-1;i>=0;i--){
			t*=base,t+=a[i];
			a[i]=t/tmp;
			t%=tmp;
		}
		sign^=(k<0);
		fix();
		return *this;
	}
	basic_BigInt div2() const{
		basic_BigInt ans;
		ans.a.resize(a.size());
		ll t=0;
		for (int i=a.size()-1;i>=0;i--){
			t*=base,t+=a[i];
			ans.a[i]=t>>1;
			t&=1;
		}
		ans.sign=sign;
		ans.fix();
		return ans;
	}
private:
	basic_BigInt _div_force(basic_BigInt x) const{
		assert(not sign and not x.sign);
		basic_BigInt y=1,ans;
		std::vector<basic_BigInt> s1,s2;
		for (;x<=*this;x+=x,y+=y) s1.push_back(x),s2.push_back(y);
		x=*this;
		for (;not s1.empty();s1.pop_back(),s2.pop_back()){
			if (s1.back()<=x) x-=s1.back(),ans+=s2.back();
		}
		return ans;
	}
public:
	basic_BigInt& shift(const int n){
		if (n<0){
			if ((-n)>=int(a.size())) a.resize(1),a[0]=0;
			else a.erase(a.begin(),a.begin()+(-n));
		}else if (n>0){
			a.insert(a.begin(),n,0);fix();
		}
		return *this;
	}
	void add1(){
		if (sign and a.size()==1 and a[0]==1){sign=false;a[0]=0;return;}
		if (sign){sub1();return;}
		a.resize(a.size()+1);
		for (int i=0;;i++){
			if (a[i]!=base-1){a[i]++;break;}
			a[i]=0;
		}
		fix();
	}
	void sub1(){
		if (a.size()==1 and a[0]==0){sign=true;a[0]=1;return;}
		if (sign){add1();return;}
		for (int i=0;;i++){
			if (a[i]==0) a[i]=base-1;
			else{a[i]--;break;}
		}
		fix();
	}
	basic_BigInt operator ~ () const{
		assert(not sign);
		if (a.size()<=32){
			basic_BigInt b;
			b.a.resize(a.size()<<1|1);
			b.a.back()=1;
			return b._div_force(*this);
		}

		basic_BigInt b,ans;
		int m=a.size(),k=(m+5)>>1;
		b.a.resize(k);
		for (int i=k-1,j=a.size()-1;i>=0;i--,j--) b.a[i]=a[j];

		b=~b;
		ans=*this*b.sqr();
		ans.shift(-(k+k));
		b+=b;
		b.shift(m-k);
		ans=b-ans;
		ans.sub1();
		b.a.clear(),b.a.resize(m<<1|1),b.a.back()=1;
		if (b-*this*ans>=*this) ans.add1(); // 1>=a*(ans+1)
		return ans;
	}
	basic_BigInt& operator /= (const basic_BigInt &b){
		assert(b.a.size()>0 and (b.a.size()>1 or b.a[0]!=0));
		basic_BigInt x=*this,y=b;
		sign=false,y.sign=false;
		if (*this<y){
			a.resize(1);
			sign=false,a[0]=0;
			return *this;
		}
		int n=a.size(),m=b.a.size();
		if (n>(m<<1)){
			shift(n-(m+m));
			y.shift(n-(m+m));
			m=n-m;
			n=m+m;
		}
		*this*=~y;
		shift(-(m+m));
		if (x-*this*b>=b) add1(); // a>b*(x+1)
		sign=x.sign^b.sign;
		fix();
		return *this;
	}
	basic_BigInt& operator %= (const basic_BigInt &b){
		assert(b.a.size()>0 and (b.a.size()>1 or b.a[0]!=0));
		return *this-=(*this/b)*b;
	}
	basic_BigInt operator + (const basic_BigInt &b) const{basic_BigInt ans(*this);return ans+=b;}
	basic_BigInt operator - (const basic_BigInt &b) const{basic_BigInt ans(*this);return ans-=b;}
	basic_BigInt operator * (int k) const{basic_BigInt ans(*this);return ans*=k;}
	basic_BigInt operator * (const basic_BigInt &b) const{basic_BigInt ans(*this);return ans*=b;}
	basic_BigInt operator / (int k) const{basic_BigInt ans(*this);return ans/=k;}
	basic_BigInt operator / (const basic_BigInt &b) const{basic_BigInt ans(*this);return ans/=b;}
	basic_BigInt operator % (const basic_BigInt &b) const{basic_BigInt ans(*this);return ans%=b;}
	basic_BigInt& operator ++ (){add1();return *this;}
	basic_BigInt operator ++ (int){basic_BigInt t(*this);add1();return t;}
	basic_BigInt& operator -- (){sub1();return *this;}
	basic_BigInt operator -- (int){basic_BigInt t(*this);sub1();return t;}

	friend basic_BigInt abs(basic_BigInt a){a.sign=false;return a;}

	friend std::pair<basic_BigInt,basic_BigInt> divmod(const basic_BigInt &a,const basic_BigInt &b){
		std::pair<basic_BigInt,basic_BigInt> ans;
		ans.first=a/b,ans.second=a-ans.first*b;
		return ans;
	}

	friend basic_BigInt qpow(basic_BigInt a,int k){
		if (k==0) return basic_BigInt(1);
		if (k==1) return a;
		basic_BigInt res(1);
		for (;k;k>>=1,a=a.sqr()) if(k&1) res*=a;
		return res;
	}

	friend basic_BigInt sqrt_force(const basic_BigInt &a){
		basic_BigInt x0=base;
		x0.shift((a.a.size()+2)>>1);
		basic_BigInt x=(x0+a/x0).div2();
		while (x<x0){
			std::swap(x,x0);
			x=(x0+a/x0).div2();
		}
		return x0;
	}
	friend basic_BigInt rsqrt(const basic_BigInt &a){
		if (a.a.size()==0) return basic_BigInt();
		if (a.a.size()==1) return basic_BigInt(ll(ll(base)*base/sqrtl(a.a[0])));
		if (a.a.size()==2) return basic_BigInt(ll(ll(base)*base/sqrtl(ll(a.a[1])*base+a.a[0])));
		int u=a.a.size();
		u+=(u&1);
		int v=((u+2)>>2)<<1;
		if (a.a.size()<=5){
			basic_BigInt t=1,x=a;
			t.shift(u+u),x.shift(u+u);
			return t/=sqrt_force(x);
		}
		basic_BigInt b,ans;
		b.a.resize(v+a.a.size()-u);
		for (int i=b.a.size()-1,j=a.a.size()-1;i>=0;i--,j--) b.a[i]=a.a[j];
		b=rsqrt(b);
		b.shift((u-v)>>1);
		ans=(b*3).div2();
		b*=(b.sqr()*a).div2();
		b.shift(-(u<<1));
		ans-=b;
		ans.sub1();
		return ans;
	}
	friend basic_BigInt sqrt(const basic_BigInt &a){
		if (a<=0) return basic_BigInt();
		int n=a.a.size();
		n+=(n&1);
		basic_BigInt ans=rsqrt(a)*a;
		ans.shift(-n);
		int d=1;
		for (;;d+=d){
			if ((ans+d).sqr()>a) break;
			ans+=d;
		}
		for (;d>0;d>>=1){
			if ((ans+d).sqr()>a) continue;
			ans+=d;
		}
		return ans;
	}

	friend basic_BigInt root_binary(const basic_BigInt &a,int k){
		if (k==1) return a;
		basic_BigInt l=basic_BigInt(0),r=basic_BigInt(1);
		while (qpow(r,k)<=a) l=r,r*=2;
		while (l<r){
			basic_BigInt mid=(l+r).div2();
			int t=qpow(mid,k).cmp(a);
			if (t==0) return mid;
			if (t==-1) l=++mid;
			else r=mid;
		}
		return --l;
	}
	friend basic_BigInt rroot(const basic_BigInt &a,int k){
		int u=(a.a.size()-1)/k+1;
		if (u<=2){
			basic_BigInt ans=root_binary(a,k);
			ans.shift(u*k);
			return ans/=a;
		}
		int v=(u>>1)+1;
		basic_BigInt b,ans;
		b.a.resize(a.a.size()-k*(u-v));
		for (int i=b.a.size()-1,j=a.a.size()-1;i>=0;i--,j--) b.a[i]=a.a[j];
		b=rroot(b,k);
		b.sub1();
		ans=b;
		b.shift(-(int(a.a.size())-1));
		ans-=b*2;
		ans.shift(u-v);
		return (ans*(k+1)-(qpow(a*ans,k-1)*ans.sqr()).shift(-u*k*k))/k;
	}
	friend basic_BigInt root(const basic_BigInt &a,int k){
		if (k==1) return a;
		if (a==0 or a==1) return a;
		bool flag=false;
		if (a<0){
			if (k&1) flag=true;
			else return 0;
		}
		int n=(a.a.size()-1)/k+1;
		basic_BigInt ans=(a*rroot(a,k)).shift(-n*k);
		ans.sub1();
		int d=1;
		for (;;d+=d){
			if (qpow(ans+d,k)>a) break;
			ans+=d;
		}
		for (;d>0;d>>=1){
			if (qpow(ans+d,k)>a) continue;
			ans+=d;
		}
		if (flag) ans.sign^=1;
		return ans;
	}
};

typedef basic_BigInt<> BigInt;

BigInt gcd(BigInt a,BigInt b){
	a.sign=b.sign=false;
	if (a.a.size()<=1 and b.a.size()<=1) return std::gcd(a.a[0],b.a[0]);
	if (a<b) std::swap(a,b);
	if (b==0) return a;
	int t=0;
	for (;(a.a[0]&1)==0 and (b.a[0]&1)==0;t++) a=a.div2(),b=b.div2();
	while (b>0){
		if ((a.a[0]&1)==0) a=a.div2();
		else if ((b.a[0]&1)==0) b=b.div2();
		else a-=b;
		if (a<b) std::swap(a,b);
	}
	while (t--) a*=2;
	return a;
}
BigInt lcm(const BigInt &a,const BigInt &b){return abs(a/gcd(a,b)*b);}

BigInt pow10(int k){
	BigInt ans=1;
	ans.shift(k/BigInt::digit);
	int t=1;
	for (int i=0;i<k%BigInt::digit;i++) t*=10;
	if (t>1) ans*=t;
	return ans;
}

typedef basic_BigInt<2,67108864,26,8192> BigBin;

std::pair<BigBin,BigBin> _to_bin(const BigInt &a,int l=0,int r=-1){
	if (r==-1) r=a.a.size()-1;
	if (l==r) return {BigBin(a.a[l]),BigBin(BigInt::base)};
	int mid=(l+r)>>1;
	auto [lo,pl]=_to_bin(a,l,mid);
	auto [hi,pr]=_to_bin(a,mid+1,r);
	return {lo+=(hi*=pl),pr*=pl};
}
std::pair<BigInt,BigInt> _to_dec(const BigBin &a,int l=0,int r=-1){
	if (r==-1) r=a.a.size()-1;
	if (l==r) return {BigInt(a.a[l]),BigInt(BigBin::base)};
	int mid=(l+r)>>1;
	auto [lo,pl]=_to_dec(a,l,mid);
	auto [hi,pr]=_to_dec(a,mid+1,r);
	return {lo+=(hi*=pl),pr*=pl};
}

template <int a, int b, int c, int d>
auto& basic_BigInt<a, b, c, d>::operator &= (basic_BigInt<a, b, c, d> &a,const basic_BigInt<a, b, c, d> &b){
	BigBin x=_to_bin(a).first,y=_to_bin(b).first;
	if (x.a.size()<y.a.size()) x.a.resize(y.a.size());
	for (int i=0;i<int(y.a.size());i++) x.a[i]&=y.a[i];
	return a=_to_dec(x).first;
}
basic_BigInt& basic_BigInt::operator |= (basic_BigInt &a,const basic_BigInt &b){
	BigBin x=_to_bin(a).first,y=_to_bin(b).first;
	if (x.a.size()<y.a.size()) x.a.resize(y.a.size());
	for (int i=0;i<int(y.a.size());i++) x.a[i]|=y.a[i];
	return a=_to_dec(x).first;
}
basic_BigInt& basic_BigInt::operator ^= (basic_BigInt &a,const basic_BigInt &b){
	BigBin x=_to_bin(a).first,y=_to_bin(b).first;
	if (x.a.size()<y.a.size()) x.a.resize(y.a.size());
	for (int i=0;i<int(y.a.size());i++) x.a[i]^=y.a[i];
	return a=_to_dec(x).first;
}
basic_BigInt basic_BigInt::operator &= (const basic_BigInt &a,const basic_BigInt &b){basic_BigInt ans(a);ans&=b;return ans;}
basic_BigInt basic_BigInt::operator |= (const basic_BigInt &a,const basic_BigInt &b){basic_BigInt ans(a);ans|=b;return ans;}
basic_BigInt basic_BigInt::operator ^= (const basic_BigInt &a,const basic_BigInt &b){basic_BigInt ans(a);ans^=b;return ans;}

using namespace std;

int main(){
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
	BigInt a;
	cin>>a;
	cout<<_to_bin(a).first;
	return 0;
}