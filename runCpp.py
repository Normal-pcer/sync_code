import os, sys, time
from typing import Dict, List, Optional, Tuple
from abc import abstractmethod
import re
import json

class Option:
    switch: List[str]
    switchNone: Optional[List[str]]
    name: str
    data: List[str] | bool
    noAttachedData: bool

    def __init__(self, name: str, switch: List[str] | str, switchNone: Optional[List[str] | str], 
                    noAttachedData = True):
        self.name = name
        if isinstance(switch, list):
            self.switch = switch
        else:
            self.switch = [switch]
        if switchNone is not None:
            if isinstance(switchNone, list):
                self.switchNone = switchNone
            else:
                self.switchNone = [switchNone]
        self.noAttachedData = noAttachedData
        self.data = False if noAttachedData else []
    
    @abstractmethod
    def apply(self):  pass

class DebugOption(Option):
    def __init__(self):
        super().__init__("debug", "d", "nd")
    
    def apply(self):
        global debug
        runArgs.append("-d")
        debug = True

class ReplaceOption(Option):
    def __init__(self):
        super().__init__("replace", "r", "nr")

    def apply(self):
        code = ""
        with open(fileName, "r", encoding="UTF-8") as f:
            code = f.read()
        def callback(s):
            with open(os.path.join( os.path.split(fileName)[0], s[1] ), "r", encoding="UTF-8") as f:
                return f.read()
        code = re.sub("#include \"(.*)\"", callback, code)
        with open(fileName, "w", encoding="UTF-8") as f:
            f.write(code)

class CompileOption(Option):
    def __init__(self):
        super().__init__("compile", "c", "nc", False)
    
    def apply(self):
        global compileArgs
        # 从self.data中获取编译参数
        # +... 表示添加一个参数
        # ~... 表示删除一个参数（匹配开头的全部删除）
        for arg in self.data:
            if arg.startswith("+"):
                compileArgs.append(arg[1:])
            elif arg.startswith("~"):
                compileArgs = [x for x in compileArgs if not x.startswith(arg[1:])]

class AutoInputOption(Option):  # 自动生成输入语句
    def __init__(self):
        super().__init__("autoInput", "i", "ni")

    def apply(self):
        TYPE_KEYS = {"int": "%d", "float": "%f", "ll": "%lld", "double": "%lf", "char": "%c", "long long": "%lld"}

        code = ""
        # 匹配需要输入的注释
        with open(fileName, "r", encoding="UTF-8") as f:
            code = f.read()
        blocks = re.finditer(r"//.*input:.*\n(//.*\n)*", code)
        for block in blocks:
            words = block.group().replace("//", '').split(';')
            if len(words)>=1:  words[0] = words[0].replace("input:", '')
            words = [i.replace("\n", "") for i in words]
            initial: List[str] = []
            inMain: List[str] = []
            for line in words:
                # 推测变量类型
                varType = "int"
                line = line.strip()
                for key in TYPE_KEYS:
                    if line.startswith(key):
                        varType = key
                        break
                # 解析接下来的内容
                line = line[len(varType)+1:]
                lis = line.split(',')

                listInput = False
                fmt = ""
                args = []
                listLen = 0

                # 可能出现：
                # a - 单个变量
                # a<=100 - 有上限变量
                # a=100 - 有初值变量
                # a[n] - 数组
                # a[n]=v... - 有初值数组
                for item in lis:
                    item = item.replace(' ', '').replace('\n', '')
                    if '<=' in item:
                        name, limit = item.split('<=')
                        initial.append(f"const {varType} _{name} = {limit}")
                        initial.append(f"{varType} {name} = {limit}")
                        inMain.append(f'scanf("{TYPE_KEYS[varType]}", &{name})')
                    elif '[' in item:
                        if '=' in item:
                            name, count, init = re.findall("(.*)\[(.*)\]=(.*)", item.replace(' ', ''))[0]
                        else:
                            print(item.replace(' ', ''))
                            name, count = re.findall("(.*)\[(.*)\]", item.replace(' ', ''))[0]
                            init = None

                        try: 
                            count = int(count)
                            initial.append(f"{varType} {name}[{count}]")
                        except ValueError:
                            count = '_' + count
                            initial.append(f"{varType} {name}[{count}]")

                        if init is not None:
                            inMain.append(f"std::fill({name}, {name}+{count}, {init})")
                        else:
                            listInput = True
                            fmt += f"{TYPE_KEYS[varType]}"
                            args.append(f"{name}+i")
                            listLen = count
                    elif '=' in item:
                        initial.append(f"{varType} "+item)
                    else:
                        if len(item.strip()) == 0:  continue
                        name = item
                        initial.append(f"{varType} {name} = {limit}")
                        inMain.append(f'scanf("{TYPE_KEYS[varType]}", &{name})')
                if listInput:
                    inMain.append(f'upto(i, {listLen}) scanf("{fmt}", {','.join(args)})')
            #print('; '.join(initial)+';\n')
            #print('; '.join(inMain)+';\n')
            code = (code[:block.span()[0]] + '; '.join(initial)+';\n' + code[block.span()[1]+1:])
            code = (re.sub("main.*\{", lambda p:p[0]+'; '.join(inMain)+';\n', code))
            with open(fileName, "w", encoding="UTF-8") as f:
                f.write(code)



