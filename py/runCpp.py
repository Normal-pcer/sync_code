import os, sys, time
from typing import Dict, List
from abc import abstractmethod
import re
import json

class Option:
    switch: List[str]
    switchNone: List[str] | None
    name: str
    data: str | bool
    noAttachedData: bool

    def __init__(self, name: str, switch: List[str] | str, switchNone: List[str] | str | None, noAttachedData = True):
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
        self.data = False if noAttachedData else ""
    
    @abstractmethod
    def apply(self):  pass

class DebugOption(Option):
    def __init__(self):
        super().__init__("debugEnabled", "d", "nd")
    
    def apply(self):
        global debug
        runArgs.append("-d")
        debug = True

class ReplaceOption(Option):
    def __init__(self):
        super().__init__("replaceEnabled", "r", "nr")

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

options: Dict[str, Option] = {
    "debugEnabled": DebugOption(),
    "replaceOption": ReplaceOption()
}
switchMap: Dict[str, Option] = {}
switchNoneMap: Dict[str, Option] = {}

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
        if arg.startswith('-'):
            arg = arg[1:]
        positiveOption = switchMap.get(arg)
        negativeOption = switchNoneMap.get(arg)

        if positiveOption is not None:
            if positiveOption.noAttachedData:
                positiveOption.data = True
            else:
                positiveOption.data = sys.argv[index+1]
            if setDefault:
                defaultConfig[positiveOption.name] = positiveOption.data

        if negativeOption is not None:
            negativeOption.data = False
            if setDefault:
                defaultConfig[negativeOption.name] = negativeOption.data
        index += 1

        if negativeOption is None and positiveOption is None:
            compileArgs.append(("--" if setDefault else "-") + arg)

    for name in options:
        option = options[name]
        if option.data != False:
            option.apply()

    os.system(f"g++ {fileName} -o {fileName[:-4]}.exe " + ' '.join(compileArgs))

    tm = time.perf_counter()
    print("="*8 + ("DEBUG" if debug else "=====") + "="*8)
    returns = os.system(f".\\{fileName[:-4]}.exe ".replace('/', '\\') + ' '.join(runArgs))
    print("="*21)
    print("Stopped after {:.4f}s, with code {}.".format(time.perf_counter()-tm, returns))


    json.dump(defaultConfig, open( os.path.join(baseDir, 'config.json'), "w", encoding="UTF-8" ))
