import sys

def main():
    S = sys.stdin.readline().strip()
    T = sys.stdin.readline().strip()
    K = int(sys.stdin.readline())
    C = []
    A = []
    for _ in range(K):
        parts = sys.stdin.readline().split()
        Ci = parts[0]
        Ai = parts[1]
        C.append(Ci)
        A.append(Ai)
    
    m = len(T)
    p = len(S)
    if p > m:
        print(-1)
        return
    
    # Preprocess Floyd-Warshall for single character transitions
    INF = 10**18
    d = [[INF] * 26 for _ in range(26)]
    for i in range(26):
        d[i][i] = 0
    for i in range(K):
        if len(A[i]) == 1:
            a_char = A[i][0]
            c_char = C[i][0]
            a = ord(a_char) - ord('a')
            c = ord(c_char) - ord('a')
            d[c][a] = min(d[c][a], 1)
    # Floyd-Warshall
    for k in range(26):
        for i in range(26):
            for j in range(26):
                d[i][j] = min(d[i][j], d[i][k] + d[k][j])
    
    # Initialize f[l][r][c]
    f = [[[INF] * 26 for _ in range(m + 2)] for __ in range(m + 2)]
    for l in range(1, m + 1):
        target_char = T[l - 1]
        tc = ord(target_char) - ord('a')
        for c in range(26):
            f[l][l][c] = d[c][tc]
    
    # Precompute lengths of A[i]
    lenA = []
    for ai in A:
        lenA.append(len(ai))
    
    # Process f[l][r][c] for all lengths
    for length in range(1, m + 1):
        for l in range(1, m - length + 2):
            r = l + length - 1
            if l > r:
                continue
            # Initialize g for this l, r
            # g[i][j] represents for operation idx i, processing j-th character of A[i]
            max_ai_len = max(lenA) if lenA else 0
            g = [[[INF] * (max_ai_len + 2) for _ in range(K)] for __ in range(m + 2)]
            for i in range(K):
                ai_len = lenA[i]
                if ai_len < 2:
                    continue
                # Initialize j=1 (first character of A[i])
                g[i][1] = f[l][r][ord(A[i][0]) - ord('a')]
                for j in range(2, ai_len + 1):
                    for k in range(l, r + 1):
                        if k >= r:
                            continue
                        prev = g[i][j - 1][k]  # Processing j-1 chars up to k
                        current_char = ord(A[i][j - 1]) - ord('a')
                        cost = prev + f[k + 1][r][current_char]
                        if cost < g[i][j][r]:
                            g[i][j][r] = cost
            # Update f[l][r][C_i] using g
            for i in range(K):
                ai_len = lenA[i]
                if ai_len < 2:
                    continue
                Ci_char = C[i][0]
                ci = ord(Ci_char) - ord('a')
                total_cost = 1 + g[i][ai_len][r]
                if total_cost < f[l][r][ci]:
                    f[l][r][ci] = total_cost
            # Relax using Floyd-Warshall
            for c in range(26):
                for via in range(26):
                    f[l][r][c] = min(f[l][r][c], f[l][r][via] + d[via][c])
    
    # Main DP
    dp = [[INF] * (m + 1) for _ in range(p + 1)]
    dp[0][0] = 0
    for i in range(1, p + 1):
        for j in range(1, m + 1):
            for k in range(0, j):
                if dp[i - 1][k] == INF:
                    continue
                current_char = S[i - 1]
                c = ord(current_char) - ord('a')
                cost = dp[i - 1][k] + f[k + 1][j][c]
                if cost < dp[i][j]:
                    dp[i][j] = cost
    result = dp[p][m]
    print(result if result < INF else -1)

if __name__ == "__main__":
    main()