print(' '.join([i[::-1]if'/'<i[0]<':'else i.swapcase()for i in input().split()][::-1]))
