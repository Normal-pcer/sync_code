#include <bits/stdc++.h>

class SegTree {
    struct Node {
        int begin, end;
        long long sum = 0, add_tag = 0;
    };
    std::vector<Node> tr;
    std::vector<long long> &init;
    int len(int p) const { return tr[p].end - tr[p].begin; }
    int mid_(int p) const { return std::midpoint(tr[p].begin, tr[p].end); }
#define ls (mid<<1)
#define rs (mid<<1 | 1)
    void pushUp(int p) {
        auto mid = mid_(p);
        tr[p].sum = tr[ls].sum + tr[rs].sum;
    }
    void addNode(int p, int val) {
        tr[p].sum += val * len(p);
        tr[p].add_tag += val;
    }
    void pushDown(int p) {
        if (tr[p].add_tag) {
            auto mid = mid_(p);
            for (auto s: {ls, rs}) {
                addNode(s, tr[p].add_tag);
            }
            tr[p].add_tag = 0;
        }
    }
    void build(int begin, int end, int p = 1) {
        tr[p].begin = begin, tr[p].end = end;
        if (begin + 1 == end) {
            tr[p].sum = init.at(begin);
            return;
        }
        auto mid = mid_(p);
        build(begin, mid, ls), build(mid, end, rs);
        pushUp(p);
    }
public:
    SegTree(int N, std::vector<long long> &init): tr(N << 1), init(init) {
        build(0, N);
    }
    long long sum(int begin, int end, int p = 1) {
        if (tr[p].begin >= begin and tr[p].end <= end) {
            return tr[p].sum;
        }
        pushDown(p);
        auto res = 0LL;
        auto mid = mid_(p);
        if (tr[ls].end > begin)  res += sum(begin, end, ls);
        if (tr[rs].begin < end)  res += sum(begin, end, rs);
        return res;
    }
    void add(int begin, int end, long long val, int p = 1) {
        if (tr[p].begin >= begin and tr[p].end <= end) {
            addNode(p, val);
            return;
        }
        pushDown(p);
        auto mid = mid_(p);
        if (tr[ls].end > begin)  add(begin, end, val, ls);
        if (tr[rs].begin < end)  add(begin, end, val, rs);
        pushUp(p);
    }
#undef ls
#undef rs
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    int N, M;  std::cin >> N >> M;
    std::vector<long long> init(N+1);
    for (auto &i: init | std::views::drop(1))  std::cin >> i; 
    SegTree sgt(N+1, init);
    for (auto _ = 0; _ != M; _++) {
        int op, x, y;
        std::cin >> op >> x >> y;
        if (op == 1) {
            int k;  std::cin >> k;
            sgt.add(x, y+1, k);
        } else {
            auto ans = sgt.sum(x, y+1);
            std::cout << ans << '\n';
        }
    }
    return 0;
}