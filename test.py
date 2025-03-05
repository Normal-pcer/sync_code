import sys
sys.setrecursionlimit(1 << 25)

class SegmentTreeNode:
    def __init__(self, l, r):
        self.l = l
        self.r = r
        self.left = None
        self.right = None
        self.max_val = 0

def build(arr, l, r):
    node = SegmentTreeNode(l, r)
    if l == r:
        node.max_val = arr[l]
    else:
        mid = (l + r) // 2
        node.left = build(arr, l, mid)
        node.right = build(arr, mid+1, r)
        node.max_val = max(node.left.max_val, node.right.max_val)
    return node

def update(node, idx, value):
    if node.l == node.r:
        node.max_val = value
    else:
        if idx <= node.left.r:
            update(node.left, idx, value)
        else:
            update(node.right, idx, value)
        node.max_val = max(node.left.max_val, node.right.max_val)

def query_max(node, ql, qr):
    if node.r < ql or node.l > qr:
        return -float('inf')
    if ql <= node.l and node.r <= qr:
        return node.max_val
    return max(query_max(node.left, ql, qr), query_max(node.right, ql, qr))

def find_first(node, ql, qr, x):
    if node.l == node.r:
        if ql <= node.l <= qr and node.max_val >= x:
            return node.l
        else:
            return float('inf')
    # Check left child
    left = node.left
    if left.max_val >= x:
        left_l = max(left.l, ql)
        left_r = min(left.r, qr)
        if left_l <= left_r:
            pos = find_first(left, ql, qr, x)
            if pos != float('inf'):
                return pos
    # Check right child
    right = node.right
    if right.max_val >= x:
        right_l = max(right.l, ql)
        right_r = min(right.r, qr)
        if right_l <= right_r:
            pos = find_first(right, ql, qr, x)
            if pos != float('inf'):
                return pos
    return float('inf')

def main():
    import sys
    input = sys.stdin.read().split()
    ptr = 0
    N = int(input[ptr])
    ptr +=1
    Q = int(input[ptr])
    ptr +=1
    a = list(map(int, input[ptr:ptr+N]))
    ptr +=N
    a = [0] + a  # Convert to 1-based indexing
    root = build(a, 1, N)
    INF = float('inf')
    for _ in range(Q):
        parts = input[ptr:ptr+4]
        if parts[0] == '1':
            x = int(parts[1])
            y = int(parts[2])
            update(root, x, y)
            ptr +=3
        else:
            l = int(parts[1])
            r = int(parts[2])
            x = int(parts[3])
            current_max = query_max(root, l, r)
            ptr +=4
            if current_max < x:
                print(N+1)
            else:
                pos = find_first(root, l, r, x)
                print(pos if pos != INF else N+1)

if __name__ == '__main__':
    main()