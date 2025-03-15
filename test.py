chief=0
bodyguard=1
elephant=2
horse=3
chariot=4
cannon=5
soldier=6

maximumHistoricalMoves=256
maximumSearchDepth=64
checkmateScore=10000
perpetualCheckNegativeScore=checkmateScore-100
scoreBoundaryVictoryAndDefeat=checkmateScore-200
scoreDuringDrawNegative=20
priorityScore=3
randomSocre=7
emptyStepCutForceBorder=400
emptyStepCutDepth=2
maximumPermutationTableSize=1048576
AlphaPermutationTableEntry=1
BetaPermutationTableEntry=2
PVPermutationTableEntry=3

isOnChessboardList=(
    0, 0, 0  # ...
)
isInGridList=(
    0, 0, 0  # ...

)
stepMatchWalkingMethodList=(
    0, 0, 0  # ...

)
horseLameLegList=(
    0, 0, 0  # ...
)

chiefStep=(-16,-1,1,16)
bodyguardStep=(-17,-15,15,17)
horseLegUseChiefStep=((-33,-31),(-18,14),(-14,18),(31,33))
horseLegUsebodyguardStep=((-33,-18),(-31,-14),(14,31),(18,33))

initialChessboard=(
    0, 0, 0  # ...

)

MvvLvaForceValue=(
    0,0,0,0,0,0,0,0,
    5,1,1,3,4,3,2,0,
    5,1,1,3,4,3,2,0
)

forcePositionValue=(
    (0), (0), (0)  # ...
)

def isOnChessboard(pos):
    return isOnChessboardList[pos]!=0

def isInGrid(pos):
    return isInGridList[pos]!=0

def getx(pos):
    return pos&15

def gety(pos):
    return pos>>4

def posToGrid(x,y):
    return x+(y<<4)

def mousePosToGrid(x,y):
    return (x+3)+((y+3)<<4)

def flipGrid(pos):
    return 254-pos

def forwardPos(pos,turn):
    return pos-16+(turn<<5)

def checkChief(start,end):
    return stepMatchWalkingMethodList[end-start+256]==1

def checkBodyguard(start,end):
    return stepMatchWalkingMethodList[end-start+256]==2

def checkEleplant(start,end):
    return stepMatchWalkingMethodList[end-start+256]==3

def elephantEye(start,end):
    return (start+end)>>1

def horseLeg(start,end):
    return start+horseLameLegList[end-start+256]

def notCrossedRiver(pos,turn):
    return (pos&0x80)!=(turn<<7)

def crossedRiver(pos,turn):
    return (pos&0x80)==(turn<<7)

def onTheSameSide(start,end):
    return ((start^end)&0x80)==0

def onTheSameLine(start,end):
    return ((start^end)&0xf0)==0

def onTheSameColumn(start,end):
    return ((start^end)&0x0f)==0

def getRedBlackMark(turn):
    return 8+(turn<<3)

def getOpponentRedBlackMark(turn):
    return 16-(turn<<3)

def getStart(method):
    return method&255

def getEnd(method):
    return method>>8

def getMethod(start,end):
    return start+(end<<8)

def revMethod(method):
    return getMethod(getEnd(method),getStart(method))

class RC4CipherStreamGenerator:
    def __init__(self):
        self.s=list(range(256))
        self.x=self.y=0

    def initCSG(self):
        self.x=self.y=j=0
        for i in range(256):
            j=(j+self.s[i])&255
            self.s[i],self.s[j]=self.s[j],self.s[i]

    def generateNextByte(self):
        self.x=(self.x+1)&255
        self.y=(self.y+self.s[self.x])&255
        self.s[self.x],self.s[self.y]=self.s[self.y],self.s[self.x]
        return self.s[(self.s[self.x]+self.s[self.y])&255]

    def generateNext4Bytes(self):
        uc0=self.generateNextByte()
        uc1=self.generateNextByte()
        uc2=self.generateNextByte()
        uc3=self.generateNextByte()
        return uc0+(uc1<<8)+(uc2<<16)+(uc3<<24)

