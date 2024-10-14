import os, sys, time, shutil
from typing import Dict, List, Optional, Tuple
from traceback import print_exc
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
            try:
                with open(os.path.join( os.path.split(fileName)[0], s[1] ), "r", encoding="UTF-8") as f:
                    return f.read()
            except FileNotFoundError:
                with open(os.path.join( os.path.split(fileName)[0], s[1]+'.hpp' ), "r", encoding="UTF-8") as f:
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
                else:
                    line = "int " + line
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
                        if '0'<=name[0]<='9':  name = '_' + name
                        initial.append(f"const {varType} _{name} = {limit}")
                        initial.append(f"{varType} {name}")
                        inMain.append(f'scanf("{TYPE_KEYS[varType]}", &{name})')
                    elif '[' in item:
                        if '=' in item:
                            name, count, init = re.findall(r"(.*?)(\[.*\])=(.*)", item.replace(' ', ''))[0]
                        else:
                            name, count = re.findall(r"(.*?)(\[.*\])", item.replace(' ', ''))[0]
                            init = None
                        def callback(index):
                            try:
                                index = int(index)
                            except ValueError:
                                index = '_' + index
                            return index
                        count: str = re.sub(r"\[(.*?)\]", lambda m: f'[{callback(m[1])}]', count)
                        if '0'<=name[0]<='9':  name = '_' + name
                        initial.append(f"{varType} {name}{count}")

                        if init is not None:
                            init: str
                            initCode = ""
                            if init == "0":
                                initCode = ""
                            elif init in {"inf", "0x3f3f3f3f"}:
                                initCode = f"std::memset({name}, 0x3f, sizeof({name}))"
                            elif init in {"-inf", "-0x3f3f3f3f"}:
                                initCode = f"std::memset({name}, -0x3f, sizeof({name}))"
                            elif init.startswith("..."):
                                initCode = f"std::memset({name}, {init[3:]}, sizeof({name}))"
                            else:
                                initCode = f"std::fill({name}, {name}+{count[1:-1]}, {init})"
                            if initCode: inMain.append(initCode)
                        else:
                            listInput = True
                            fmt += f"{TYPE_KEYS[varType]}"
                            args.append(f"{name}+i")
                            listLen = count[1:-1]
                    elif '=' in item:
                        initial.append(f"{varType} "+item)
                        if '===' in item:
                            initial[-1] = "const "+ initial[-1]
                    else:
                        if len(item.strip()) == 0:  continue
                        name = item
                        if '0'<=name[0]<='9':  name = '_' + name
                        initial.append(f"{varType} {name}")
                        inMain.append(f'scanf("{TYPE_KEYS[varType]}", &{name})')
                if listInput:
                    inMain.append(f'upto(i, {listLen[1:]}) scanf("{fmt}", {','.join(args)})')
            #print('; '.join(initial)+';\n')
            #print('; '.join(inMain)+';\n')
            code = (code[:block.span()[0]] + '; '.join(initial)+';\n' + code[block.span()[1]+1:])
            code = (re.sub(r"main.*\{", lambda p:p[0]+'; '.join(inMain)+';\n', code))
            with open(fileName, "w", encoding="UTF-8") as f:
                f.write(code)

class AutoInitializeOption(Option):  # 自动创建和填充模板
    def __init__(self):
        super().__init__("autoInitialize", "autoinit", "noautoinit")
    def apply(self):
        global autoInitialize
        autoInitialize = True

class LogOption(Option):
    def __init__(self):
        super().__init__("log", "l", "nl")
    
    def apply(self):
        global log
        log = True

class BreakOption(Option):
    def __init__(self):
        super().__init__("break", "b", "nb")
    
    def apply(self):
        global breakBeforeExecute
        breakBeforeExecute = True

def isInt(n: str):
    try:
        int(n)
        return True
    except ValueError:
        return False

options: Dict[str, Option] = {
    "debug": DebugOption(),
    "replace": ReplaceOption(),
    "compile": CompileOption(),
    "log": LogOption(),
    "autoInput": AutoInputOption(),
    "autoInitialize": AutoInitializeOption(),
    "break": BreakOption()
}
switchMap: Dict[str, Option] = {}
switchNoneMap: Dict[str, Option] = {}
log: bool = False
autoInitialize = False

