#pragma GCC target("avx2")
#include<bits/stdc++.h>
#include<immintrin.h>
using namespace std;
using ui=unsigned;
using ll=long long;
using ull=unsigned long long;
using m256=__m256i;

mt19937 rnd(random_device{}());
constexpr bool POLY_RANGE_CHECK=false;

template<class T>
inline void zero_n(T *a,const ui n){memset(a,0,sizeof(T)*n);}
template<class T>
inline void copy_zero_n(const T *a,const ui n,T *b,const ui m){
	copy_n(a,n,b);
	zero_n(b+n,m-n);
}
template<class T>
inline void renew(T *&a,ui rsiz,ui nsiz,align_val_t align){
	T *na=new(align) T[nsiz];
	copy_zero_n(a,min(rsiz,nsiz),na,nsiz);
	::operator delete[](a,align);
	a=na;
}
inline m256& cast(ui *p){return *(m256*)p;}
inline const m256& cast(const ui *p){return *(m256*)p;}
inline m256 make_4(const ull x){return _mm256_set_epi64x(x,x,x,x);}
inline m256 make_8(const ui x){return _mm256_set_epi32(x,x,x,x,x,x,x,x);}
inline m256 make_8(const ui x0,const ui x1,const ui x2,const ui x3,const ui x4,const ui x5,const ui x6,const ui x7){return _mm256_set_epi32(x7,x6,x5,x4,x3,x2,x1,x0);}
constexpr inline ui getlim(const ui x){return x<3?x:2<<__lg(x-1);}
constexpr inline ui Zinv(const ui x){
	ui y=x*(2-x*x);
	y*=2-x*y;
	y*=2-x*y;
	return y*(2-x*y);
}
constexpr inline ui qpow_mod(ui a,ui b,ui mod){
	ull res=1;
	for (;b;b>>=1,a=a*a%mod) if (b&1) res=res*a%mod;
	return res;
} 
inline constexpr int primitive_root(int m){
	if (m==2) return 1;
	else if (m==167772161) return 3;
	else if (m==469762049) return 3;
	else if (m==754974721) return 11;
	else if (m==998244353) return 3;
	else if (m==1004535809) return 3;
	int divs[32]={2},cnt=1;
	int x=(m-1)>>1;
	while ((x&1)==0) x>>=1;
	for (ll i=3;i*i<=x;i+=2){
		if (x%i==0){
			divs[cnt++]=i;
			do{
				x/=i;
			}while (x%i==0);
		}
	}
	if (x>1) divs[cnt++]=x;
	for (int g=2;;g++){
		bool flag=true;
		for (int i=0;i<cnt;i++){
			if (qpow_mod(g,(m-1)/divs[i],m)==1){
				flag=false;
				break;
			}
		}
		if (flag) return g;
	}
}

template<ui mod>
class Poly{
	inline static constexpr ui PP=mod<<1,PPP=mod+PP,R=-Zinv(mod),RR=-ull(mod)%mod;
public:
	static constexpr inline ui reduce(const ull x){return (x+ui(x)*R*ull(mod))>>32;}
	static constexpr inline ui fix(const ui x){return x<mod?x:x-mod;}
	static constexpr inline ui mul(const ui x,const ui y){return reduce(ull(x)*y);}
	static constexpr inline ui in(const ui x){return mul(x,RR);}
	static constexpr inline ui out(const ui x){return fix(reduce(x));}
	static constexpr inline ui add(ui x,const ui y){return x+=y,x<PP?x:x-PP;}
	static constexpr inline ui sub(ui x,const ui y){return x-=y,x<PP?x:x+PP;}
	static constexpr inline ui neg(const ui x){return PP-x;}
	static constexpr inline ui div2(const ui x){return (x&1?x+mod:x)>>1;}
	static constexpr inline ui negdiv2(const ui x){return (x&1?PPP-x:PP-x)>>1;}
	inline static constexpr ui one=in(1),two=in(2),neg1=in(mod-1);
	static constexpr inline ui qpow(ui a,ui b,ui res=one){
		for (;b;b>>=1,a=mul(a,a)) if (b&1) res=mul(res,a);
		return res;
	}
	inline static constexpr ui g=in(primitive_root(mod)),inv2=qpow(two,mod-2);
private:
	inline static ui _Cipolla_w;
	struct _Cipolla_Complex{
		ui x,y;
		inline _Cipolla_Complex operator * (const _Cipolla_Complex &b) const{
			return {add(mul(x,b.x),mul(y,mul(b.y,_Cipolla_w))),add(mul(x,b.y),mul(y,b.x))};
		}
	};
	static inline ui qpowc(_Cipolla_Complex a,ui b){
		_Cipolla_Complex res={one,0};
		for (;b;b>>=1,a=a*a) if (b&1) res=res*a;
		return res.x;
	}
public:
	static inline ui cipolla(ui n){
		if (fix(n)==0) return 0;
		if (fix(n)==1) return one;
		if (qpow(n,(mod-1)>>1)==neg1) return -1u;
		ui a;
		while (true){
			a=in(rnd()%mod);
			_Cipolla_w=sub(mul(a,a),n);
			if (qpow(_Cipolla_w,(mod-1)>>1)==neg1) break;
		}
		_Cipolla_Complex x={a,one};
		ll res=out(qpowc(x,(mod+1)>>1));
		return in(min(res,mod-res));
	}
	inline static const ui img=cipolla(in(mod-1)),iimg=add(img,img),negi=neg(img),invi=negi,inv2i=qpow(add(img,img),mod-2);
	static inline bool checkImaginary(){
		if (img==-1u) cerr<<"Warning: Imaginary units do not exist in the modulo "<<mod<<" sense.\nDo not use sin,cos and tan.\n";
		return img!=-1u;
	}
	inline static const bool useImaginary=checkImaginary();
private:
	inline static const m256 zero8=make_8(0),one8=make_8(1),MSK_LO=make_4(~0u),P4=make_4(mod),P8=make_8(mod),PP8=make_8(PP),Q8=make_8(mod-1),R4=make_4(R),RR8=make_8(RR),PP_PPP=make_8(PP,PPP,0,0,0,0,0,0);
	inline static const m256 dif_4_idx=make_8(4,0,5,1,6,2,7,3),dif_2_idx=make_8(4,0,6,2,5,1,7,3),dit_4_idx=make_8(0,2,4,6,1,3,5,7),dit_2_idx=make_8(0,4,2,6,1,5,3,7);

