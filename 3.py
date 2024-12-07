# 在dir1(旧)和dir2(新)中遍历所有的文件，并输出它们的差异
# 对于每个文件的哈希进行比较，如果新文件夹里出现了旧文件夹中不存在的哈希则为“新增”，反之为删除
# 这个匹配到的字符串相同则判定为这两个文件相同
# 如果新增了文件，输出+[新文件名]
# 如果删除了文件，输出-[旧文件名]
# 相同文件不给予输出

import hashlib, os  
def fileMD5(filename):  
    hash_md5 = hashlib.md5()  
    with open(filename, "rb") as f:  
        for byte_block in iter(lambda: f.read(4096), b""):  
            hash_md5.update(byte_block)  
    return hash_md5

dir1 = input("DIR1:")
dir2 = input("DIR2:")
COPY_DESC = False
FLAG_NEW = True
hash1 = {}
hash2 = {}
lis1 = os.listdir(dir1)
lis2 = os.listdir(dir2)
for i in range(len(lis1)):
    file = lis1[i]
    if (file.startswith("!")):  file = file[1:]
    hash1[fileMD5(os.path.join(dir1, file)).hexdigest()] = file
    print(f"Hash1: {i}/{len(lis1)}", end='\r')
print()
for i in range(len(lis2)):
    file = lis2[i]
    hash2[fileMD5(os.path.join(dir2, file)).hexdigest()] = file
    print(f"Hash2: {i}/{len(lis2)}", end='\r')
print()
for key in hash1:
    if key not in hash2:
        print("-" + hash1[key])
    elif COPY_DESC:
        # 复制文件描述
        # 文件名中如果有单引号，这个单引号的后面叫做这个文件的描述
        # 如果旧文件有描述，则合并到新文件中
        desc1 = set(hash1[key].split("'")[1:] if "'" in hash1[key] else [])
        desc2 = set(hash2[key].split("'")[1:] if "'" in hash2[key] else [])
        newFileName = hash2[key].split("'")[0] + "'".join(desc1.union(desc2))
        if newFileName != hash2[key]:
            os.rename(os.path.join(dir2, hash2[key]), os.path.join(dir2, newFileName))
            hash2[key] = newFileName
for key in hash2:
    if key not in hash1:
        print("+" + hash2[key])
        if FLAG_NEW:
            # 文件名前面添加一个感叹号，表示这是一个新文件
            # 如果文件名中已经有感叹号，则不添加
            if not hash2[key].startswith("!"):
                os.rename(os.path.join(dir2, hash2[key]), os.path.join(dir2, "!" + hash2[key]))