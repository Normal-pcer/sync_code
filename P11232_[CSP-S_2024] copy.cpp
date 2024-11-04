#include <bits/stdc++.h>
#define DEBUG_MODE false
#define from(i, begin, end) for(int i=begin; i<=end; i++)
#define debug if(DEBUG_MODE)
#define log debug printf

template <const int MAX_SIZE>
struct IO {
    char buf[MAX_SIZE], *p1, *p2;

    IO(): buf(), p1(), p2() {}

    using string = std::string;

    inline char gc() {
#if true
        if (p2==p1)  p2 = (p1=buf) + fread(buf, 1, MAX_SIZE, stdin);
        return (p2==p1? ' ': *p1++);
#else
        return getchar();
#endif
    }
    bool isd(char c) { return '0'<=c and c<='9'; }
    bool blank(char c) { return c==' ' or c=='\t' or c=='\n' or c=='\r'; }

    template <class T>
    void read(T&& x) {
        double tmp = 1; bool sign = false; x = 0; char ch = gc();
        for (; not isd(ch); ch=gc())  if (ch=='-') sign=true;
        for (; isd(ch); ch=gc())  x=(x*10)+(ch^48);
        if (ch=='.')  for (ch=gc(); isd(ch); ch=gc())  tmp*=.1f, x+=tmp*(ch^48);
        if (sign)  x=-x;
    }
    void read(char& c) { for(c=gc(); blank(c); c=gc()); }
    void read(char* s) {
        char ch = gc();
        for (; blank(ch); ch=gc());
        for (; not blank(ch); ch=gc()) *s++=ch;
        *s=0;
    }
    void read(string& s) {
        string().swap(s); char ch = gc();
        for (; blank(ch); ch=gc());
        for (; not blank(ch); ch=gc()) s.push_back(ch);
    }
    template <class T, class ...Ts>
    void read(T x, Ts ...args) { read(x); read(args...); }
    template <class T>
    IO& operator>> (T& x) { read(x); return *this; }
};

IO<1048576> io;

int T;

namespace Solution {

    template <const int MAX_SIZE>
    struct SegTree {
        struct Node {
            int l, r, val;
        } tr[MAX_SIZE<<2];
        #define ls (p<<1)
        #define rs (p<<1|1)
        
        void pushup(int p) {
            tr[p].val = tr[ls].val + tr[rs].val;
        }

        void build(int p, int l, int r) {
            tr[p].l = l, tr[p].r = r;
            if (l==r) {
                tr[p].val = 0;
                return;
            }
            int mid = (l+r)>>1;
            build(ls, l, mid);
            build(rs, mid+1, r);
            pushup(p);
        }

        void add(int p, int pos, int val) {
            if (tr[p].l == pos and tr[p].r == pos) {
                tr[p].val += val;
                return;
            }
            if (tr[ls].r >= pos)  add(ls, pos, val);
            if (tr[rs].l <= pos)  add(rs, pos, val);
            pushup(p);
        }

        int query(int p, int l, int r) {
            if (tr[p].l>=l and tr[p].r<=r) {
                return tr[p].val;
            }
            int res = 0;
            if (tr[ls].r >= l)  res += query(ls, l, r);
            if (tr[rs].l <= r)  res += query(rs, l, r);
            return res;
        }
        #undef ls
        #undef rs
    };
    

    int N, M;  // Amount of cars and observers 
    const int _N = 1e5+5, _L = 1e6+5;
    int L, V;
    SegTree<_L> detected;  // How many observers are there in this segment

    // calculate the first position x that v_x > V.
    // ensure that a>0
    int firstPosIllegal(int x0, int v0, int a) {
        return (V*V - v0*v0)/(2*a) + 1 + x0;
    }

    // calculate the last position x that v_x > V.
    // ensure that a<0 and a_abs = abs(a)
    int lastPosIllegal(int x0, int v0, int a_abs) {
        int m = v0*v0 - V*V, n = 2 * a_abs;
        int div = m/n, mod = m%n;
        if (mod==0)  return std::max(x0-1, div+x0-1);
        else  return std::max(x0-1, div+x0);
    }

    struct seg {
        int first, second;
        bool detected = false;
    };

    // Calculate the segment of position that the car is running illegally. 
    // !IMPORTANT! it will returns a pair of {-1, -1} when it is legal competely.
    // This function is testified as right with sample data 1.
    seg calcIllegalSeg(int x0, int v0, int a) {
        if (a <= 0 and v0 <= V)  return {-1, -1};
        if (a >= 0 and v0 > V)  return {x0, L};

        if (a > 0) {
            return {firstPosIllegal(x0, v0, a), L};
        }
        if (a < 0) {
            return {x0, lastPosIllegal(x0, v0, -a)};
        }
        assert(0);  // Unreachable
        return {-1, -1};
    }

    

    std::vector<seg> carSegs;
    std::vector<int> observers;

    // Find the last element x that x<=val
    // returns end if not found
    template <class T, class U>
    auto customize_bound(T begin, T end, U val) {
        // while (end-begin>1) {
        //     auto mid = begin + (end-begin)/2;
        //     if (*mid >= val) {
        //         end = mid;
        //     } else {
        //         begin = mid+1;
        //     }
        // }
        auto tmp = std::upper_bound(begin, end, val);
        if (tmp==end or *tmp > val)  return tmp-1;
        return tmp;
    }

    void init() {  // Don't forget to clear all arrays used!!!!
        carSegs.clear();
        observers.clear();

        io >> N >> M >> L >> V;
        // all position appeared in this code means that 'distance from outset', which is able to be 0.
        detected.build(1, 0, L+5); 

        carSegs.reserve(N);
        from(i, 1, N) {
            int d, v, a;
            io >> d >> v >> a;
            carSegs.push_back( calcIllegalSeg(d, v, a) );
        }
        observers.resize(M);
        from(i, 0, M-1)  io >> observers[i];
    }


    void solve() {
        init();

        // With the method
        // Sort segments with right point decrease
        std::sort(carSegs.begin(), carSegs.end(), [&](auto a, auto b) { return a.second>b.second; } );

        // Check all segment weather it is detected
        // Add all observers to the tree
        for (auto obs: observers) {
            detected.add(1, obs, 1);
        }
        int cnt_car = 0;
        // For each segment, check it
        for (auto &seg: carSegs) {
            if (seg.first == -1) {
                seg.detected = false; continue;  // Not required to detect
            }
            if (detected.query(1, seg.first, seg.second) != 0) {
                // debug printf("!!!!!%d %d\n", seg.first, seg.second);
                seg.detected = true;
                cnt_car++;
            }
        }
        printf("%d ", cnt_car);

        // Clear the segment tree
        detected.build(1, 0, L+5);
        
        int cnt = 0;
        // For each segment
        for (auto &seg: carSegs) {
            int l=seg.first, r=seg.second;
            // Not detected initially - pass!
            if (not seg.detected) { continue; }
            // Or it's already detected now?
            if (detected.query(1, l, r) != 0) { continue; }
            // Find the most-right observer
            std::vector<int>::iterator selectedPtr = customize_bound(observers.begin(), observers.end(), r);
            assert(selectedPtr != observers.end());
            // Select it and add to total count
            cnt++;
            // Record the observer to the segment tree
            detected.add(1, *selectedPtr, 1);
        }
        printf("%d\n", M-cnt);
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("detect.in", "r", stdin);
    freopen("detect.out", "w", stdout);
#endif
    
    io >> T;

    while (T-->0)
        Solution::solve();
    return 0;
}