class ZobristStructure:
    def fillWithZeros(self):
        self.duplicateSituationCheckCode=self.permutationTableCheckCode=0

    def fillWithCipherStream(self,generator):
        self.duplicateSituationCheckCode=generator.generateNext4Bytes()
        self.permutationTableCheckCode=generator.generateNext4Bytes()

    def XOROperation(self,checkcode):
        self.duplicateSituationCheckCode^=checkcode.duplicateSituationCheckCode
        self.permutationTableCheckCode^=checkcode.permutationTableCheckCode

class ZobristTable:
    ZobristPlayer=ZobristStructure()
    desktop=[[ZobristStructure()for i in range(256)]for j in range(14)]

def initZobristTable():
    RC4Generator=RC4CipherStreamGenerator()
    RC4Generator.initCSG()
    ZobristTable.ZobristPlayer.fillWithCipherStream(RC4Generator)
    for i in range(14):
        for j in range(256):
            ZobristTable.desktop[i][j].fillWithCipherStream(RC4Generator)

class HistoricalRouteInformation:
    def __init__(self):
        self.bestMove=self.previousStepEating=self.checkmate=self.duplicateSituationCheckCode=0

    def setHistoricalRoute(self,thisMove,previousStepEating,checkmate,duplicateSituationCheckCode):
        self.bestMove=thisMove
        self.previousStepEating=previousStepEating
        self.checkmate=checkmate
        self.duplicateSituationCheckCode=duplicateSituationCheckCode

