#include <bits/stdc++.h>
#define initDebug DEBUG_MODE=(argc-1)&&!strcmp("-d", argv[1])
#define debug if(DEBUG_MODE)
#define log(f, a...) debug printf(f, ##a);
#define upto(i,n) for(int i=1;i<=n;i++)
#define from(i,b,e) for(int i=b;i<=e;i++)
#define reverse(i,e,b) for(int i=e;i>=b;i--)
#define main() main(int argc, char const *argv[])
#define optimizeIO std::ios::sync_with_stdio(false); std::cin.tie(0); std::cout.tie(0);
#define chkMax(base,cmp) base=std::max(base,cmp)
#define chkMin(base,cmp) base=std::min(base,cmp)
#define update(base,op,modify) base=op(base,modify)
#define ensure(con,v) if(!(con))v=false;
#define optional(ptr) if(ptr)ptr
bool DEBUG_MODE=false;


struct Tree {
    int l, r, t;  // 左起、右起、总共的连续长度
    int l0, r0, t0;  // 类似地，但是最长连续0
    int lpt, rpt;  // 左侧和右侧的下标
    Tree *lc = nullptr;
    Tree *rc = nullptr;

    int mid() { return (lpt+rpt)>>1; }

    int len() { return rpt-lpt+1; }

    void calc() {  // 重新计算最长连续1的个数
        if (this->lpt == this->rpt)  return;
        this->l = this->lc->l;
        if (this->lc->l == this->lc->len())     this->l += this->rc->l;
        this->r = this->rc->r;
        if (this->rc->r == this->rc->len())     this->r += this->rc->r;
        this->t = std::max({this->lc->t, this->rc->t, this->lc->r+this->rc->l});
    }

    int get(int l, int r) {
        if (this->lpt == this->rpt)  return this->t;
        int mid = this->mid();
        if (r<=mid)  return this->lc->get(l, r);
        if (l>mid) return this->rc->get(l, r);
        if (l==1 && r==7)
            printf("x");  // For Debugger
        int tmpL = std::max(l, mid + 1 - this->lc->r);   // 左子树右起序列的最左
        int tmpR = std::min(r, mid + this->rc->l);   // 右子树左起序列的最右
        log("[%d, %d].get(%d, %d) = max(%d, %d, %d)\n", lpt, rpt, l, r, this->lc->get(l, mid), this->rc->get(mid+1, r), tmpR-tmpL+1)
        return std::max({this->lc->get(l, mid), this->rc->get(mid+1, r), tmpR-tmpL+1});
    }

    void rev(int l, int r) {
        if (l>r)  return;
        if (l==this->lpt && r==this->rpt) {
            this->rev_all();
            return;
        }
        int mid = this->mid();
        this->lc->rev(l, mid);
        this->rc->rev(mid+1, r);
    }

    void rev_all() {
        std::swap(l0, l);
        std::swap(r0, r);
        std::swap(t0, t);
    }

    Tree *getIndex(int index) {
        log("getIndex %d %d %d\n", lpt, index, rpt)
        if (lpt==index && index==rpt)   return this;
        int mid = this->mid();
        if(!(this->lc)) this->lc = new Tree({0, 0, 0, 1, 1, 1, lpt, mid});
        if(!(this->rc)) this->rc = new Tree({0, 0, 0, 1, 1, 1, mid+1, rpt});
        return (index<=mid? this->lc: this->rc)->getIndex(index);
    }

    void set(int value) {
        assert(l==r);
        l=1, r=1, t=1, l0=0, r0=0, t0=0;
        if (!value) this->rev_all();
    }
};

const int maxN = (int)5e5+5;
int N, Q;  // Q - queries

int main() {
    initDebug;
    scanf("%d%d", &N, &Q);
    Tree *root = new Tree({0, 0, 0, 1, 1, 1, 1, N});
    int temp;
    upto(i, N) {
        scanf("%1d", &temp);
        root->getIndex(i)->set(temp);
    }
    root->calc();
    int OP, L, R;
    upto(i, Q) {
        scanf("%d%d%d", &OP, &L, &R);
        if (OP==1) {
            root->rev(L, R);
            root->calc();
        } else {
            printf("%d\n", root->get(L, R));
        }
    }
    return 0;
}