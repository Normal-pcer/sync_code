#include<bits/stdc++.h>
using namespace std;
int n,q,a[100001];
struct d{
	int w,b,lw,lb,rw,rb,mw,mb;
	d(int w=0,int b=0,int lw=0,int lb=0,int rw=0,int rb=0,int mw=0,int mb=0):
    w(w),b(b),lw(lw),lb(lb),rw(rw),rb(rb),mw(mw),mb(mb){}
};
inline d hb(d i,d j){
	return d(
	i.w+j.w, i.b+j.b,
	(i.b?i.lw:i.w+j.lw), (i.w?i.lb:i.b+j.lb),
	(j.b?j.rw:j.w+i.rw), (j.w?j.rb:j.b+i.rb),
	max(max(i.mw,j.mw),i.rw+j.lw),
	max(max(i.mb,j.mb),i.rb+j.lb));
}
d dat[262144]; int len[262144],tg1[262144],tg2[262144];
inline void P(int i,int typ){
	d&t=dat[i];
	if(typ==0) tg2[i]= 0, tg1[i]=0, t=d(0,len[i],0,len[i],0,len[i],0,len[i]);
	if(typ==1) tg2[i]= 0, tg1[i]=1, t=d(len[i],0,len[i],0,len[i],0,len[i],0);
	if(typ==2) tg2[i]^=1, swap(t.w,t.b), swap(t.lw,t.lb), swap(t.rw,t.rb), swap(t.mw,t.mb);
}
inline void pd(int i){
	if(~tg1[i]) P(i<<1,tg1[i]), P(i<<1|1,tg1[i]);
	if(tg2[i]) P(i<<1,2), P(i<<1|1,2);
	tg1[i]=-1, tg2[i]=0;
}
void build(int i,int l,int r){
	len[i]=r-l+1; tg1[i]=-1;
	if(l==r) {int t=a[l]; dat[i]=d(t,t^1,t,t^1,t,t^1,t,t^1); return;}
	build(i<<1,l,l+r>>1);
	build(i<<1|1,(l+r>>1)+1,r);
	dat[i]=hb(dat[i<<1],dat[i<<1|1]);
}
void Mdf(int i,int l,int r,int a,int b,int t){
	if(b<l||r<a) return; if(a<=l&&r<=b) {P(i,t); return;}
	pd(i); Mdf(i<<1,l,l+r>>1,a,b,t), Mdf(i<<1|1,(l+r>>1)+1,r,a,b,t);
	dat[i]=hb(dat[i<<1],dat[i<<1|1]);
}
d Qur(int i,int l,int r,int a,int b){
	if(b<l||r<a) return d(); if(a<=l&&r<=b) return dat[i];
	pd(i); return hb(Qur(i<<1,l,l+r>>1,a,b),Qur(i<<1|1,(l+r>>1)+1,r,a,b));
}
int main(){
	scanf("%d%d",&n,&q);
	for(int i=1;i<=n;++i) scanf("%d",a+i);
	build(1,1,n);
	for(int i=1;i<=q;++i){
		int opt,l,r;
		scanf("%d%d%d",&opt,&l,&r); ++l, ++r;
		if(opt<3) Mdf(1,1,n,l,r,opt);
		else {d t=Qur(1,1,n,l,r); printf("%d\n",opt==3?t.w:t.mw);}
	}
	return 0;
}