class SituationStructure:
    def __init__(self):
        self.turn=0
        self.chessboard=[0 for i in range(256)]
        self.redForce=self.blackForce=0
        self.distanceToRoot=self.historicalMovementsNumber=0
        self.historicalRoutingInformationTable=[HistoricalRouteInformation() for i in range(maximumHistoricalMoves)]
        self.checkCode=ZobristStructure()

    def clear(self):
        self.turn=self.redForce=self.blackForce=self.distanceToRoot=0
        self.chessboard=[0 for i in range(256)]
        self.checkCode.fillWithZeros()

    def initHistoricalRoutingInformation(self):
        self.historicalRoutingInformationTable[0].setHistoricalRoute(0,0,self.isCheckmated(),self.checkCode.duplicateSituationCheckCode)
        self.historicalMovementsNumber=1

    def initChessboard(self):
        self.clear()
        for i in range(256):
            j=initialChessboard[i]
            if j!=0:
                self.fallChess(i,j)
        self.initHistoricalRoutingInformation()

    def swapPlayer(self):
        self.turn=1-self.turn
        self.checkCode.XOROperation(ZobristTable.ZobristPlayer)

    def fallChess(self,pos,i):
        self.chessboard[pos]=i
        if i<16:
            self.redForce+=forcePositionValue[i-8][pos]
            self.checkCode.XOROperation(ZobristTable.desktop[i-8][pos])
        else:
            self.blackForce+=forcePositionValue[i-16][flipGrid(pos)]
            self.checkCode.XOROperation(ZobristTable.desktop[i-9][pos])

    def takeChess(self,pos,i):
        self.chessboard[pos]=0
        if i<16:
            self.redForce-=forcePositionValue[i-8][pos]
            self.checkCode.XOROperation(ZobristTable.desktop[i-8][pos])
        else:
            self.blackForce-=forcePositionValue[i-16][flipGrid(pos)]
            self.checkCode.XOROperation(ZobristTable.desktop[i-9][pos])

    def situationAssessment(self):
        return (self.redForce-self.blackForce)*(1 if self.turn==0 else -1)+priorityScore

    def isCheckmatedByHistoryTable(self):
        return self.historicalRoutingInformationTable[self.historicalMovementsNumber-1].checkmate

    def eatPreviousStep(self):
        return self.historicalRoutingInformationTable[self.historicalMovementsNumber-1].previousStepEating!=0

    def moveChess(self,method):
        start,end=getStart(method),getEnd(method)
        tag=self.chessboard[end]
        if tag!=0:
            self.takeChess(end,tag)
        i=self.chessboard[start]
        self.takeChess(start,i)
        self.fallChess(end,i)
        return tag

    def revokeMoveChess(self,method,tag):
        start,end=getStart(method),getEnd(method)
        i=self.chessboard[end]
        self.takeChess(end,i)
        self.fallChess(start,i)
        if tag!=0:
            self.fallChess(end,tag)

    def makeAMove(self,method,force=False):
        code=self.checkCode.duplicateSituationCheckCode
        eaten=self.moveChess(method)
        if self.isCheckmated():
            if not force:
                self.revokeMoveChess(method,eaten)
            return False
        self.swapPlayer()
        self.historicalRoutingInformationTable[self.historicalMovementsNumber].setHistoricalRoute(method,eaten,self.isCheckmated(),code)
        self.historicalMovementsNumber+=1
        self.distanceToRoot+=1
        return True

    def revokeMakeAMove(self):
        self.distanceToRoot-=1
        self.historicalMovementsNumber-=1
        self.swapPlayer()
        tmp=self.historicalRoutingInformationTable[self.historicalMovementsNumber]
        self.revokeMoveChess(tmp.bestMove,tmp.previousStepEating)

    def emptyMove(self):
        code=self.checkCode.duplicateSituationCheckCode
        self.swapPlayer()
        self.historicalRoutingInformationTable[self.historicalMovementsNumber].setHistoricalRoute(0,0,False,code)
        self.historicalMovementsNumber+=1
        self.distanceToRoot+=1

    def revokeEmptyMove(self):
        self.distanceToRoot-=1
        self.historicalMovementsNumber-=1
        self.swapPlayer()

    def generateAllRoutes(self,eatOnly=False):
        generates=[]
        MyMark=getRedBlackMark(self.turn)
        OpponentMark=getOpponentRedBlackMark(self.turn)
        for start in range(256):
            startChess=self.chessboard[start]
            if (startChess&MyMark)==0:
                continue
            chess=startChess-MyMark
            if chess==chief:
                for i in range(4):
                    end=start+chiefStep[i]
                    if not isInGrid(end):
                        continue
                    endChess=self.chessboard[end]
                    if (endChess&OpponentMark)!=0 if eatOnly else (endChess&MyMark)==0:
                        generates.append(getMethod(start,end))
            elif chess==bodyguard:
                for i in range(4):
                    end=start+bodyguardStep[i]
                    if not isInGrid(end):
                        continue
                    endChess=self.chessboard[end]
                    if (endChess&OpponentMark)!=0 if eatOnly else (endChess&MyMark)==0:
                        generates.append(getMethod(start,end))
            elif chess==elephant:
                for i in range(4):
                    end=start+bodyguardStep[i]
                    if not(isOnChessboard(end) and notCrossedRiver(end,self.turn) and self.chessboard[end]==0):
                        continue
                    end+=bodyguardStep[i]
                    endChess=self.chessboard[end]
                    if (endChess&OpponentMark)!=0 if eatOnly else (endChess&MyMark)==0:
                        generates.append(getMethod(start,end))
            elif chess==horse:
                for i in range(4):
                    end=start+chiefStep[i]
                    if self.chessboard[end]!=0:
                        continue
                    for j in range(2):
                        end=start+horseLegUseChiefStep[i][j]
                        if not isOnChessboard(end):
                            continue
                        endChess=self.chessboard[end]
                        if (endChess&OpponentMark)!=0 if eatOnly else (endChess&MyMark)==0:
                            generates.append(getMethod(start,end))
            elif chess==chariot:
                for i in range(4):
                    offset=chiefStep[i]
                    end=start+offset
                    while isOnChessboard(end):
                        endChess=self.chessboard[end]
                        if endChess==0:
                            if not eatOnly:
                                generates.append(getMethod(start,end))
                        else:
                            if (endChess&OpponentMark)!=0:
                                generates.append(getMethod(start,end))
                            break
                        end+=offset
            elif chess==cannon:
                for i in range(4):
                    offset=chiefStep[i]
                    end=start+offset
                    while isOnChessboard(end):
                        endChess=self.chessboard[end]
                        if endChess==0:
                            if not eatOnly:
                                generates.append(getMethod(start,end))
                        else:
                            break
                        end+=offset
                    end+=offset
                    while isOnChessboard(end):
                        endChess=self.chessboard[end]
                        if endChess!=0:
                            if (endChess&OpponentMark)!=0:
                                generates.append(getMethod(start,end))
                            break
                        end+=offset
            elif chess==soldier:
                end=forwardPos(start,self.turn)
                if isOnChessboard(end):
                    endChess=self.chessboard[end]
                    if (endChess&OpponentMark)!=0 if eatOnly else (endChess&MyMark)==0:
                        generates.append(getMethod(start,end))
                if crossedRiver(start,self.turn):
                    for offset in range(-1,2,2):
                        end=start+offset
                        if isOnChessboard(end):
                            endChess=self.chessboard[end]
                            if (endChess&OpponentMark)!=0 if eatOnly else (endChess&MyMark)==0:
                                generates.append(getMethod(start,end))
        return generates

    def moveAllow(self,method):
        start=getStart(method)
        startChess=self.chessboard[start]
        MyMark=getRedBlackMark(self.turn)
        if (startChess&MyMark)==0:
            return False
        end=getEnd(method)
        endChess=self.chessboard[end]
        if (endChess&MyMark)!=0:
            return False
        chess=startChess-MyMark
        if chess==chief:
            return isInGrid(end) and checkChief(start,end)
        elif chess==bodyguard:
            return isInGrid(end) and checkBodyguard(start,end)
        elif chess==elephant:
            return onTheSameSide(start,end) and checkEleplant(start,end) and self.chessboard[elephantEye(start,end)]==0
        elif chess==horse:
            obstacleOffset=horseLeg(start,end)
            return obstacleOffset!=start and self.chessboard[obstacleOffset]==0
        elif chess==chariot or chess==cannon:
            if onTheSameLine(start,end):
                offset=-1 if end<start else 1
            elif onTheSameColumn(start,end):
                offset=-16 if end<start else 16
            else:
                return False
            obstacleOffset=start+offset
            while obstacleOffset!=end and self.chessboard[obstacleOffset]==0:
                obstacleOffset+=offset
            if obstacleOffset==end:
                return endChess==0 or startChess-MyMark==chariot
            elif endChess!=0 and startChess-MyMark==cannon:
                obstacleOffset+=offset
                while obstacleOffset!=end and self.chessboard[obstacleOffset]==0:
                    obstacleOffset+=offset
                return obstacleOffset==end
            else:
                return False
        elif chess==soldier:
            if crossedRiver(end,self.turn) and (end==start-1 or end==start+1):
                return True
            return end==forwardPos(start,self.turn)
        else:
            return False

    def isCheckmated(self):
        MyMark=getRedBlackMark(self.turn)
        OpponentMark=getOpponentRedBlackMark(self.turn)
        for start in range(256):
            if self.chessboard[start]!=MyMark+chief:
                continue
            if self.chessboard[forwardPos(start,self.turn)]==OpponentMark+soldier:
                return True
            for offset in range(-1,2,2):
                if self.chessboard[start+offset]==OpponentMark+soldier:
                    return True
            for i in range(4):
                if self.chessboard[start+bodyguardStep[i]]!=0:
                    continue
                for j in range(2):
                    endChess=self.chessboard[start+horseLegUsebodyguardStep[i][j]]
                    if endChess==OpponentMark+horse:
                        return True
            for i in range(4):
                offset=chiefStep[i]
                end=start+offset
                while isOnChessboard(end):
                    endChess=self.chessboard[end]
                    if endChess!=0:
                        if endChess==OpponentMark+chariot or endChess==OpponentMark+chief:
                            return True
                        break
                    end+=offset
                end+=offset
                while isOnChessboard(end):
                    endChess=self.chessboard[end]
                    if endChess!=0:
                        if endChess==OpponentMark+cannon:
                            return True
                        break
                    end+=offset
            return False
        return False

    def isKilled(self):
        allRoutes=self.generateAllRoutes()
        for i in allRoutes:
            eaten=self.moveChess(i)
            if not self.isCheckmated():
                self.revokeMoveChess(i,eaten)
                return False
            else:
                self.revokeMoveChess(i,eaten)
        return True

    def drawChessScore(self):
        return -scoreDuringDrawNegative if (self.distanceToRoot&1)==0 else scoreDuringDrawNegative

    def detectDuplicateSituation(self,repetitions=1):
        checkMySide=False
        thisSidePerpetualCheck=opponentPerpetualCheck=True
        chessIndexValue=self.historicalMovementsNumber-1
        while self.historicalRoutingInformationTable[chessIndexValue].bestMove!=0 and self.historicalRoutingInformationTable[chessIndexValue].previousStepEating==0:
            if checkMySide:
                thisSidePerpetualCheck=thisSidePerpetualCheck and self.historicalRoutingInformationTable[chessIndexValue].checkmate
                if self.historicalRoutingInformationTable[chessIndexValue].duplicateSituationCheckCode==self.checkCode.duplicateSituationCheckCode:
                    repetitions-=1
                    if repetitions==0:
                        return 1+(2 if thisSidePerpetualCheck else 0)+(4 if opponentPerpetualCheck else 0)
            else:
                opponentPerpetualCheck=opponentPerpetualCheck and self.historicalRoutingInformationTable[chessIndexValue].checkmate
            checkMySide=not checkMySide
            chessIndexValue-=1
        return 0

    def duplicateSituationScore(self,duplicateStatusCode):
        score=(0 if (duplicateStatusCode&2)==0 else self.distanceToRoot-perpetualCheckNegativeScore)+(0 if (duplicateStatusCode&4)==0 else perpetualCheckNegativeScore-self.distanceToRoot)
        return self.drawChessScore() if score==0 else score

    def allowEmptyStepCut(self):
        return (self.redForce if self.turn==0 else self.blackForce)>emptyStepCutForceBorder