	static inline m256 reduce_unshift(const m256 &x){return _mm256_add_epi64(x,_mm256_mul_epu32(_mm256_mul_epu32(x,R4),P4));}
	static inline m256 reduce_shift(const m256 &x){return _mm256_srli_epi64(reduce_unshift(x),32);}
	static inline m256 reduce(const m256 &x){return _mm256_blend_epi32(reduce_shift(_mm256_and_si256(x,MSK_LO)),reduce_unshift(_mm256_srli_epi64(x,32)),170);}
	static inline m256 fix(const m256 &x){return _mm256_min_epu32(x,_mm256_sub_epi32(x,P8));}
	static inline m256 mul(const m256 &x,const m256 &y){return _mm256_blend_epi32(reduce_shift(_mm256_mul_epu32(x,y)),reduce_unshift(_mm256_mul_epu32(_mm256_srli_epi64(x,32),_mm256_srli_epi64(y,32))),170);}
	static inline m256 mulx(const m256 &x,const m256 &y){return _mm256_blend_epi32(reduce_shift(_mm256_mul_epu32(x,y)),reduce_unshift(_mm256_mul_epu32(_mm256_srli_epi64(x,32),y)),170);}
	static inline m256 in(const m256 &x){return mulx(x,RR8);}
	static inline m256 out(m256 x){return fix(reduce(x));}
	static inline m256 add(m256 x,const m256 &y){return x=_mm256_add_epi32(x,y),_mm256_min_epu32(x,_mm256_sub_epi32(x,PP8));}
	static inline m256 sub(m256 x,const m256 &y){return x=_mm256_sub_epi32(x,y),_mm256_min_epu32(x,_mm256_add_epi32(x,PP8));}
	static inline m256 neg(const m256 &x){return _mm256_sub_epi32(PP8,x);}
	static inline m256 negdiv2(const m256 &x){return _mm256_srli_epi32(_mm256_sub_epi32(_mm256_permutevar8x32_epi32(PP_PPP,_mm256_and_si256(x,one8)),x),1);}

	#define make_op1_n(op)\
	static inline void op##_n(ui *a,const ui n){ui i=0;for (;i+7<n;i+=8) cast(a+i)=op(cast(a+i));for (;i!=n;i++) a[i]=op(a[i]);}\
	static inline void op##_n(const ui *a,const ui n,ui *c){ui i=0;for (;i+7<n;i+=8) cast(c+i)=op(cast(a+i));for (;i!=n;i++) c[i]=op(a[i]);}
	#define make_op2_n(op)\
	static inline void op##_n(ui *a,const ui *b,const ui n){ui i=0;for (;i+7<n;i+=8) cast(a+i)=op(cast(a+i),cast(b+i));for (;i!=n;i++) a[i]=op(a[i],b[i]);}\
	static inline void op##_n(const ui *a,const ui *b,const ui n,ui *c){ui i=0;for (;i+7<n;i+=8) cast(c+i)=op(cast(a+i),cast(b+i));for (;i!=n;i++) c[i]=op(a[i],b[i]);}
	#define make_opx_n(op)\
	static inline void op##_n(ui *a,const ui b,const ui n){m256 x=make_8(b);ui i=0;for (;i+7<n;i+=8) cast(a+i)=op##x(cast(a+i),x);for (;i!=n;i++) a[i]=op(a[i],b);}\
	static inline void op##_n(const ui *a,const ui b,const ui n,ui *c){m256 x=make_8(b);ui i=0;for (;i+7<n;i+=8) cast(c+i)=op##x(cast(a+i),x);for (;i!=n;i++) c[i]=op(a[i],b);}
	make_op1_n(in) make_op1_n(out) make_op1_n(fix) make_op1_n(neg) make_op1_n(negdiv2)
	make_op2_n(add) make_op2_n(sub) make_op2_n(mul)
	make_opx_n(mul)

	inline static ui nrt,*rt,*irt;
	inline static m256 *rt4,*irt4,*rt2,*irt2,*rt1,*irt1;
	static inline void init_root(const ui n){
		if (nrt>=(n>>1)) [[likely]] return;
		renew(rt,nrt,n>>1,align_val_t{32});
		renew(irt,nrt,n>>1,align_val_t{32});
		renew(rt4,nrt>>3,n>>3,align_val_t{256});
		renew(irt4,nrt>>3,n>>3,align_val_t{256});
		renew(rt2,nrt>>3,n>>3,align_val_t{256});
		renew(irt2,nrt>>3,n>>3,align_val_t{256});
		renew(rt1,nrt>>3,n>>3,align_val_t{256});
		renew(irt1,nrt>>3,n>>3,align_val_t{256});
		if (nrt==0) [[unlikely]] nrt=1,*rt=*irt=one;
		ui m=nrt>>3;
		for (;nrt!=(n>>1);nrt<<=1){
			ui w=qpow(g,mod/(nrt<<2));
			mul_n(rt,w,nrt,rt+nrt);
			mul_n(irt,qpow(w,mod-2),nrt,irt+nrt);
			fix_n(irt+nrt,nrt);
		}
		for (;m!=(n>>3);m++){
			rt4[m]=make_4(rt[m]);
			irt4[m]=make_4(irt[m]);
			rt2[m]=make_8(rt[m<<1],0,rt[m<<1],0,rt[m<<1|1],0,rt[m<<1|1],0);
			irt2[m]=make_8(irt[m<<1],0,irt[m<<1],0,irt[m<<1|1],0,irt[m<<1|1],0);
			rt1[m]=make_8(rt[m<<2],0,rt[m<<2|1],0,rt[m<<2|2],0,rt[m<<2|3],0);
			irt1[m]=make_8(irt[m<<2],0,irt[m<<2|1],0,irt[m<<2|2],0,irt[m<<2|3],0);
		}
	}

