import random

random.seed(163337)

with open('1.in', 'w', newline='\n') as f:
    for i in range(65536):
        str = ''.join([chr(random.randint(ord('a'), ord('z'))) for _ in range(1000)])
        f.write(str + '\n')

