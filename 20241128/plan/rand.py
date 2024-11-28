from random import randint

n = 10
with open('plan.in', 'w', newline='\n') as f:

    f.write("10 9\n")
    for i in range(2, 11):
        f.write(' '.join(map(str, (i, i-randint(1, min(i-1, 5)))))+'\n')