currChessGame=SituationStructure()

class PermutationTableStructure:
    def __init__(self):
        self.depth=self.sign=self.score=self.bestMove=self.permutationTableCheckCode=0

class SearchRelatedVariables:
    chessByComputer=0
    historyTable=[0 for i in range(65536)]
    killerMethodTable=[[0,0] for i in range(maximumSearchDepth)]
    permutationTable=[PermutationTableStructure() for i in range(maximumPermutationTableSize)]

def extractPermutationTableEntry(vlAlpha,vlBeta,dep,method):
    entry=SearchRelatedVariables.permutationTable[currChessGame.checkCode.duplicateSituationCheckCode&(maximumPermutationTableSize-1)]
    if entry.permutationTableCheckCode!=currChessGame.checkCode.permutationTableCheckCode:
        method[0]=0
        return -checkmateScore
    method[0]=entry.bestMove
    killFlag=False
    if entry.score>scoreBoundaryVictoryAndDefeat:
        if entry.score<perpetualCheckNegativeScore:
            return -checkmateScore
        entry.score-=currChessGame.distanceToRoot
        killFlag=True
    elif entry.score<-scoreBoundaryVictoryAndDefeat:
        if entry.score>-perpetualCheckNegativeScore:
            return -checkmateScore
        entry.score+=currChessGame.distanceToRoot
        killFlag=True
    if enrty.depth>=dep or killFlag:
        if entry.sign==BetaPermutationTableEntry:
            return entry.score if entry.score>=vlBeta else -checkmateScore
        elif entry.sign==AlphaPermutationTableEntry:
            return entry.score if entry.score<=vlAlpha else -checkmateScore
        return entry.score
    return -checkmateScore

