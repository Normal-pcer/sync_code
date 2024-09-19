from enum import Enum
from abc import abstractmethod
from typing import List, Set, Dict
from sys import exit

PLAYER_POWER_LIMIT = 4
playersCount: int
cardsCount: int

cardsPool: List["Card"]

class PlayerType(Enum):
    Master = "M"
    Minister = "Z"
    Enemy = "F"
    Unknown = "UK"
    Questionable = "QA"
typeMap = {
    "M": PlayerType.Master, "Z": PlayerType.Minister, "F": PlayerType.Enemy, "UK": PlayerType.Unknown, "QA": PlayerType.Questionable
}

class PlayerStatus(Enum):
    Nothing = 0
    ToBeKilled = 1

class Player:
    identifier: int             # 编号
    power: int                  # 体力值
    status: PlayerStatus        # 状态
    characterType: PlayerType   # 类型
    impression: PlayerType      # 主猪对自己的认知
    cards: List["Card"]         # 持有的卡牌
    weapon: bool                # 是否持有武器；由于本题的武器只有“猪哥连弩”，简化为布尔值

    def __init__(self, identifier: int, character_type: str):
        self.identifier = identifier
        self.power = 4
        self.status = PlayerStatus.Nothing
        self.characterType = typeMap[character_type]
        self.impression = PlayerType.Unknown
        self.cards = list()

    def __eq__(self, other: "Player"):
        return self.identifier == other.identifier

    def __sub__(self, other: "Player"):
        return ((other.identifier - self.identifier) + len(players)) % len(players)

    def getCard(self, card: "Card"):
        card.owner = self
        self.cards.append(card)

    def use(self, card: "Card"):
        card.apply()
        self.abandon(card)

    def abandon(self, card: "Card"):
        self.cards.remove(card)

    def inKilling(self):       # 受到“杀”的攻击时触发
        for card in self.cards:
            if card.name == "D":
                self.use(card)
                return

    def inDueling(self, another: "Player"):         # 被卷入“决斗”时触发
        if self.characterType == PlayerType.Minister and another.characterType == PlayerType.Master:  # 忠猪不打主猪
            return
        for card in self.cards:
            if card.name == "K":  # 如果手牌中有“杀”一定使用
                self.abandon(card)
                another.inDueling(self)  # 轮到对方参与决斗
                return
        Damage(1, another, self).apply() # 决斗失败，受到1点伤害
    
    def inInvasion(self, another: "Player"):        # 面对“南蛮入侵”时触发
        for card in self.cards:
            if card.name == "K":        # 一定弃置一张“杀”
                self.abandon(card)
                return
        Damage(1, another, self, False).apply() # 没有卡牌弃置，受到一点伤害

    def inFatalArrows(self, another: "Player"):        # 面临“万箭齐发”时触发
        for card in self.cards:
            if card.name == "D":        # 一定弃置一张“闪”
                self.abandon(card)
                return
        Damage(1, another, self, False).apply() # 没有卡牌弃置，受到一点伤害
    
    def next(self):  # 获取逆时针方向上的下一个玩家
        if self.identifier >= len(players)-1:
            return players[self.identifier - len(players) + 1]
        return players[self.identifier+1]


class Card:
    owner: Player               # 持有者
    limited: bool               # 限制距离

    def __init__(self, owner: Player):
        self.owner = owner
    
    def usable() -> bool:  return True

    @property
    @abstractmethod
    @staticmethod
    def name() -> str:  pass

    @abstractmethod
    def apply():  pass

class Damage:
    amount: int
    source: Player
    target: Player
    force: bool   # 是否为表敌意

    def __init__(self, amount: int, source: Player, target: Player, force=True):
        self.amount = amount
        self.source = source
        self.target = target
        self.force = force
    
    def apply(self):
        self.target.power -= self.amount
        if (self.target.characterType == PlayerType.Master or self.target.impression == PlayerType.Minister) and self.force:  # 跳反
            self.source.impression = PlayerType.Enemy
        elif self.target.characterType == PlayerType.Master:  # 认定为类反猪
            self.source.impression = PlayerType.Questionable

players: List[Player] = list()
enemies: Set[Player]
master: Player
cardsQueue: List[Card] = list()
NonePlayer = Player(-1, PlayerType.Unknown.value)

def removePlayer(player: "Player"):
    players.pop(player.identifier)
    for index in range(1, playersCount):
        players[index].identifier = index
    playersCount -= 1
    if player.characterType == PlayerType.Enemy:
        enemies.discard(player)
        if len(enemies) == 0:
            stop(PlayerType.Master)
    elif player.characterType == PlayerType.Master:
        stop(PlayerType.Enemy)

def stop(victor: PlayerType):
    exit(0)

class TestCard(Card):
    @property
    @staticmethod
    def name() -> str:
        return "T"

    def apply(self):
        print("test")

