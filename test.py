def maximize_dissimilarity(n, S):
    T = []
    current_char = 'a'
    for s_char in S:
        # 选择一个与 s_char 不同的字符
        # 简单地通过循环 'a' 到 'z' 来实现
        # 如果 current_char 与 s_char 相同，则跳到下一个字符
        while current_char == s_char:
            current_char = chr((ord(current_char) - ord('a') + 1) % 26 + ord('a'))
        T.append(current_char)
        # 为了下一次迭代，可能需要更新 current_char
        # 但由于我们只需要任意一种解，所以这里不需要真正地“跟踪”使用过的字符
        # 简单地继续循环即可（这会导致在某些情况下字符会重复，但这仍然是一个有效的解）
        # 如果想要避免在 T 中重复使用相同的字符（尽管这不是必需的），
        # 可以使用一个集合来跟踪已经使用过的字符，并在选择新字符时进行检查。
        # 但由于题目要求输出任意一种解，并且 n 可以远大于 26，
        # 因此这种方法在这里是不必要的。
    return ''.join(T)

# 读取输入
n = int(input())
S = input().strip()

# 计算并输出结果
print(maximize_dissimilarity(n, S))