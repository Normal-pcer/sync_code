import os
import re

ops='+-*/%=<>,?:!^&|~;()#'

for root,dirs,files in os.walk(r"."):
    for name in files:
        if name.endswith(".cpp"):
            s=""
            print(os.path.join(root,name))
            with open(os.path.join(root,name),"r", encoding="UTF-8") as f:
                s=f.read()
            ans=""
            flag=False
            tmp=""
            for i in range(len(s)):
                if s[i]=='\"':
                    if not flag:
                        flag=True
                        tmp=tmp.replace("    ","\t")
                        for op in ops:
                            tmp=tmp.replace(" "+op,op)
                            tmp=tmp.replace(op+" ",op)
                        tmp=re.sub(r"\s*\n+\s*{","{",tmp)
                        ans+=tmp
                        tmp=""
                    else:
                        if i!=0 and s[i-1]!='\\':
                            flag=False
                if flag:
                    ans+=s[i]
                else:
                    tmp+=s[i]
            tmp=tmp.replace("    ","\t")
            for op in ops:
                tmp=tmp.replace(" "+op,op)
                tmp=tmp.replace(op+" ",op)
            tmp=re.sub(r"\s*\n+\s*{","{",tmp)
            ans+=tmp
            if re.search(r"#define\s*int\s*long\s*long",ans):
                ans=re.sub(r"#define\s*int\s*long\s*long","using ll=long long;",ans)
                ans=ans.replace("int","ll")
            ans=re.sub(r"signed main","int main",ans)
            with open(os.path.join(root,name),"w", encoding="UTF-8") as f:
                f.write(ans)
