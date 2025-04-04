MOD = 10**9 + 7

def main():
    import sys
    sys.setrecursionlimit(1 << 25)
    n, k = map(int, sys.stdin.readline().split())
    s = list(sys.stdin.readline().strip())
    
    # 预处理轴对称条件，计算符合轴对称的填充方式数目 sym_count，并记录哪些位置的字符被确定
    # 如果无法满足轴对称条件，sym_count=0，此时valid_count=0
    sym_count = 1
    # 同时，预处理后的字符数组，对称的位置的字符可能被确定，如果无法满足，则sym_count=0
    # 预处理后的数组，每个位置可能被确定为一个字符，或者需要填充
    # 这里我们生成一个预处理后的字符数组，其中每个位置的可能字符被确定，或者需要填充的可能情况
    # 同时计算sym_count
    
    # 预处理每个对称对
    mid = (n-1)//2
    for i in range(n):
        j = n-1 - i
        if i > j:
            break
        if i == j:
            # 中间字符必须是'*'
            if s[i] == '?':
                # 必须填'*'
                s[i] = '*'
            elif s[i] != '*':
                sym_count = 0
        else:
            a = s[i]
            b = s[j]
            if a == '?' and b == '?':
                # 有三种选择：()、)(、**
                # 但是，当处理规范条件时，整个序列必须由括号开始和结束，所以只有当i=0且j=n-1时可能允许不同的情况？
                # 这里暂时按照轴对称的条件处理，后续动态规划会处理规范条件
                # 这里的sym_count *= 3，但需要看是否在轴对称条件允许的情况下
                # 但是，在动态规划处理时，只有i是'(', j是')'的对称对才能构成规范的结构
                # 因此，在预处理时，这里的对称对的处理需要允许三种可能，但动态规划会筛选出符合条件的
                # 例如，这里的填充方式数目是3，但只有那些形成括号对的才可能被动态规划选中
                # 因此，预处理时，每个这样的对称对的填充方式数目是3种可能
                sym_count *= 3
                sym_count %= MOD
            elif a == '?':
                # b是确定的，a必须填对应的对称字符
                if b == '(':
                    s[i] = ')'
                elif b == ')':
                    s[i] = '('
                elif b == '*':
                    s[i] = '*'
                else:
                    sym_count = 0
            elif b == '?':
                # a是确定的，b必须填对应的对称字符
                if a == '(':
                    s[j] = ')'
                elif a == ')':
                    s[j] = '('
                elif a == '*':
                    s[j] = '*'
                else:
                    sym_count = 0
            else:
                # 检查a和b是否对称
                if (a == '(' and b != ')') or (a == ')' and b != '(') or (a == '*' and b != '*'):
                    sym_count = 0
    
    if sym_count == 0:
        total = 1
        q_count = sum(1 for c in s if c == '?')
        for _ in range(q_count):
            total = total * 3 % MOD
        print((total - 0) % MOD)
        return
    
    # 现在，s已经被预处理为必须满足轴对称条件的填充方式，其中'?'的位置需要填对应的字符
    # 例如，对于对称对i和j，如果其中一个是'?',另一个被填充为对应的字符
    # 但是，这个预处理可能覆盖原来的字符，导致某些字符被强制填充为特定值，从而减少可能性
    # 但是，根据问题描述，输入的字符可能已经被部分确定，如果原字符与轴对称条件冲突，则sym_count=0
    # 此时，s数组中的字符已经是被预处理后的，满足轴对称条件的最小确定字符，其余的?可以自由选择对称的方式
    # 例如，对于对称对i,j都是'?',则它们必须填为()、)(、**这三种情况之一，从而贡献3种可能
    
    # 现在，需要计算这些预处理后的字符中，符合规范的数目
    # 这里的预处理可能已经将某些位置的字符固定，例如中间的字符必须为'*'
    # 动态规划处理
    
    # 预处理每个区间[i,j]中间是否全为星号，且长度<=k
    # 预处理star[i][j] 表示区间[i,j]是否全为星号，并且长度<=k
    star = [[False]*(n) for _ in range(n)]
    for i in range(n):
        for j in range(i, n):
            valid = True
            length = j -i +1
            for t in range(i, j+1):
                if s[t] != '*' and s[t] != '?':
                    valid = False
                    break
            if valid and length >=1 and length <=k:
                star[i][j] = True
            else:
                star[i][j] = False
    
    # 动态规划，dp[i][j]表示区间[i,j]形成的符合规范的轴对称超级括号序列的数目
    dp = [[0]*n for _ in range(n)]
    
    for length in range(2, n+1):
        for i in range(n - length +1):
            j = i + length -1
            # 根据预处理后的s数组，i和j的字符必须满足轴对称条件
            # 但动态规划中，我们只处理那些i和j的字符是'('和')'的情况，因为规范条件要求序列以'('开始，')'结束
            # 在预处理后的s数组中，i和j的字符可能已经被确定为对称的字符，例如，i是'(',j是')'，或者其他情况
            # 这里，只有当i的字符是'(',j的字符是')'时，才有可能形成规范的结构
            a = s[i]
            b = s[j]
            # 检查a和b是否可以是'('和')'的组合
            # 例如，在预处理后的s数组中，i和j的字符可能已经被确定为对称的，例如a是'(',b是')'
            # 或者，它们可能是'?',此时可以填为'('和')'
            # 因此，需要判断i和j的字符是否可能成为'('和')'，即：
            # a可以是'('或者'?'
            # b可以是')'或者'?'
            can_left = (a == '(' or a == '?')
            can_right = (b == ')' or b == '?')
            if not (can_left and can_right):
                continue  # 该区间无法形成规范结构
            
            # 现在，i和j的位置可以填为'('和')'
            # 需要计算所有可能的填充方式中，该区间形成规范结构的数目
            
            # 情况1：基础情况，整个区间是()或者(S)
            if length ==2:
                # 只有可能形成'()'，贡献1
                dp[i][j] = (dp[i][j] + 1) % MOD
            else:
                # 情况1a：中间部分全为星号，长度<=k
                if star[i+1][j-1]:
                    dp[i][j] = (dp[i][j] + 1) % MOD
            
            # 情况2：AB或ASB结构
            for m in range(i, j):
                # AB结构：A是[i,m], B是[m+1,j]
                # 需要A和B都是符合规范的结构
                # 并且，AB结构必须轴对称，这可能需要A和B的结构互为镜像
                # 但动态规划中已经处理了轴对称条件，所以A和B的结构自动符合轴对称条件
                # 所以，只要A和B符合规范，AB结构就符合规范
                dp[i][j] = (dp[i][j] + dp[i][m] * dp[m+1][j]) % MOD
                # ASB结构：A是[i,m], S是m+1到 m+s， B是 m+s+1到 j
                # 其中 s是星号的数量，1<=s <=k
                for s in range(1, k+1):
                    if m+1 + s > j:
                        break
                    # 检查m+1到 m+s是否全为星号
                    # 在预处理后的s数组中，这些位置的字符可能被确定为星号或者可以填为星号
                    # 需要判断这些位置是否可以全为星号
                    valid = True
                    for t in range(m+1, m+1 +s):
                        if t >=n:
                            valid = False
                            break
                        if s[t] != '*' and s[t] != '?':
                            valid = False
                            break
                    if valid:
                        if m+1 +s <= j:
                            dp[i][j] = (dp[i][j] + dp[i][m] * dp[m+1+s][j]) % MOD
            
            # 情况3：被括号包裹的结构，如(A), (SA), (AS)
            # (A)
            if i+1 <= j-1:
                dp[i][j] = (dp[i][j] + dp[i+1][j-1]) % MOD
            # (SA)
            for s_len in range(1, k+1):
                if i+1 + s_len > j-1:
                    break
                # 检查i+1到 i+1 + s_len-1是否全为星号
                valid = True
                for t in range(i+1, i+1 + s_len):
                    if s[t] != '*' and s[t] != '?':
                        valid = False
                        break
                if valid:
                    dp[i][j] = (dp[i][j] + dp[i+1+s_len][j-1]) % MOD
            # (AS)
            for s_len in range(1, k+1):
                if j-1 - s_len < i+1:
                    break
                # 检查 j-1 -s_len +1 到 j-1 是否全为星号
                valid = True
                start = j-1 - s_len +1
                end = j-1
                for t in range(start, end+1):
                    if s[t] != '*' and s[t] != '?':
                        valid = False
                        break
                if valid:
                    dp[i][j] = (dp[i][j] + dp[i+1][end - s_len]) % MOD
    
    valid_count = dp[0][n-1] if n >=2 else 0
    
    # 计算总方案数
    q_count = 0
    for i in range(n):
        j = n-1 -i
        if i > j:
            break
        if i == j:
            if s[i] == '?':
                # 在预处理中已经固定为'*'，所以此处不贡献选择
                pass
        else:
            a = s[i]
            b = s[j]
            if a == '?' and b == '?':
                q_count +=1  # 3种可能，贡献一个乘法因子3
            elif a == '?' or b == '?':
                # 已经确定了一个字符，另一个被确定，不贡献选择
                pass
            else:
                # 无贡献
                pass
    
    # 总方案数是 3^q_count * sym_count_pre，但sym_count_pre可能已经处理了部分选择
    # 例如，对于每个对称对，如果是两个?，贡献3种可能，此时在预处理时sym_count *=3
    # 但现在，sym_count已经计算了所有对称对的填充方式数目
    
    # 因此，总方案数是 sym_count
    
    # 但原问题中的总方案数是所有可能的填充方式，不管是否满足轴对称条件
    # 我们需要重新计算总方案数，即每个问号位置的自由选择数目
    
    original_s = list(sys.stdin.readline().strip()) if False else s  # 这行代码有问题，因为之前的s已经被修改
    # 正确的做法是重新读取原始输入
    # 由于之前的处理已经覆盖了s数组，我们需要重新读取原始字符串
    # 所以，需要重新处理原始输入
    # 重新计算总方案数中的问号数目
    original_s = list(sys.stdin.readline().strip().split()[1])
    q_count_total = 0
    for c in original_s:
        if c == '?':
            q_count_total +=1
    total = pow(3, q_count_total, MOD)
    
    ans = (total - valid_count * sym_count) % MOD
    ans = (ans + MOD) % MOD  # 确保非负
    print(ans)
    
if __name__ == '__main__':
    main()