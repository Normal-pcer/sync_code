x = int(input())
ans = [str(i) for i in [3, 5, 7] if x%i==0]

if ans:
    print(' '.join(ans))
else:
    print('n')
