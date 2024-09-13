from enum import Enum
from abc import abstractmethod
from typing import List

PLAYER_POWER_LIMIT = 4

class PlayerType(Enum):
    Master = "M"
    Minister = "Z"
    Enemy = "F"

class PlayerStatus(Enum):
    Nothing = 0
    ToBeKilled = 1

class Player:
    identifier: int             # 编号
    power: int                  # 体力值
    status: PlayerStatus        # 状态
    characterType: PlayerType   # 类型
    cards: List["Card"]         # 持有的卡牌

    def __init__(self, identifier: int, character_type: PlayerType):
        self.identifier = identifier
        self.power = 4
        self.status = PlayerStatus.Nothing
        self.characterType = character_type

class Card:
    owner: Player               # 持有者

    def __init__(self, owner: Player):
        self.owner = owner

    @property
    @abstractmethod
    def name() -> str:  pass

    @abstractmethod
    def apply():  pass

players: List[Player]
playersOperationsQueue: List[Player]  # 将要进行操作的玩家

class TestCard(Card):
    @property
    def name() -> str:
        return "T"

    def apply(self):
        print("test")

class P_PeachCard(Card):
    """ "桃" """
    @property
    def name():  return "P"

    def apply(self):
        owner = self.owner
        if owner.power != PLAYER_POWER_LIMIT:
            owner.power += 1 

class K_KillingCard(Card):
    """ "杀" """
    @property
    def name():  return "K"

    def apply(attackTarget: Player):
        attackTarget.power -= 1

def generateCard(nameChar: str, owner: Player) -> Card:
    CARD_MAP = {
        "P": P_PeachCard,
    }



if __name__ == "__main__":
    pass