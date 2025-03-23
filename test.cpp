#include <bits/stdc++.h>

using i32 = std::int32_t; using i64 = std::int64_t;

class SegTree {
    struct Node {
        i64 sum = 0, add_tag = 0;

        auto add(i64 x, i32 node_left, i32 node_right) -> void {
            sum += (node_right - node_left + 1) * x;
            add_tag += x;
        }
    };
    i32 root_left, root_right;
    std::vector<Node> tree;

    auto static constexpr lson(i32 p) -> i32 { return p << 1; }
    auto static constexpr rson(i32 p) -> i32 { return p << 1 | 1; }
    auto pushUp(i32 p) -> void {
        tree[p].sum = tree[lson(p)].sum + tree[rson(p)].sum;
    }
    auto pushDown(i32 p, i32 node_left, i32 node_right) -> void {
        if (tree[p].add_tag != 0) {
            auto node_mid = std::midpoint(node_left, node_right);
            tree[lson(p)].add(tree[p].add_tag, node_left, node_mid);
            tree[rson(p)].add(tree[p].add_tag, node_mid + 1, node_right);
            tree[p].add_tag = 0;
        }
    }
    auto build(i32 p, i32 node_left, i32 node_right, std::vector<i64> const &init) -> void {
        if (node_left == node_right) {
            tree[p].sum = init[node_left];
            return;
        }
        auto node_mid = std::midpoint(node_left, node_right);
        build(lson(p), node_left, node_mid, init);
        build(rson(p), node_mid + 1, node_right, init);
        pushUp(p);
    }
    auto sumBetween(i32 left, i32 right, i32 p, i32 node_left, i32 node_right) -> i64 {
        if (node_left >= left and node_right <= right) {
            return tree[p].sum;
        }
        auto node_mid = std::midpoint(node_left, node_right);
        i64 res = 0;
        pushDown(p, node_left, node_right);
        if (left <= node_mid)  res += sumBetween(left, right, lson(p), node_left, node_mid);
        if (right > node_mid)  res += sumBetween(left, right, rson(p), node_mid + 1, node_right);
        return res;
    }
    auto addBetween(i32 left, i32 right, i64 val, i32 p, i32 node_left, i32 node_right) -> void {
        if (node_left >= left and node_right <= right) {
            tree[p].add(val, node_left, node_right);
            return;
        }
        auto node_mid = std::midpoint(node_left, node_right);
        pushDown(p, node_left, node_right);
        if (left <= node_mid)  addBetween(left, right, val, lson(p), node_left, node_mid);
        if (right > node_mid)  addBetween(left, right, val, rson(p), node_mid + 1, node_right);
        pushUp(p);
    }
public:
    SegTree(i32 N, std::vector<i64> const &init): root_left(1), root_right(N), tree(N << 2) {
        build(1, 1, N, init);
    }
    auto sumBetween(i32 left, i32 right) -> i64 {
        return sumBetween(left, right, 1, root_left, root_right);
    }
    auto addBetween(i32 left, i32 right, i64 val) -> void {
        addBetween(left, right, val, 1, root_left, root_right);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr), std::cout.tie(nullptr);

    i32 N, M;  std::cin >> N >> M;
    std::vector<i64> init(N + 1);
    for (auto &x: init | std::views::drop(1))  std::cin >> x;

    SegTree sgt{N, init};
    for (i32 q = 0; q < M; q++) {
        i32 op, l, r;  std::cin >> op >> l >> r;
        if (op == 1) {  // add
            i64 val;  std::cin >> val;
            sgt.addBetween(l, r, val);
        } else {
            auto ans = sgt.sumBetween(l, r);
            std::cout << ans << '\n';
        }
    }
    return 0;
}