def savePermutationTableEntry(sign,vl,dep,method):
    entry=SearchRelatedVariables.permutationTable[currChessGame.checkCode.duplicateSituationCheckCode&(maximumPermutationTableSize-1)]
    if entry.depth>dep:
        return
    entry.sign=sign
    entry.depth=dep
    if vl>scoreBoundaryVictoryAndDefeat:
        if method==0 and vl<=perpetualCheckNegativeScore:
            return
        entry.score=vl+currChessGame.distanceToRoot
    elif vl<-scoreBoundaryVictoryAndDefeat:
        if method==0 and vl>=-perpetualCheckNegativeScore:
            return
        entry.score=vl-currChessGame.distanceToRoot
    else:
        entry.score=vl
    entry.bestMove=method
    entry.permutationTableCheckCode=currChessGame.checkCode.duplicateSituationCheckCode
    SearchRelatedVariables.permutationTable[currChessGame.checkCode.duplicateSituationCheckCode&(maximumPermutationTableSize-1)]=entry

def getMvvLva(method):
    return (MvvLvaForceValue[currChessGame.chessboard[getEnd(method)]]<<3)-MvvLvaForceValue[currChessGame.chessboard[getStart(method)]]

class RoutingSortStructure:
    def init(self,mvHash):
        self.permutationTableMethod=mvHash
        self.killer1=SearchRelatedVariables.killerMethodTable[currChessGame.distanceToRoot][0]
        self.killer2=SearchRelatedVariables.killerMethodTable[currChessGame.distanceToRoot][1]
        self.stage=0

    def getNextMove(self):
        if self.stage==0:
            self.stage=1
            if self.permutationTableMethod!=0:
                return self.permutationTableMethod
        if self.stage==1:
            self.stage=2
            if self.killer1!=self.permutationTableMethod and self.killer1!=0 and currChessGame.moveAllow(self.killer1):
                return self.killer1
        if self.stage==2:
            self.stage=3
            if self.killer2!=self.permutationTableMethod and self.killer2!=0 and currChessGame.moveAllow(self.killer2):
                return self.killer2
        if self.stage==3:
            self.stage=4
            self.allRoutes=currChessGame.generateAllRoutes()
            self.allRoutes.sort(key=lambda x:SearchRelatedVariables.historyTable[x],reverse=True)
            self.adoptedWalkingMethod=0
        if self.stage==4:
            while self.adoptedWalkingMethod<len(self.allRoutes):
                method=self.allRoutes[self.adoptedWalkingMethod]
                self.adoptedWalkingMethod+=1
                if method!=self.permutationTableMethod and method!=self.killer1 and method!=self.killer2:
                    return method
        return 0

