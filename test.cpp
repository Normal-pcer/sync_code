/**
 * 
 */

#include "lib"
#include "libs/all.hpp"
using namespace lib;



namespace Solution {

    int N; const int _N = 5e5+5;

    template <const int MAX_SZ>
    struct SegTree {
        #define ls (p<<1)
        #define rs (p<<1|1)
        struct Node {
            int l, r;
            int val;
            int tag;
        } t[MAX_SZ<<2];

        void push_up(int p) {
            t[p].val = t[ls].val + t[rs].val;
        }

        void push_down(int p) {
            if (t[p].tag) {
                t[ls].val += (t[ls].r-t[ls].l+1) * t[p].tag;
                t[rs].val += (t[rs].r-t[rs].l+1) * t[p].tag;
                t[ls].tag += t[p].tag;
                t[rs].tag += t[p].tag;
                t[p].tag=0;
            }
        }

        void build(int p, int l, int r) {
            t[p].l = l, t[p].r = r;
            if (l==r) {
                t[p].val = a[l];
                return;
            }
            int mid = (l+r)>>1;
            build(ls, l, mid);
            build(rs, mid+1, r);
            push_up();
        }

        int add_query(int p, int l, int r, int val) {
            if (tr[p].l>=l and tr[p].r<=r) {
                tr[p].val += val;
                return tr[p].val;
            }
            int res = 0;
            if (t[ls].r>=l)  res += add_query(ls, l, r, val);
            if (t[rs].l<=r)  res += add_query(rs, l, r, val);
            return res;
        }
        #undef ls
        #undef rs
    };
    
    void init() {

    }

    void solve() {
        init();

    }
}


int main() {
    initDebug;
    Solution::solve();
    return 0;
}