class LogOption(Option):
    def __init__(self):
        super().__init__("log", "l", "nl")
    
    def apply(self):
        global log
        log = True

options: Dict[str, Option] = {
    "debug": DebugOption(),
    "replace": ReplaceOption(),
    "compile": CompileOption(),
    "log": LogOption(),
    "autoInput": AutoInputOption()
}
switchMap: Dict[str, Option] = {}
switchNoneMap: Dict[str, Option] = {}
log: bool = False

baseDir = os.path.join( os.getenv("localAppdata"), "RunCpp/" )

compileArgs = ['-std=c++20', '-O2', '-Wall', '-Wextra']
runArgs = []

fileName = ""
debug = False

if __name__ == '__main__':
    if not os.path.exists(baseDir):
        os.makedirs(baseDir)

    if len(sys.argv) <= 1:
        raise Exception("未指定文件")

    defaultConfig = {}
    try:
        defaultConfig = json.load(open( os.path.join(baseDir, 'config.json'), "r", encoding="UTF-8" ))
    except Exception:
        pass

    for cfg in defaultConfig:
        option = options.get(cfg)
        if option is not None:  option.data = defaultConfig[cfg]

    fileName = sys.argv[1]
    if not fileName.endswith('.cpp'):   fileName = fileName + ".cpp"

    for name in options:
        for s in options[name].switch:
            switchMap[s] = options[name]

        for s in options[name].switchNone:
            switchNoneMap[s] = options[name]
    index = 2
    while index < len(sys.argv):
        arg = sys.argv[index]
        setDefault = False
        if arg.startswith('--'):
            setDefault = True
            arg = arg[2:]
        elif arg.startswith('-'):
            arg = arg[1:]
        else:
            print("无法解析的参数: ", arg)
        positiveOption = switchMap.get(arg)
        negativeOption = switchNoneMap.get(arg)
        
        def getAttached(start: int):
            # 从argIndex向后读取argv，直到遇到"-"或"--"开头的参数
            attached = []
            index = start
            while index < len(sys.argv) and not sys.argv[index].startswith('-'):
                attached.append(sys.argv[index])
                index += 1
            return attached, index

        if positiveOption is not None:
            if positiveOption.noAttachedData:
                positiveOption.data = True
            else:
                positiveOption.data, index = getAttached(index+1)
            if setDefault:
                defaultConfig[positiveOption.name] = positiveOption.data

        elif negativeOption is not None:
            negativeOption.data = False
            if setDefault:
                defaultConfig[negativeOption.name] = negativeOption.data
        
        else:
            print("无法解析的参数: ", arg)
        index += 1

    for name in options:
        option = options[name]
        if option.data != False:
            option.apply()
    
    if log:
        print(">", f"g++ {fileName} -o {fileName[:-4]}.exe " + ' '.join(compileArgs))
    print("正在编译...", end='\r')
    os.system(f"g++ {fileName} -o {fileName[:-4]}.exe " + ' '.join(compileArgs))

    tm = time.perf_counter()
    if log: 
        print(">", f".\\{fileName[:-4]}.exe ".replace('/', '\\') + ' '.join(runArgs))
    print("="*8 + ("DEBUG" if debug else "=====") + "="*8)
    returns = os.system(f".\\{fileName[:-4]}.exe ".replace('/', '\\') + ' '.join(runArgs))
    print("="*21)
    print("Exited after {:.4f}s, with code {}.".format(time.perf_counter()-tm, returns))


    json.dump(defaultConfig, open( os.path.join(baseDir, 'config.json'), "w", encoding="UTF-8" ))
