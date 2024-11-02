import re;r,f,p=input,lambda x:len(x)**2+len(x),re.split
for _ in"_"*int(r()):r();s=r();print(sum(map(f,p('<',s)+p('>',s)))//2-sum(map(f,p("<|>",s))))