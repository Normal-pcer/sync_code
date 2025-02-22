import sys
import math
from math import gcd
from collections import defaultdict

def main():
    input = sys.stdin.read().split()
    ptr = 0
    N = int(input[ptr])
    ptr +=1
    A = list(map(int, input[ptr:ptr+N]))
    ptr +=N
    Q = int(input[ptr])
    ptr +=1
    queries = []
    for _ in range(Q):
        L = int(input[ptr])
        R = int(input[ptr+1])
        queries.append((L, R))
        ptr +=2
    
    # Preprocess each r's gcd list
    gcd_lists = [[] for _ in range(N+1)]  # 1-based
    for r in range(1, N+1):
        current_val = A[r-1]
        new_gcds = []
        # Previous gcds from r-1
        if r-1 >=1:
            for g, s in gcd_lists[r-1]:
                new_g = math.gcd(g, current_val)
                new_gcds.append((new_g, s))
        # Add the single element
        new_gcds.append((current_val, r))
        # Merge the same gcds
        merged = []
        for g, s in reversed(new_gcds):
            if merged and merged[-1][0] == g:
                continue
            merged.append((g, s))
        # Reverse back to maintain increasing start order
        merged.reverse()
        gcd_lists[r] = merged
    
    # Precompute log values for sparse table
    logn = math.floor(math.log2(N)) +1
    st_gcd = [[0]*(N+1) for _ in range(logn)]
    st_gcd[0] = [0] + A.copy()
    for k in range(1, logn):
        for i in range(1, N+1 - (1<<k) +1):
            st_gcd[k][i] = math.gcd(st_gcd[k-1][i], st_gcd[k-1][i + (1<<(k-1))])
    
    # Function to compute interval gcd
    def compute_gcd(L, R):
        length = R - L +1
        k = length.bit_length() -1
        return math.gcd(st_gcd[k][L], st_gcd[k][R - (1<<k) +1])
    
    # Process queries
    results = []
    for L, R in queries:
        g = compute_gcd(L, R)
        total =0
        for r in range(L, R+1):
            found = False
            for current_g, s in gcd_lists[r]:
                if current_g == g:
                    left = max(s, L)
                    if left > r:
                        continue
                    cnt = r - left +1
                    total += cnt
                    found = True
                    break
            # If not found, check if single element
            if not found:
                if A[r-1] == g:
                    total +=1
        results.append(f"{g} {total}")
    
    print('\n'.join(results))

if __name__ == '__main__':
    main()