baseDir = os.path.join( os.getenv("localAppdata"), "RunCpp/" )

compileArgs = ['-std=c++20', '-O2', '-Wall', '-Wextra', '-finput-charset=utf-8']
runArgs = []

fileName = ""
debug = False
breakBeforeExecute = False

if __name__ == '__main__':
    try:
        if not os.path.exists(baseDir):
            os.makedirs(baseDir)

        defaultConfig = {}
        try:
            defaultConfig = json.load(open( os.path.join(baseDir, 'config.json'), "r", encoding="UTF-8" ))
        except Exception:
            pass
        
        previousFile = ""
        try: 
            with open( os.path.join(baseDir, 'previousFile.txt'), "r", encoding="UTF-8" ) as f:
                previousFile = f.read()
        except Exception:
            pass

        for cfg in defaultConfig:
            option = options.get(cfg)
            if option is not None:  option.data = defaultConfig[cfg]

        for name in options:
            for s in options[name].switch:
                switchMap[s] = options[name]

            for s in options[name].switchNone:
                switchNoneMap[s] = options[name]
        index = 1
        while index < len(sys.argv):
            arg = sys.argv[index]
            setDefault = False
            if arg.startswith('--'):
                setDefault = True
                arg = arg[2:]
            elif arg.startswith('-'):
                arg = arg[1:]
            elif index != 1:
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
                if index != 1:  
                    print("无法解析的参数: ", arg)
            index += 1
        json.dump(defaultConfig, open( os.path.join(baseDir, 'config.json'), "w", encoding="UTF-8" ))

        fileName = sys.argv[1] if len(sys.argv)>=2 else previousFile
        fileName = fileName.replace(' ', '_')
        if not fileName.endswith('.cpp'):   fileName = fileName + ".cpp"
        if not os.path.exists(fileName):
            if not options["autoInitialize"].data:
                raise FileNotFoundError("未找到文件")
            if fileName == "new.cpp":
                maxIndex = max(map( lambda s:int(s[:-4]) , filter(lambda s: s.endswith(".cpp") and isInt(s[:-4]), os.listdir()) ))
                maxIndex += 1
                maxIndex = str(maxIndex)
            else:
                maxIndex = fileName[:-4]
                previousFile = fileName.replace(' ', '_')
            fileName = str(maxIndex) + '.cpp'
            with open( os.path.join(baseDir, 'previousFile.txt'), "w", encoding="UTF-8" ) as f:
                f.write(previousFile)
            shutil.copyfile("init.cpp", fileName)
            os.system(" ".join(["code", '"' + fileName + '"']))
            sys.exit()

        for name in options:
            option = options[name]
            if option.data != False:
                option.apply()
        
        previousFile = fileName
        with open( os.path.join(baseDir, 'previousFile.txt'), "w", encoding="UTF-8" ) as f:
            f.write(previousFile)
        

        if log:
            print(">", f"g++ \"{fileName}\" -o \"{fileName[:-4]}.exe\" " + ' '.join(compileArgs))
        print("正在编译...", end='\r')
        os.system(f"g++ \"{fileName}\" -o \"{fileName[:-4]}.exe\" " + ' '.join(compileArgs))

        if breakBeforeExecute:
            sys.exit(0)

        tm = time.perf_counter()
        if log: 
            print(">", f"\"{fileName[:-4]}.exe\" ".replace('/', '\\') + ' '.join(runArgs))
        print("="*8 + ("DEBUG" if debug else "=====") + "="*8)
        returns = os.system(f"\"{fileName[:-4]}.exe\" ".replace('/', '\\') + ' '.join(runArgs))
        print("="*21)
        print("Exited after {:.4f}s, with code {}.".format(time.perf_counter()-tm, returns))
    except KeyboardInterrupt:
        print("终止")
    except Exception as e:
        print("未捕获的错误:", e)
        print_exc()