	inline static ui nnum,*seq_num,*seq_inv,*seq_fac,*seq_ifac;
	static inline void init_seq(ui n){
		if (nnum>=n) [[likely]] return;
		n=max(n,2u);
		renew(seq_num,nnum,n,align_val_t{32});
		renew(seq_inv,nnum,n,align_val_t{32});
		renew(seq_fac,nnum,n,align_val_t{32});
		renew(seq_ifac,nnum,n,align_val_t{32});
		if (nnum==0) [[unlikely]] nnum=2,seq_num[1]=seq_inv[1]=*seq_fac=seq_fac[1]=*seq_ifac=seq_ifac[1]=one;
		for (;nnum!=n;nnum++){
			seq_num[nnum]=in(nnum);
			seq_inv[nnum]=mul(in(mod-mod/nnum),seq_inv[mod%nnum]);
			seq_fac[nnum]=mul(seq_fac[nnum-1],seq_num[nnum]);
			seq_ifac[nnum]=mul(seq_ifac[nnum-1],seq_inv[nnum]);
		}
	}

	static inline void dif_421(m256 &x,const ui k){
		x=_mm256_permutevar8x32_epi32(x,dif_4_idx);
		m256 y=reduce_unshift(_mm256_mul_epu32(x,rt4[k]));
		x=_mm256_permutevar8x32_epi32(add(_mm256_blend_epi32(_mm256_srli_epi64(x,32),x,170),_mm256_blend_epi32(_mm256_srli_epi64(y,32),neg(y),170)),dif_2_idx);
		y=reduce_unshift(_mm256_mul_epu32(x,rt2[k]));
		x=_mm256_shuffle_epi32(add(_mm256_blend_epi32(_mm256_srli_epi64(x,32),x,170),_mm256_blend_epi32(_mm256_srli_epi64(y,32),neg(y),170)),114);
		y=reduce_unshift(_mm256_mul_epu32(x,rt1[k]));
		x=add(_mm256_blend_epi32(_mm256_srli_epi64(x,32),x,170),_mm256_blend_epi32(_mm256_srli_epi64(y,32),neg(y),170));
	}
	static inline void dif(ui *a,const ui n){
		if (n==1) return;
		init_root(n);
		if (n==2){
			ui x=*a,y=a[1];
			*a=add(x,y);
			a[1]=sub(x,y);
			return;
		}
		if (n==4){
			ui x=*a,y=a[1],u=a[2],v=a[3];
			*a=add(x,u),a[1]=add(y,v),a[2]=sub(x,u),a[3]=sub(y,v);
			x=*a,y=a[1],u=a[2],v=mul(a[3],rt[1]);
			*a=add(x,y),a[1]=sub(x,y),a[2]=add(u,v),a[3]=sub(u,v);
			return;
		}
		for (ui i=n>>1;i!=4;i>>=1){
			for (ui p=0,q=i;p!=i;p+=8,q+=8){
				m256 x=cast(a+p),y=cast(a+q);
				cast(a+p)=add(x,y),cast(a+q)=sub(x,y);
			}
			for (ui j=i<<1,k=1;j!=n;j+=i<<1,k++){
				for (ui p=j,q=j+i;p!=j+i;p+=8,q+=8){
					m256 x=cast(a+p),y=mulx(cast(a+q),rt4[k]);
					cast(a+p)=add(x,y),cast(a+q)=sub(x,y);
				}
			}
		}
		for (ui j=0,k=0;j!=n;j+=8,k++) dif_421(cast(a+j),k);
	}
	static inline void dit_124(m256 &x,const ui k){
		m256 y=_mm256_srli_epi64(x,32);
		x=_mm256_shuffle_epi32(_mm256_blend_epi32(add(x,y),reduce_unshift(_mm256_mul_epu32(_mm256_add_epi32(x,_mm256_sub_epi32(PP8,y)),irt1[k])),170),216);
		y=_mm256_srli_epi64(x,32);
		x=_mm256_permutevar8x32_epi32(_mm256_blend_epi32(add(x,y),reduce_unshift(_mm256_mul_epu32(_mm256_add_epi32(x,_mm256_sub_epi32(PP8,y)),irt2[k])),170),dit_2_idx);
		y=_mm256_srli_epi64(x,32);
		x=_mm256_permutevar8x32_epi32(_mm256_blend_epi32(add(x,y),reduce_unshift(_mm256_mul_epu32(_mm256_add_epi32(x,_mm256_sub_epi32(PP8,y)),irt4[k])),170),dit_4_idx);
	}
	static inline void dit(ui *a,const ui n){
		if (n==1) return;
		init_root(n);
		if (n==2){
			ui x=*a,y=a[1];
			*a=div2(add(x,y));
			a[1]=div2(sub(x,y));
			return;
		}
		if (n==4){
			ui x=*a,y=a[1],u=a[2],v=a[3];
			*a=add(x,y),a[1]=sub(x,y),a[2]=add(u,v),a[3]=mul(u+PP-v,irt[1]);
			x=*a,y=a[1],u=a[2],v=a[3];
			*a=div2(div2(add(x,u))),a[1]=div2(div2(add(y,v))),a[2]=div2(div2(sub(x,u))),a[3]=div2(div2(sub(y,v)));
			return;
		}
		for (ui j=0,k=0;j!=n;j+=8,k++) dit_124(cast(a+j),k);
		for (ui i=8;i!=n;i<<=1){
			for (ui p=0,q=i;p!=i;p+=8,q+=8){
				m256 x=cast(a+p),y=cast(a+q);
				cast(a+p)=add(x,y),cast(a+q)=sub(x,y);
			}
			for (ui j=i<<1,k=1;j!=n;j+=i<<1,k++){
				for (ui p=j,q=j+i;p!=j+i;p+=8,q+=8){
					m256 x=cast(a+p),y=cast(a+q);
					cast(a+p)=add(x,y),cast(a+q)=mulx(_mm256_add_epi32(x,_mm256_sub_epi32(PP8,y)),irt4[k]);
				}
			}
		}
		mul_n(a,in(mod-(mod-1)/n),n);
	}

