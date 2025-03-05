#include <bits/stdc++.h>
using namespace std;

// 定义线段树节点结构体
struct SegmentTreeNode {
    int l, r;
    SegmentTreeNode* left;
    SegmentTreeNode* right;
    long long max_val;

    // 构造函数
    SegmentTreeNode(int l, int r) : l(l), r(r), left(nullptr), right(nullptr), max_val(0) {}
};

// 构建线段树
SegmentTreeNode* build(const vector<long long>& arr, int l, int r) {
    SegmentTreeNode* node = new SegmentTreeNode(l, r);
    if (l == r) {
        node->max_val = arr[l];
    } else {
        int mid = (l + r) / 2;
        node->left = build(arr, l, mid);
        node->right = build(arr, mid + 1, r);
        node->max_val = max(node->left->max_val, node->right->max_val);
    }
    return node;
}

// 更新线段树
void update(SegmentTreeNode* node, int idx, long long value) {
    if (node->l == node->r) {
        node->max_val = value;
    } else {
        if (idx <= node->left->r) {
            update(node->left, idx, value);
        } else {
            update(node->right, idx, value);
        }
        node->max_val = max(node->left->max_val, node->right->max_val);
    }
}

// 查询区间最大值
long long query_max(SegmentTreeNode* node, int ql, int qr) {
    if (node->r < ql || node->l > qr) {
        return -1e18; // 返回负无穷
    }
    if (ql <= node->l && node->r <= qr) {
        return node->max_val;
    }
    return max(query_max(node->left, ql, qr), query_max(node->right, ql, qr));
}

// 查找第一个满足条件的位置
int find_first(SegmentTreeNode* node, int ql, int qr, long long x) {
    if (node->l == node->r) {
        if (ql <= node->l && node->l <= qr && node->max_val >= x) {
            return node->l;
        } else {
            return 1e9 + 7; // 返回正无穷
        }
    }
    // 检查左子树
    auto left = node->left;
    if (left->max_val >= x) {
        int left_l = max(left->l, ql);
        int left_r = min(left->r, qr);
        if (left_l <= left_r) {
            int pos = find_first(left, ql, qr, x);
            if (pos != 1e9 + 7) {
                return pos;
            }
        }
    }
    // 检查右子树
    auto right = node->right;
    if (right->max_val >= x) {
        int right_l = max(right->l, ql);
        int right_r = min(right->r, qr);
        if (right_l <= right_r) {
            int pos = find_first(right, ql, qr, x);
            if (pos != 1e9 + 7) {
                return pos;
            }
        }
    }
    return 1e9 + 7; // 返回正无穷
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, Q;
    cin >> N >> Q;
    vector<long long> a(N + 1); // 1-based indexing
    for (int i = 1; i <= N; ++i) {
        cin >> a[i];
    }

    // 构建线段树
    SegmentTreeNode* root = build(a, 1, N);

    const int INF = 1e9 + 7;
    while (Q--) {
        string op;
        cin >> op;
        if (op == "1") {
            int x, y;
            cin >> x >> y;
            update(root, x, y);
        } else {
            int l, r;
            long long x;
            cin >> l >> r >> x;
            long long current_max = query_max(root, l, r);
            if (current_max < x) {
                cout << N + 1 << "\n";
            } else {
                int pos = find_first(root, l, r, x);
                cout << (pos != INF ? pos : N + 1) << "\n";
            }
        }
    }

    return 0;
}