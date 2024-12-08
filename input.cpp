#include<bits/stdc++.h>
         #define      ll    long         long
        using      namespace     std      ;
         inline      ll   read      (     ){
   	ll      x    =0     ,y        =1            ;
        	char          c   =getchar       (      )          ;
        	while        (c<'0'||c>'9'){
		if(c=='-')y=-1;
		c=getchar();
	}
	while(c>='0'&&c<='9')x=(x<<3)+(x<<1)+c-'0',c=getchar();
	return x*y;
}
ll T,M,a,b,c,delta;
ll gcd(ll a,ll b){
	if(b==0)return a;
	return gcd(b,a%b);
}
int main(){
	T=read();M=read();
	while(T--){
		a=read();b=read();c=read();
		delta=b*b-4*a*c;
		if(delta<0){
			printf("NO\n");
		}else if(delta==0){
			ll p=-b,q=2*a;
			ll pq=gcd(abs(q),abs(p));
			p/=pq,q/=pq;
			if(q<0)q=-q,p=-p;
			if(q==1)printf("%lld\n",p);
			else printf("%lld/%lld\n",p,q);
		}else{
			ll p=-b,q=2*a;
			ll sq=(ll)sqrt(delta);
			if(sq*sq==delta){
				if(q>0)p+=sq;
				else p-=sq;
				ll pq=gcd(abs(q),abs(p));
				p/=pq,q/=pq;
				if(q<0)q=-q,p=-p;
				if(q==1)printf("%lld\n",p);
				else printf("%lld/%lld\n",p,q);
			}else{
				ll pq=gcd(abs(q),abs(p));
				p/=pq,q/=pq;
				if(q<0)q=-q,p=-p;
				if(p!=0){
					if(q==1)printf("%lld+",p);
					else printf("%lld/%lld+",p,q);
				}
				q=abs(2*a);
				p=1;
				ll t=0;
				for(int r=sq;r>=1;r--)
					if(delta%(r*r)==0){
						p*=r;
						t=delta/(r*r);
						break;
					}
				pq=gcd(abs(q),abs(p));
				p/=pq,q/=pq;
				if(q<0)q=-q,p=-p;
				if(p==q) printf("sqrt(%lld)\n",t);
				else if(q==1) printf("%lld*sqrt(%lld)\n",p,t);
				else if(p==1) printf("sqrt(%lld)/%lld\n",t,q);
				else printf("%lld*sqrt(%lld)/%lld\n",p,t,q);
			}
		}
	}
	return 0;
}