	static inline void _inv_impl(const ui *a,const ui n,ui *b){
		const ui m=getlim(n);
		alignas(32) ui c[m],d[m];
		*b=qpow(*a,mod-2);
		for (ui i=1,j=2,k;i!=n;i=k,j<<=1){
			k=j;
			if (j==m) k=n;
			copy_zero_n(a,k,c,j);
			dif(c,j);
			copy_zero_n(b,i,d,j);
			dif(d,j);
			mul_n(c,d,j);
			dit(c,j);
			zero_n(c,i);
			dif(c,j);
			mul_n(c,d,j);
			dit(c,j);
			neg_n(c+i,k-i,b+i);
		}
	}

	static inline void _diff_impl(const ui *a,const ui n,ui *b){
		init_seq(n);
		ui i=1;
		if (n>7) [[likely]]{
			for (;i!=8;i++) b[i-1]=mul(a[i],seq_num[i]);
			for (;i+7<n;i+=8) _mm256_storeu_si256((__m256i_u*)(b+i-1),mul(cast(a+i),cast(seq_num+i)));
		}
		for (;i!=n;i++) b[i-1]=mul(a[i],seq_num[i]);
	}
	static inline void _integ_impl(const ui *a,const ui n,ui *b){
		init_seq(n);
		ui i=n;
		if (n>7) [[likely]]{
			while (i&7) i--,b[i]=mul(a[i-1],seq_inv[i]);
			while (i!=8) i-=8,cast(b+i)=mul(_mm256_loadu_si256((const __m256i_u*)(a+i-1)),cast(seq_inv+i));
		}
		while (--i) b[i]=mul(a[i-1],seq_inv[i]);
		*b=0;
	}

	static inline void _log_impl(const ui *a,const ui n,ui *b){
		const ui m=getlim(n+n-1);
		alignas(32) ui c[m],d[m];
		_diff_impl(a,n,c);
		zero_n(c+n-1,m-n+1);
		dif(c,m);
		_inv_impl(a,n,d);
		zero_n(d+n,m-n);
		dif(d,m);
		mul_n(c,d,m);
		dit(c,m);
		_integ_impl(c,n,b);
	}

	static inline void inplace_integ(ui *a,const ui l,const ui r){
		init_seq(r);
		if (r-l>7) [[likely]]{
			for (ui i=r;i!=l;) i-=8,cast(a+i)=mul(_mm256_loadu_si256((const __m256i_u*)(a+i-1)),cast(seq_inv+i));
		}else{
			for (ui i=r;i!=l;) i--,a[i]=mul(a[i-1],seq_inv[i]);
		}
	}

	static inline void _exp_impl(const ui *a,const ui n,ui *b){
		*b=one;
		if (n==1) return;
		const ui m=getlim(n);
		alignas(32) ui c[m],d[m],e[m],f[m];
		*c=*e=e[1]=one;
		for (ui i=1,j=2,k;i!=n;i=k,j<<=1){
			k=j;
			if (j==m) k=n;
			_diff_impl(a,i,d);
			d[i-1]=0;
			dif(d,i);
			mul_n(d,e,i);
			dit(d,i);
			d[i-1]=neg(d[i-1]);
			_diff_impl(b,i,d+i);
			d[i+i-1]=0;
			sub_n(d+i,d,i);
			zero_n(d,i-1);
			dif(d,j);
			copy_zero_n(c,i,f,j);
			dif(f,j);
			mul_n(d,f,j);
			dit(d,j);
			inplace_integ(d,i,j);
			zero_n(d,i);
			sub_n(d+i,a+i,k-i);
			dif(d,j);
			mul_n(d,e,j);
			dit(d,j);
			neg_n(d+i,k-i,b+i);
			if (j==m) break;
			copy_zero_n(b,k,e,j<<1);
			dif(e,j<<1);
			mul_n(e,f,j,d);
			dit(d,j);
			zero_n(d,i);
			dif(d,j);
			mul_n(d,f,j);
			dit(d,j);
			neg_n(d+i,k-i,c+i);
		}
	}

	static inline void _sqrtn0_impl(const ui *a,const ui n,ui *b,ui *c,const ui &w){
		const ui m=getlim(n);
		alignas(32) ui d[m],e[m],f[m];
		*b=*f=w,*c=qpow(w,mod-2);
		for (ui i=1,j=2,k;;i=k,j<<=1){
			k=j;
			if (j==m) k=n;
			mul_n(f,f,i);
			dit(f,i);
			sub_n(f,a,i,f+i);
			zero_n(f,i);
			sub_n(f+i,a+i,k-i);
			zero_n(f+k,j-k);
			dif(f,j);
			copy_zero_n(c,i,e,j);
			dif(e,j);
			mul_n(f,e,j);
			dit(f,j);
			negdiv2_n(f+i,k-i,b+i);
			if (j==m) break;
			copy_n(b,j,d);
			dif(d,j);
			copy_n(d,j,f);
			mul_n(d,e,j);
			dit(d,j);
			zero_n(d,i);
			dif(d,j);
			mul_n(d,e,j);
			dit(d,j);
			neg_n(d+i,i,c+i);
		}
	}
	static inline bool _sqrt_impl(const ui *a,const ui n,ui *b){
		ui i=0;
		while (i!=n and a[i]==0) i++;
		if (i==n){
			zero_n(b,n);
			return true;
		}
		if (i&1) return false;
		ui w=cipolla(a[i]);
		if (w==-1u) return false;
		if (i){
			const ui m=n-(i>>1);
			alignas(32) ui c[m],d[m],e[m];
			copy_zero_n(a+i,n-i,c,m);
			_sqrtn0_impl(c,m,d,e,w);
			zero_n(b,i>>1);
			copy_n(d,m,b+(i>>1));
		}else{
			alignas(32) ui c[n];
			_sqrtn0_impl(a,n,b,c,w);
		}
		return true;
	}