def dealBestWay(method,dep):
    SearchRelatedVariables.historyTable[method]+=dep**2
    lpmvKillers=SearchRelatedVariables.killerMethodTable[currChessGame.distanceToRoot]
    if lpmvKillers[0]!=method:
        lpmvKillers[1]=lpmvKillers[0]
        lpmvKillers[0]=method

def staticSearch(vlAlpha,vlBeta):
    vl=currChessGame.detectDuplicateSituation()
    if vl!=0:
        return currChessGame.duplicateSituationScore(vl)
    if currChessGame.distanceToRoot==maximumSearchDepth:
        return currChessGame.situationAssessment()
    maxn=-checkmateScore
    if currChessGame.isCheckmatedByHistoryTable():
        allRoutes=currChessGame.generateAllRoutes()
        allRoutes.sort(key=lambda x:SearchRelatedVariables.historyTable[x],reverse=True)
    else:
        vl=currChessGame.situationAssessment()
        if vl>maxn:
            maxn=vl
            if vl>=vlBeta:
                return vl
            if vl>vlAlpha:
                vlAlpha=vl
        allRoutes=currChessGame.generateAllRoutes(True)
        allRoutes.sort(key=lambda x:getMvvLva(x),reverse=True)

    for i in allRoutes:
        if currChessGame.makeAMove(i):
            vl=-staticSearch(-vlBeta,-vlAlpha)
            currChessGame.revokeMakeAMove()
            if vl>maxn:
                maxn=vl
                if vl>=vlBeta:
                    return vl
                if vl>vlAlpha:
                    vlAlpha=vl
    return currChessGame.distanceToRoot-checkmateScore if maxn==-checkmateScore else maxn

