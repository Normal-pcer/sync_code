import sys

class FenwickTree:
    def __init__(self, size):
        self.size = size
        self.tree = [0] * (self.size + 2)
    
    def update(self, idx, val):
        while idx <= self.size:
            self.tree[idx] += val
            idx += idx & -idx
    
    def query(self, idx):
        res = 0
        while idx > 0:
            res += self.tree[idx]
            idx -= idx & -idx
        return res

def main():
    input = sys.stdin.read().split()
    ptr = 0
    n = int(input[ptr])
    k = int(input[ptr+1])
    ptr += 2
    
    elements = []
    for _ in range(n):
        a = int(input[ptr])
        b = int(input[ptr+1])
        c = int(input[ptr+2])
        elements.append((a, b, c))
        ptr += 3
    
    elements.sort()
    
    # Merge duplicates
    nodes = []
    if elements:
        current = elements[0]
        count = 1
        for elem in elements[1:]:
            if elem == current:
                count += 1
            else:
                nodes.append((current[0], current[1], current[2], count))
                current = elem
                count = 1
        nodes.append((current[0], current[1], current[2], count))
    m = len(nodes)
    
    ans = [0] * m
    
    # CDQ分治函数
    def cdq(l, r):
        if l >= r:
            return
        mid = (l + r) // 2
        cdq(l, mid)
        cdq(mid+1, r)
        
        # 归并排序按b，并处理贡献
        i = l
        j = mid + 1
        merged = []
        while i <= mid and j <= r:
            if nodes[i][1] <= nodes[j][1]:
                merged.append(nodes[i])
                i += 1
            else:
                merged.append(nodes[j])
                j += 1
        while i <= mid:
            merged.append(nodes[i])
            i += 1
        while j <= r:
            merged.append(nodes[j])
            j += 1
        
        ft = FenwickTree(k)
        # 处理左半部分对右半部分的贡献
        for idx in range(len(merged)):
            a_val, b_val, c_val, cnt = merged[idx]
            # 如果该元素属于左半部分
            if l <= merged[idx][0] <= mid:
                ft.update(c_val, cnt)
            else:
                # 该元素属于右半部分，累加贡献
                ans_idx = l + idx
                ans[ans_idx] += ft.query(c_val)
        
        # 将merged复制回原数组的[l..r]区间
        for idx in range(len(merged)):
            nodes[l + idx] = merged[idx]
    
    # 初始的a排序已经正确，调用cdq
    # 但需要重新映射索引，这里需要将nodes视为按原顺序排序
    # 由于原nodes已经按a排序，这里直接调用cdq(0, m-1)
    # 但是需要注意，在cdq函数中处理时，nodes的索引可能会被改变，这里需要重新设计
    # 重新设计nodes的结构，使其包含原索引
    indexed_nodes = []
    for idx in range(m):
        a, b, c, cnt = nodes[idx]
        indexed_nodes.append((a, b, c, cnt, idx))  # 增加原索引
    
    nodes = indexed_nodes
    
    def indexed_cdq(l, r):
        if l >= r:
            return
        mid = (l + r) // 2
        indexed_cdq(l, mid)
        indexed_cdq(mid+1, r)
        
        # 归并排序按b
        merged = []
        i = l
        j = mid + 1
        while i <= mid and j <= r:
            if nodes[i][1] <= nodes[j][1]:
                merged.append(nodes[i])
                i += 1
            else:
                merged.append(nodes[j])
                j += 1
        while i <= mid:
            merged.append(nodes[i])
            i += 1
        while j <= r:
            merged.append(nodes[j])
            j += 1
        
        ft = FenwickTree(k)
        # 处理左半部分对右半部分的贡献
        for elem in merged:
            a_val, b_val, c_val, cnt, original_idx = elem
            # 如果该元素属于左半部分（原索引 <= mid）
            if original_idx <= mid:
                ft.update(c_val, cnt)
        
        for elem in merged:
            a_val, b_val, c_val, cnt, original_idx = elem
            # 如果该元素属于右半部分（原索引 > mid）
            if original_idx > mid:
                ans[original_idx] += ft.query(c_val)
        
        # 将merged写回nodes的[l..r]区间
        for idx in range(len(merged)):
            nodes[l + idx] = merged[idx]
    
    indexed_cdq(0, m-1)
    
    # 处理每个元素的答案，包括组内的贡献
    result = [0] * n
    for i in range(m):
        a, b, c, cnt, original_idx = nodes[i]
        total = ans[i] + cnt - 1
        result[total] += cnt
    
    for d in range(n):
        print(result[d])

if __name__ == "__main__":
    main()