class P_PeachCard(Card):
    """ "桃" """
    @property
    @staticmethod
    def name():  return "P"

    def usable(self) -> bool:
        return self.owner.power != PLAYER_POWER_LIMIT

    def apply(self):
        self.owner.power += 1 

class K_KillingCard(Card):
    """ "杀" """
    @property
    @staticmethod
    def name():  return "K"

    def apply(self, attackTarget: Player):
        Damage(1, self.owner, attackTarget).apply()
        attackTarget.inKilling()

class D_EvadeCard(Card):
    """ "闪" """
    @property
    @staticmethod
    def name():  return "D"

    def apply(self):
        self.owner.power += 1    # 抵消"杀"的伤害

class F_DuelingCard(Card):
    """ "决斗" """
    @property
    @staticmethod
    def name():  return "F"

    def apply(self, duelingTarget: Player):
        duelingTarget.inDueling(self)

class N_InvasionCard(Card):
    """ "南猪入侵" """
    @property
    @staticmethod
    def name():  return "N"

    def apply(self):
        currentTarget = self.owner.next()
        while currentTarget != self:  # 逆时针依次入侵目标
            currentTarget.inInvasion()
            currentTarget = currentTarget.next()

class W_FatalArrowsCard(Card):
    """ "万箭齐发" """
    @property
    @staticmethod
    def name():  return "W"

    def apply(self):
        currentTarget = self.owner.next()
        while currentTarget != self:  # 逆时针依次攻击目标
            currentTarget.inInvasion()
            currentTarget = currentTarget.next()

class J_UnbreakableCard(Card):
    """ "无懈可击" """
    @property
    @staticmethod
    def name():  return "J"

    def apply(self):  pass

class Z_CrossbowCard(Card):
    """ "猪哥连弩" """
    @property
    @staticmethod
    def name():  return "Z"

    def apply(self):
        self.owner.weapon = True

def generateCard(nameChar: str, owner: Player) -> Card:
    CARD_MAP = {
        "P": P_PeachCard,
        "K": K_KillingCard,
        "D": D_EvadeCard,
        "F": F_DuelingCard,
        "N": N_InvasionCard,
        "W": W_FatalArrowsCard,
        "J": J_UnbreakableCard,
        "Z": Z_CrossbowCard
    }
    return CARD_MAP[nameChar](owner)


if __name__ == "__main__":
    playersCount, cardsCount = map(int, input().split())
    for playerId in range(playersCount):
        line = input().split()
        player = Player(playerId, line[0][0])
        if player.characterType == PlayerType.Master:  master = player
        for cardChar in line[1:]:
            card = generateCard(cardChar, player)
            player.getCard(card)
    cardsQueue += [generateCard(c, NonePlayer) for c in input().split()]

    # 游戏开始
    currentPlayer = master
    while True:
        # 摸牌阶段
        currentPlayer.getCard(cardsQueue.pop(1))
        currentPlayer.getCard(cardsQueue.pop(1))

        cardsToUse: List[Card] = list()
        attackedWays: Dict[str, bool] = {"K": True, "F": True}
        # 出牌阶段
        for card in currentPlayer.cards:
            if not card.usable:  continue
            if card.name == "P":  # 必吃桃
                cardsToUse.append(card)
                card.apply()
            elif card.name == "N" or card.name == "W":  # 万箭齐发和南猪入侵必然使用
                cardsToUse.append(card)
                card.apply()

            if currentPlayer.characterType == PlayerType.Master and card.name in attackedWays:  # 主猪表敌意
                target = currentPlayer.next()
                while target != currentPlayer:
                    if card.limited and target-currentPlayer>1:  break
                    if target.impression == PlayerType.Enemy or target.impression == PlayerType.Questionable and attackedWays[card.name]:
                        attackedWays[card.name] = False
                        cardsToUse.append(card)
                        card.apply()
            if currentPlayer.characterType == PlayerType.Minister and card.name in attackedWays:  # 忠猪表敌意
                target = currentPlayer.next()
                while target != currentPlayer:
                    if card.limited and target-currentPlayer>1:  break
                    if target.impression == PlayerType.Enemy and attackedWays[card.name]:
                        attackedWays[card.name] = False
                        cardsToUse.append(card)
                        card.apply()
            if currentPlayer.characterType == PlayerType.Enemy and card.name in attackedWays:  # 反猪表敌意
                target = currentPlayer.next()
                while target != currentPlayer:      # 对主猪
                    if card.limited and target-currentPlayer>1:  break
                    if target.impression == PlayerType.Master and attackedWays[card.name]:
                        attackedWays[card.name] = False
                        cardsToUse.append(card)
                        card.apply()
                target = currentPlayer.next()
                while target != currentPlayer:      # 对忠猪
                    if card.limited and target-currentPlayer>1:  break
                    if target.impression == PlayerType.Minister and attackedWays[card.name]:
                        attackedWays[card.name] = False
                        cardsToUse.append(card)
                        card.apply()

            map(currentPlayer.abandon, cardsToUse)