	static inline void _pow1_impl(const ui *a,const ui &n,ui *b,const ui &k){
		if (k==0){
			*b=one;
			zero_n(b+1,n-1);
		}else if (k==1){
			copy_n(a,n,b);
		}else{
			alignas(32) ui c[n];
			_log_impl(a,n,c);
			mul_n(c,in(k),n);
			_exp_impl(c,n,b);
		}
	}
	static inline void _pown0_impl(const ui *a,const ui n,ui *b,const ui k_mod_p,const ui k_mod_phi_p){
		if (fix(*a)==one) _pow1_impl(a,n,b,k_mod_p);
		else{
			alignas(32) ui c[n];
			mul_n(a,qpow(*a,mod-2),n,c);
			_pow1_impl(c,n,b,k_mod_p);
			mul_n(b,qpow(*a,k_mod_phi_p),n);
		}
	}
	static inline void _pow_impl(const ui *a,const ui n,ui *b,const ui k_mod_p,const ui k_mod_phi_p,const ui k_small){
		if (k_small==0){
			*b=one;
			zero_n(b+1,n-1);
			return;
		}
		ui i=0;
		while (i!=n and a[i]==0) i++;
		if (ull(i)*k_small>=n) zero_n(b,n);
		else if (i){
			const ui m=n-i*k_small;
			alignas(32) ui c[m],d[m];
			copy_n(a+i,m,c);
			_pown0_impl(c,m,d,k_mod_p,k_mod_phi_p);
			zero_n(b,i*k_small);
			copy_n(d,m,b+i*k_small);
		}else{
			_pown0_impl(a,n,b,k_mod_p,k_mod_phi_p);
		}
	}
	static inline void _pow_impl(const ui *a,const ui n,ui *b,const ui k){
		return _pow_impl(a,n,b,k%mod,k%(mod-1),min(n,k));
	}

	static inline void _comp0_impl(const ui *a,ui *b,const ui m,const ui n,const ui w) {
		if (n==1){
			alignas(32) ui c[m<<1];
			init_seq(m<<1);
			for (ui v=seq_ifac[m-1],i=0;i!=m+1;i++,v=mul(v,w)) b[m-i]=mul(v,mul(seq_fac[m-1+i],seq_ifac[i]));
			zero_n(b+m+1,m-1);
			dif(b,m<<1);
			copy_zero_n(a,m,c,m<<1);
			dif(c,m<<1);
			mul_n(c,b,m<<1);
			dit(c,m<<1);
			copy_n(c+m,m,b);
			return;
		}
		const ui o=m*n;
		alignas(32) ui c[o<<2],d[o<<2],e[o<<1];
		for (ui i=0;i!=m;i++) copy_zero_n(b+i*n,n,c+(i<<1)*n,n<<1);
		c[o<<1]=one,zero_n(c+(o<<1)+1,(o<<1)-1),dif(c,o<<2);
		for (ui i=0;i!=(o<<1);i++) d[i]=mul(c[i<<1],c[i<<1|1]);
		dit(d,o<<1),*d=sub(*d,one);
		for (ui i=1;i!=(m<<1);i++) copy_n(d+i*n,n>>1,d+i*(n>>1));
		_comp0_impl(a,d,m<<1,n>>1,w);
		for (ui i=0;i!=(m<<1);i++) copy_zero_n(d+i*(n>>1),n>>1,e+i*n,n);
		dif(e,o<<1);
		for (ui i=0;i!=(o<<1);i++){
			b[i<<1]=mul(c[i<<1|1],e[i]);
			b[i<<1|1]=mul(c[i<<1],e[i]);
		}
		dit(b,o<<2);
		for (ui i=0;i!=m;i++) copy_n(b+(((i+m)<<1)*n),n,b+i*n);
	}
	static inline void _comp_impl(const ui *a,const ui n,const ui *b,const ui m,ui *c) {
		const ui o=getlim(n);
		alignas(32) ui d[o<<2];
		neg_n(b,m,d);
		zero_n(d+m,o-m);
		_comp0_impl(a,d,1,o,*b);
		copy_n(d,n,c);
	}

