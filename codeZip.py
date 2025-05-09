import chardet
import os
import re

source = {}  # 文件相对位置到文件内容的映射

if __name__ == '__main__':
    # 读取源文件
    SOURCE_MATCH = re.compile(r"(.*\.hpp)")
    TARGET_MATCH = re.compile(r".*\.cpp")

    for root, dirs, files in os.walk('.'):
        for file in files:
            if re.match(SOURCE_MATCH, file):
                fullName = os.path.join(root, file)
                print(fullName)

                with open(fullName, 'rb') as f:
                    binary = f.read()
                    encoding = chardet.detect(binary)["encoding"]
                    string = binary.decode(encoding)
                
                string = string.replace('\r\n', '\n')
                string = string.replace('\n\n\n', '\n')
                source[fullName] = string

                with open(fullName, 'w', encoding="UTF-8", newline='\n') as f:
                    f.write(string)
    
    print("Source files:", source.keys())

    for root, dirs, files in os.walk('.'):
        for file in files:
            if re.match(TARGET_MATCH, file):
                fullName = os.path.join(root, file)
                with open(fullName, 'rb') as f:
                    print(fullName)
                    binary = f.read()
                    encoding = chardet.detect(binary)["encoding"] or "UTF-8"
                    string = binary.decode(encoding)
                string = string.replace('\r\n', '\n')
                string = string.replace('\n\n\n', '\n\n')
                for key in source:
                    string = string.replace(source[key], "#include \"{}\"\n".format(key.replace("\\", '/')))
                
                with open(fullName, 'w', encoding="UTF-8", newline='\n') as f:
                    f.write(string)