def beyondBoundaryAlphaBeta(vlAlpha,vlBeta,dep,notEmptyStepCut=False):
    permutationTableMethod=[0]
    routingSort=RoutingSortStructure()
    if dep<=0:
        return staticSearch(vlAlpha,vlBeta)
    vl=currChessGame.detectDuplicateSituation()
    if vl!=0:
        return currChessGame.duplicateSituationScore(vl)
    if currChessGame.distanceToRoot>=maximumSearchDepth:
        return currChessGame.situationAssessment()
    vl=extractPermutationTableEntry(vlAlpha,vlBeta,dep,permutationTableMethod)
    if vl>-checkmateScore:
        return vl
    if not notEmptyStepCut and not currChessGame.isCheckmatedByHistoryTable() and currChessGame.allowEmptyStepCut():
        currChessGame.emptyMove()
        vl=-beyondBoundaryAlphaBeta(-vlBeta,1-vlBeta,dep-emptyStepCutDepth-1,True)
        currChessGame.revokeEmptyMove()
        if vl>=vlBeta:
            return vl
    permutationTableEntrySign=AlphaPermutationTableEntry
    maxn=-checkmateScore
    bestMove=0
    routingSort.init(permutationTableMethod[0])
    method=routingSort.getNextMove()
    while method!=0:
        if currChessGame.makeAMove(method):
            newdep=dep if currChessGame.isCheckmated() else (dep-1)
            if maxn==-checkmateScore:
                vl=-beyondBoundaryAlphaBeta(-vlBeta,-vlAlpha,newdep)
            else:
                vl=-beyondBoundaryAlphaBeta(-vlAlpha-1,-vlAlpha,newdep)
                if vlAlpha<vl<vlBeta:
                    vl=-beyondBoundaryAlphaBeta(-vlBeta,vlAlpha,newdep)
            currChessGame.revokeMakeAMove()
            if vl>maxn:
                maxn=vl
                if vl>=vlBeta:
                    permutationTableEntrySign=BetaPermutationTableEntry
                    bestMove=method
                    break
                if vl>vlAlpha:
                    permutationTableEntrySign=PVPermutationTableEntry
                    bestMove=method
                    vlAlpha=vl
        method=routingSort.getNextMove()
    if maxn==-checkmateScore:
        return currChessGame.distanceToRoot-checkmateScore
    savePermutationTableEntry(permutationTableEntrySign,maxn,dep,bestMove)
    if bestMove!=0:
        dealBestWay(bestMove,dep)
    return maxn