	static inline void _icomp_impl(const ui *a,const ui n,ui *b){
		const ui m=getlim(n<<1);
		alignas(32) ui c[m],d[m],e[m<<1],f[m<<1];
		ui w=qpow(a[1],mod-2),v=neg1,j=1,k;
		for (ui i=1;i!=n;i++){
			v=mul(v,w);
			c[i]=mul(a[i],v);
		}
		*c=0;
		zero_n(c+n,m-n);
		*d=one;
		zero_n(d+1,m-1);
		for (ui i=n,o;i!=1;i=k,j<<=1){
			k=(i+1)>>1;
			o=getlim((i<<1)*j);
			for (ui p=0;p!=j;p++) copy_zero_n(c+i*p,i,e+(i<<1)*p,i<<1);
			for (ui p=0;p!=j;p++) copy_zero_n(d+i*p,i,f+(i<<1)*p,i<<1);
			e[(i<<1)*j]=one;
			zero_n(e+(i<<1)*j+1,(o<<1)-(i<<1)*j-1);
			dif(e,o<<1);
			zero_n(f+(i<<1)*j,(o<<1)-(i<<1)*j);
			dif(f,o<<1);
			for (ui p=0;p!=o;p++) c[p]=mul(e[p<<1],e[p<<1|1]);
			if (i&1){
				for (ui p=0;p!=o;p++) d[p]=div2(add(mul(e[p<<1|1],f[p<<1]),mul(e[p<<1],f[p<<1|1])));
			}else{
				for (ui p=0;p!=o;p++) d[p]=div2(sub(mul(e[p<<1|1],f[p<<1]),mul(e[p<<1],f[p<<1|1])));
				init_root(o);
				mul_n(d,irt,o);
			}
			if (k!=1){
				dit(c,o);
				if ((i<<1)*j==o) *c=sub(*c,one);
				memmove(c+k,c+i,k<<2);
				for (ui p=2;p!=(j<<1);p++) copy_n(c+i*p,k,c+k*p);
			}
			dit(d,o);
			memmove(d+k,d+i,k<<2);
			for (ui p=2;p!=(j<<1);p++) copy_n(d+i*p,k,d+k*p);
		}
		reverse_copy(d+j*k-n,d+j*k,c);
		init_seq(n);
		mul_n(c,seq_inv,n);
		mul_n(c,seq_num[n-1],n);
		reverse(c,c+n);
		_pow_impl(c,n-1,d,out(PP-seq_inv[n-1]));
		mul_n(d,w,n-1);
		*b=0;
		copy_n(d,n-1,b+1);
	}
public:
	ui lim,n,*a;
	inline ui& operator [](const ui x){
		if constexpr (POLY_RANGE_CHECK) if (x>=n) throw out_of_range("qwq");
		return a[x];
	}
	inline const ui& operator [](const ui x) const {
		if constexpr (POLY_RANGE_CHECK) if (x>=n) throw out_of_range("qwq");
		return a[x];
	}
	inline void resize(const ui x){
        ui tmp=lim;
		lim=getlim(x);
		if (tmp!=lim) renew(a,min(x,tmp),lim,align_val_t{32});
		else zero_n(a+n,lim-n);
		n=x;
	}
	Poly(){
		lim=n=0;
		a=nullptr;
	}
	Poly(const ui x){
		n=x;
		lim=getlim(n);
		a=new(align_val_t{32}) ui[lim];
		zero_n(a,lim);
	}
	Poly(const initializer_list<ll>& il){
		n=il.size();
		lim=getlim(n);
		a=new(align_val_t{32}) ui[lim];
		zero_n(a,lim);
		int i=0;
		for (auto it=il.begin();it!=il.end();it++,i++) a[i]=*it%mod;
	}
	Poly(const Poly &b){
		lim=b.lim,n=b.n;
		a=new(align_val_t{32}) ui[lim];
		copy_n(b.a,lim,a);
	}
	Poly(const Poly &b,const ui x){
		n=x;
		lim=getlim(n);
		a=new(align_val_t{32}) ui[lim];
		copy_n(b.a,min(n,b.n),a);
	}
	~Poly(){
		::operator delete[] (a,align_val_t{32});
	}
	inline Poly operator = (const Poly &b){
		lim=b.lim,n=b.n;
		::operator delete[] (a,align_val_t{32});
		a=new(align_val_t{32}) ui[lim];
		copy_n(b.a,lim,a);
		return *this;
	}
	inline void swap(Poly &b){
		std::swap(lim,b.lim);
		std::swap(n,b.n);
		std::swap(a,b.a);
	}
	inline Poly operator += (const ui x){
		*a=add(*a,x);
		return *this;
	}
	inline Poly operator += (const Poly &b){
		resize(max(n,b.n));
		add_n(a,b.a,n);
		return *this;
	}
	inline Poly operator - () const{
		Poly res(*this);
		neg_n(res.a,res.n);
		return res;
	}
	inline Poly operator -= (const ui x){
		*a=sub(*a,x);
		return *this;
	}
	inline Poly operator -= (const Poly &b){
		resize(max(n,b.n));
		sub_n(a,b.a,n);
		return *this;
	}
	inline Poly operator *= (const ui x){
		mul_n(a,x,n);
		return *this;
	}
	inline Poly operator *= (Poly b){
		const ui resl=n+b.n-1,l=getlim(resl);
		resize(l);
		b.resize(l);
		dif(a,l);
		dif(b.a,l);
		mul_n(a,b.a,l);
		dit(a,l);
		resize(resl);
		return *this;
	}
	inline Poly mulT(const Poly b) const{
		Poly f(*this),g(b);
		reverse(g.a,g.a+g.n);
		g*=f;
		for (ui i=0;i<n;i++) f[i]=g[i+b.n-1];
		return f;
	}
	inline Poly operator /= (const ui x){
		mul_n(a,qpow(x,mod-2),n);
		return *this;
	}
#define make_poly_operator(op) inline Poly operator op (const Poly &b)const{Poly res(*this);return (res op##=b);}
#define make_ui_operator(op) inline Poly operator op (const ui &x)const{Poly res(*this);return (res op##=x);}
	make_poly_operator(+)
	make_poly_operator(-)
	make_poly_operator(*)
	make_ui_operator(+)
	make_ui_operator(-)
	make_ui_operator(*)
	make_ui_operator(/)
	inline void input(const ui n){
		resize(n);
		for (ui i=0;i<n;i++){
			ll x;
			cin>>x;
			x%=mod;
			a[i]=in(x<0?x+mod:x);
		}
	}
	friend inline ostream& operator << (ostream& stream,const Poly &f){
		for (ui i=0;i<f.n;i++) stream<<out(f.a[i])<<' ';
		return stream;
	}
	inline void print(const char sep=' '){
		for (ui i=0;i<n;i++) cout<<out(a[i])<<sep;
	}

