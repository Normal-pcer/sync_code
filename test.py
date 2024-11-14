def construct_sequence(n, k):
    # 初始化序列 a_i = i
    a = list(range(1, n + 1))
    
    # 当前已经形成的倍数对数
    current_pairs = 0
    
    # 从后往前遍历，尽量不破坏前面的倍数关系
    for i in range(n - 1, 0, -1):
        for j in range(i + 1, n + 1):
            if a[j] % a[i] != 0 and current_pairs < k:
                # 找到一个合适的倍数，使得 a[j] 是 a[i] 的倍数，且 a[j] <= 2n-1
                multiple = (a[j] // a[i] + 1) * a[i]  # 找到一个比当前 a[j] 大的、a[i] 的倍数
                if multiple <= 2 * n - 1:
                    a[j] = multiple
                    current_pairs += 1
                    if current_pairs == k:
                        break
            # 如果已经找到足够的倍数对，就退出内层循环
            if current_pairs == k:
                break
        # 如果已经找到足够的倍数对，就退出外层循环
        if current_pairs == k:
            break
    
    return a

# 读取输入
n, k = map(int, input().split())

# 输出构造的序列
print(" ".join(map(str, construct_sequence(n, k))))