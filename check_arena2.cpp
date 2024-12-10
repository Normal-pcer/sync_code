#include<bits/stdc++.h>
#define debug(x) cerr<<#x<<" = "<<x<<endl;
#define db(x) debug(x)
using namespace std;
int dat[10005],n;
struct trans{
    int pre,mid,suf,sum;
    int fin(){
        return max(max(pre,suf),mid);
    }
    trans DB(){
        db(pre);db(mid);db(suf);db(sum);
        return *this;
    }
};
trans merge(trans s1,trans s2){
    trans ans;
    ans.pre=max(s1.pre,s2.pre+s1.sum);
    ans.mid=max(max(s1.mid,s2.mid),s1.suf+s2.pre);
    ans.suf=max(s1.suf+s2.sum,s2.suf);
    ans.sum=s1.sum+s2.sum;
    return ans;
}
struct node{
    int l,r;
    trans dat;
}p[40005];
void update(int rt){
    p[rt].dat=merge(p[rt*2].dat,p[rt*2+1].dat);
    return;
}
void build(int rt,int l,int r){
    p[rt].l=l;p[rt].r=r;
    if(l==r){
        p[rt].dat.pre=p[rt].dat.mid=p[rt].dat.suf=p[rt].dat.sum=dat[l];
        return;
    }
    int mid=(l+r)/2;
    build(rt*2,l,mid);
    build(rt*2+1,mid+1,r);
    update(rt);
    return;
}
trans query_sub_max(int rt,int l,int r){
    if(l>r){
        return (trans){0,0,0,0};
    }
    if(p[rt].l==l&&p[rt].r==r){
        return p[rt].dat;
    }
    int mid=p[rt*2].r;
    if(r<=mid)return query_sub_max(rt*2,l,r);
    else if(l>mid)return query_sub_max(rt*2+1,l,r);
    return merge(query_sub_max(rt*2,l,mid),query_sub_max(rt*2+1,mid+1,r));
}
int query(int l1,int r1,int l2,int r2){
    if(r1<l2){
        int tmp=query_sub_max(1,l1,r1).suf;
        tmp+=query_sub_max(1,r1+1,l2-1).sum;
        tmp+=query_sub_max(1,l2,r2).pre;
        return tmp;
    }
    int ans=query_sub_max(1,l2,r1).mid;
    if(l1<l2)ans=max(ans,query_sub_max(1,l1,l2).suf+query_sub_max(1,l2,r2).pre-dat[l2]);
    if(r2>r1)ans=max(ans,query_sub_max(1,l1,r1).suf+query_sub_max(1,r1,r2).pre-dat[r1]);
    return ans;
}
int main(){
    int t;
    scanf("%d",&t);
    while(t--){
        scanf("%d",&n);
        for(int i=1;i<=n;i++)scanf("%d",&dat[i]);
        build(1,1,n);
        int m;
        scanf("%d",&m);
        while(m--){
            int x1,x2,y1,y2;
            scanf("%d%d%d%d",&x1,&y1,&x2,&y2);
            printf("%d\n",query(x1,y1,x2,y2));
        }
    }
    return 0;
}