	#define make_poly_func(f) friend inline Poly f(const Poly &a){Poly b(a.n);_##f##_impl(a.a,a.n,b.a);return b;}
	make_poly_func(inv)
	make_poly_func(diff)
	make_poly_func(integ)
	make_poly_func(log)
	make_poly_func(exp)
	make_poly_func(sqrt)

	friend inline Poly pow(const Poly &f,const ui k){
		Poly res(f.n);
		_pow_impl(f.a,f.n,res.a,k);
		return res;
	}

	friend inline Poly pow(const Poly &f,const string &str){
		const ui n=f.n;
		ull k=0,kp=0,km=0;
		for (auto c:str){
			k=(k*10+(c^48))%mod;
			kp=(kp*10+(c^48))%(mod-1);
			km=min<ull>(km*10+(c^48),n);
		}
		Poly res(n);
		_pow_impl(f.a,n,res.a,k,kp,km);
		return res;
	}

	friend pair<Poly,Poly> divmod(const Poly& f,const Poly& g){
		Poly rf(f.n),rg(g.n);
		for (ui i=0;i<f.n;i++) rf[i]=f[f.n-i-1];
		for (ui i=0;i<g.n;i++) rg[i]=g[g.n-i-1];
		rg.resize(f.n-g.n+1);
		rf=Poly(rf*inv(rg),f.n);
		Poly q(f.n-g.n+1);
		for (ui i=0;i<f.n-g.n+1;i++) q[i]=rf[f.n-g.n-i];
		return {q,Poly(f-g*q,g.n-1)};
	}

	friend Poly sin(const Poly& p){
		Poly tmp(exp(p*img));
		return (tmp-inv(tmp))*inv2i;
	}
	friend Poly cos(const Poly& p){
		Poly tmp(exp(p*img));
		return (tmp+inv(tmp))*inv2;
	}
	friend Poly tan(const Poly& p){
		return inv(exp(p*iimg)+one)*iimg-img;
	}
	friend Poly asin(const Poly& p){
		return integ(Poly(diff(p)*inv(sqrt(-p*p+one)),p.n));
	}
	friend Poly acos(const Poly &p){
		return integ(-Poly(diff(p)*inv(sqrt(-p*p+one)),p.n));
	}
	friend Poly atan(const Poly& p){
		return integ(Poly(diff(p)*inv(p*p+one),p.n));
	}

