import sys

def main():
    S = sys.stdin.readline().strip()
    T = sys.stdin.readline().strip()
    K = int(sys.stdin.readline())
    C = []
    A = []
    for _ in range(K):
        parts = sys.stdin.readline().split()
        c = parts[0]
        a = parts[1]
        C.append(c)
        A.append(a)

    INF = float('inf')
    m = len(T)
    n = len(S)

    # 预处理字符间的最短路径
    dis = [[INF] * 26 for _ in range(26)]
    for i in range(26):
        dis[i][i] = 0
    for i in range(K):
        if len(A[i]) == 1:
            c_char = C[i]
            a_char = A[i][0]
            c = ord(c_char) - ord('a')
            a = ord(a_char) - ord('a')
            dis[c][a] = min(dis[c][a], 1)
    # Floyd算法
    for k in range(26):
        for i in range(26):
            for j in range(26):
                dis[i][j] = min(dis[i][j], dis[i][k] + dis[k][j])

    # 初始化f数组，f[l][r][c]表示字符c转换成T[l-1..r-1]的最小成本
    f = [[[INF] * 26 for _ in range(m+1)] for __ in range(m+1)]
    for i in range(m):
        c = ord(T[i]) - ord('a')
        f[i+1][i+1][c] = 0

    # g数组，g[i][l][r][k]表示操作i的A_i的前k个字符覆盖T的[l..r]区间的最小成本
    max_A_len = max(len(a) for a in A) if A else 0
    g = [[[[INF] * (max_A_len + 1) for _ in range(m+2)] for __ in range(m+2)] for ___ in range(K)]

    for l in range(1, m+1):
        for r in range(l, m+1):
            for i in range(K):
                if len(A[i]) == 0:
                    continue
                a_first = ord(A[i][0]) - ord('a')
                g[i][l][r][1] = f[l][r][a_first]

    for length in range(1, m+1):
        for l in range(1, m - length + 2):
            r = l + length - 1
            if l == r:
                continue

            # 处理每个操作i的g数组
            for i in range(K):
                a = A[i]
                a_len = len(a)
                if a_len < 2:
                    continue
                for k in range(2, a_len + 1):
                    for mid in range(l, r):
                        prev_k = k - 1
                        a_char = a[k-1]
                        c = ord(a_char) - ord('a')
                        if g[i][l][mid][prev_k] + f[mid+1][r][c] < g[i][l][r][k]:
                            g[i][l][r][k] = g[i][l][mid][prev_k] + f[mid+1][r][c]

            # 更新f数组中的操作i的贡献
            for i in range(K):
                a = A[i]
                a_len = len(a)
                if a_len < 2:
                    continue
                current_cost = g[i][l][r][a_len] + 1
                c_i = ord(C[i]) - ord('a')
                if current_cost < f[l][r][c_i]:
                    f[l][r][c_i] = current_cost

            # 使用dis数组松弛f[l][r][c]
            for c in range(26):
                for d in range(26):
                    if f[l][r][d] + dis[d][c] < f[l][r][c]:
                        f[l][r][c] = f[l][r][d] + dis[d][c]

            # 更新每个操作i的k=1的g值
            for i in range(K):
                a = A[i]
                if len(a) < 1:
                    continue
                a_char = a[0]
                a_code = ord(a_char) - ord('a')
                g[i][l][r][1] = f[l][r][a_code]

    # 处理dp数组，dp[i][j]表示S的前i个字符转换为T的前j个字符的最小成本
    dp = [[INF] * (m + 1) for _ in range(n + 1)]
    dp[0][0] = 0
    for i in range(1, n + 1):
        s_char = S[i-1]
        s_code = ord(s_char) - ord('a')
        for j in range(1, m + 1):
            for k in range(j):
                if dp[i-1][k] + f[k+1][j][s_code] < dp[i][j]:
                    dp[i][j] = dp[i-1][k] + f[k+1][j][s_code]

    if dp[n][m] == INF:
        print(-1)
    else:
        print(dp[n][m])

if __name__ == "__main__":
    main()