def rootNodeAlphaBeta(dep):
    routingSort=RoutingSortStructure()
    maxn=-checkmateScore
    routingSort.init(SearchRelatedVariables.chessByComputer)
    method=routingSort.getNextMove()
    while method!=0:
        if currChessGame.makeAMove(method):
            newdep=dep if currChessGame.isCheckmatedByHistoryTable() else (dep-1)
            if maxn==-checkmateScore:
                vl=-beyondBoundaryAlphaBeta(-checkmateScore,checkmateScore,newdep,True)
            else:
                vl=-beyondBoundaryAlphaBeta(-maxn-1,-maxn,newdep)
                if vl>maxn:
                    vl=-beyondBoundaryAlphaBeta(-checkmateScore,-maxn,newdep,True)
            currChessGame.revokeMakeAMove()
            if vl>maxn:
                maxn=vl
                SearchRelatedVariables.chessByComputer=method
                if -scoreBoundaryVictoryAndDefeat<maxn<scoreBoundaryVictoryAndDefeat:
                    maxn+=(random.randint(0,32767)&randomSocre)-(random.randint(0,32767)&randomSocre)
        method=routingSort.getNextMove()
    savePermutationTableEntry(PVPermutationTableEntry,maxn,dep,SearchRelatedVariables.chessByComputer)
    dealBestWay(SearchRelatedVariables.chessByComputer,dep)
    return maxn

moveResult=0
def IDDFS(maxdep=maximumSearchDepth,maxtime=1):
    SearchRelatedVariables.historyTable=[0 for i in range(65536)]
    SearchRelatedVariables.killerMethodTable=[[0,0] for i in range(maximumSearchDepth)]
    SearchRelatedVariables.permutationTable=[PermutationTableStructure() for i in range(maximumPermutationTableSize)]
    currChessGame.distanceToRoot=0

    vl=0
    allRoutes=currChessGame.generateAllRoutes()
    for i in allRoutes:
        if currChessGame.makeAMove(i):
            currChessGame.revokeMakeAMove()
            SearchRelatedVariables.chessByComputer=i
            vl+=1
    if vl==1:
        return
    t=time.process_time()
    for i in range(1,maxdep+1):
        print(i,end=' ')
        vl=rootNodeAlphaBeta(i)
        if vl>scoreBoundaryVictoryAndDefeat or vl<-scoreBoundaryVictoryAndDefeat:
            break
        if maxtime>0 and time.process_time()-t>maxtime:
            break
    print("\n搜索深度:%d 耗时:%fs"%(i,time.process_time()-t))

computerRunning=False
def computerResponse():
    IDDFS()
    currChessGame.makeAMove(SearchRelatedVariables.chessByComputer)
    vl=currChessGame.detectDuplicateSituation(3)
    if currChessGame.isKilled():
        return -1#"你输了!"
    elif vl>0:
        vl=currChessGame.duplicateSituationScore(vl)
        return -2 if vl<-scoreBoundaryVictoryAndDefeat else 2
        #"长打作负,你输了!"            "电脑长打作负，你赢了!"
    elif currChessGame.historicalMovementsNumber>100:
        return 3#"超过自然限着作和"
    else:
        if currChessGame.eatPreviousStep():
            currChessGame.initHistoricalRoutingInformation()
    return 0


def playerMove(method: int):
    global computerRunning,moveResult
    if currChessGame.moveAllow(method):
        tmp=currChessGame.chessboard[getEnd(method)]
        if currChessGame.makeAMove(method):
            vl=currChessGame.detectDuplicateSituation(3)
            if currChessGame.isKilled():
                moveResult=1#"你赢了!"
                return
            elif vl>0:
                vl=currChessGame.duplicateSituationScore(vl)
                moveResult=-2 if vl>scoreBoundaryVictoryAndDefeat else 2
                #"长打作负,你输了!"                "电脑长打作负，你赢了!"
                return
            elif currChessGame.historicalMovementsNumber>100:
                moveResult=3#"超过自然限着作和"
                return
            else:
                if currChessGame.eatPreviousStep():
                    currChessGame.initHistoricalRoutingInformation()
                computerRunning=True
                tmp=computerResponse()
                computerRunning=False
                moveResult=tmp
                return
        else:
            moveResult=-3
            return
    moveResult=0
    return