	static Poly stirling1_row(ui n){
		if (n==1) return Poly({0,one});
		else if (n&1){
			Poly f=stirling1_row(n-1);
            cout<<f<<"n&1\n";
			f.resize(n+1);
            cout<<f<<" resize\n";
			init_seq(n+1);
			for (int i=n;i>=1;i--) f[i]=add(f[i-1],mul(f[i],seq_num[n-1]));
            cout<<f<<"**\n";
			f[0]=mul(f[0],seq_num[n-1]);
            cout<<f<<"*\n";
			return f;
		}else{
			ui m=n>>1;
			init_seq(m+1);
			Poly f=stirling1_row(m),a(m+1),b(m+1);
            cout<<f<<"else\n";
			ll t=one;
			for (ui i=0;i<=m;i++){
				a[i]=mul(f[i],seq_fac[i]);
				b[i]=mul(t,seq_ifac[i]);
				t=mul(t,seq_num[m]);
			}
			reverse(a.a,a.a+m+1);
			a*=b;
			a.resize(m+1);
			for (ui i=0;i<=m;i++) b[i]=mul(seq_ifac[i],a[m-i]);
			return Poly(f*=b,n+1);
		}
	}
	friend Poly stirling1_col(ui n,ui k){
		Poly f(n+1);
		if (k>n) return f;
		init_seq(n+1);
		f[0]=f[1]=one;
		f=pow(log(f),k)*seq_ifac[k];
		for (ui i=0;i<=n;i++){
			f[i]=mul(f[i],seq_fac[i]);
			if ((i-k)&1) f[i]=neg(f[i]);
		}
		return f;
	}
	friend Poly stirling2_row(ui n){
		Poly f(n+1),g(n+1);
		init_seq(n+1);
		for (ui i=0;i<=n;i++){
			f[i]=qpow(seq_num[i],n,seq_ifac[i]);
			if (i&1) g[i]=neg(seq_ifac[i]);
			else g[i]=seq_ifac[i];
		}
		return Poly(f*=g,n+1);
	}
	friend Poly stirling2_col(ui n,ui k){
		Poly f(n+1);
		init_seq(n+1);
		for (ui i=1;i<=n;i++) f[i]=seq_ifac[i];
		f=pow(f,k);
		for (ui i=0;i<f.n;i++) f[i]=mul(f[i],mul(seq_fac[i],seq_ifac[k]));
		return f;
	}
private:
	inline static vector<Poly> _eval_tr;
	static inline constexpr int _get_tr_id(int l,int r){
		return (l+r)|(l!=r);
	}
	static void _eval_build(ui l,ui r,const Poly& a){
		ui i=_get_tr_id(l,r);
		if (l==r){
			_eval_tr[i]=Poly({one,neg(a[l])});
			return;
		}
		ui mid=(l+r)>>1;
		_eval_build(l,mid,a);
		_eval_build(mid+1,r,a);
		_eval_tr[i]=_eval_tr[_get_tr_id(l,mid)]*_eval_tr[_get_tr_id(mid+1,r)];
	}
	static void _eval(ui l,ui r,const Poly& f,Poly& res){
		if (l==r){
			res[l]=f[0];
			return;
		}
		ui mid=(l+r)>>1;
		_eval(l,mid,Poly(f.mulT(_eval_tr[_get_tr_id(mid+1,r)]),mid-l+1),res);
		_eval(mid+1,r,Poly(f.mulT(_eval_tr[_get_tr_id(l,mid)]),r-mid),res);
	}
public:
	friend Poly eval(const Poly& f,const Poly& a){
		ui n=max(a.n,f.n);
		_eval_tr.resize(n+n+1);
		_eval_build(0,n-1,a);
		Poly res(n);
		_eval(0,n-1,f.mulT(inv(_eval_tr[_get_tr_id(0,n-1)])),res);
		res.resize(a.n);
		return res;
	}
private:
	inline static vector<Poly> _inter_tr;
	static void _inter_build(ui l,ui r,const Poly& a){
		ui i=_get_tr_id(l,r);
		if (l==r){
			_inter_tr[i]=Poly({neg(a[l]),one});
			return;
		}
		ui mid=(l+r)>>1;
		_inter_build(l,mid,a);
		_inter_build(mid+1,r,a);
		_inter_tr[i]=_inter_tr[_get_tr_id(l,mid)]*_inter_tr[_get_tr_id(mid+1,r)];
	}
	Poly _solve(ui l,ui r,const Poly& a){
		if (l==r) return Poly({a[l]});
		ui mid=(l+r)>>1;
		return _solve(l,mid,a)*_inter_tr[_get_tr_id(mid+1,r)]+_solve(mid+1,r,a)*_inter_tr[_get_tr_id(l,mid)];
	}
public:
	friend Poly interpolate(const Poly& x,const Poly& y){
		const ui n=x.n;
		_inter_tr.resize(n+n+1);
		_inter_build(0,n-1,x);
		_inter_tr[_get_tr_id(0,n-1)]=diff(_inter_tr[_get_tr_id(0,n-1)]);
		Poly t=eval(_inter_tr[_get_tr_id(0,n-1)],x);
		for (ui i=0;i<n;i++) t[i]=qpow(t[i],mod-2,y[i]);
		return _solve(0,n-1,t);
	}
	friend ui BostanMori(Poly f,Poly g,ll n){
		while (n){
			Poly t=g;
			for (ui i=1;i<t.n;i+=2) t[i]=neg(t[i]);
			f*=t;
			g*=t;
			ui i=(n&1);
			for (;i<f.n;i+=2) f[i>>1]=f[i];
			f.resize(i>>1);
			for (i=0;i<g.n;i+=2) g[i>>1]=g[i];
			g.resize(i>>1);
			n>>=1;
		}
		if (f.n==0) return 0;
		else return qpow(g[0],mod-2,f[0]);
	}
	friend ui linearRecursion(const Poly& f,const Poly& a,ll k){
		const ui n=f.n;
		Poly p(n),q(n+1);
		*q.a=one;
		for (ui i=0;i<n;i++) p.a[i]=a.a[i],q.a[i+1]=neg(f.a[i]);
		p*=q;
		p.resize(n);
		return BostanMori(p,q,k);
	}
	friend Poly BerlekampMassey(const Poly& a){
		Poly res,lst;
		ui k=-1u,d=0;
		for (ui i=0;i<a.n;i++){
			ui t=0;
			for (ui j=0;j<res.n;j++) t=add(t,mul(a.a[i-j-1],res.a[j]));
			if (a.a[i]==t) continue;
			if (k==-1u){
				k=i;
				d=sub(a.a[i],t);
				res=Poly(i+1);
				continue;
			}
			Poly u(res);
			ui val=qpow(d,mod-2,sub(a.a[i],t));
			if (res.n<lst.n+i-k) res.resize(lst.n+i-k);
			res.a[i-k-1]=add(res.a[i-k-1],val);
			for (ui j=0;j<lst.n;j++) res.a[i-k+j]=sub(res.a[i-k+j],mul(val,lst.a[j]));
			if (u.n+k<lst.n+i){
				lst.swap(u);
				k=i;
				d=sub(a.a[i],t);
			}
		}
		return res;
	}

	friend inline Poly composition(const Poly &f,const Poly &g){
		Poly res(f.n);
		_comp_impl(f.a,f.n,g.a,g.n,res.a);
		return res;
	}
	inline Poly operator ()(const Poly &g) const{
		return composition(*this,g);
	}
	friend inline Poly invComposition(const Poly &f){
		Poly res(f.n);
		_icomp_impl(f.a,f.n,res.a);
		return res;
	}
	friend Poly pointValueTranslation(const Poly& f,const ui m){
		const ui n=f.n-1;
		init_seq(n<<1|1);
		vector<ui> mfac(n+n+2),mifac(n+n+2),minv(n+n+2);
		mfac[0]=minv[0]=mifac[0]=one;
		for (ui i=1;i<=(n<<1|1);i++) mfac[i]=mul(mfac[i-1],in((m-n+i-1+mod)%mod));
		mifac[n<<1|1]=qpow(mfac[n<<1|1],mod-2);
		for (ui i=(n<<1|1);i>0;i--){
			mifac[i-1]=mul(mifac[i],in((m-n+i-1+mod)%mod));
			minv[i]=mul(mifac[i],mfac[i-1]);
		}
		Poly a(n+1),b(n<<1|1);
		for (ui i=0;i<=n;i++){
			a.a[i]=mul(mul(f[i],seq_ifac[i]),seq_ifac[n-i]);
			if ((n-i)&1) a.a[i]=neg(a.a[i]);
		}
		for (ui i=0;i<=(n<<1);i++) b.a[i]=minv[i+1];
		a*=b;
		b.resize(n+1);
		for (ui i=n;i<=(n<<1);i++) b.a[i-n]=mul(mul(mfac[i+1],a.a[i]),mifac[i-n]);
		return b;
	}
};

int main() {
	// ios::sync_with_stdio(false);
	// cin.tie(0);
	// cout.tie(0);
	int n;
	cin>>n;
	cout<<Poly<167772161>::stirling1_row(n);
    // Poly<998244353> f;
    // f.input(n);

    // cout << cos(f) << endl;
	return 0;
}
