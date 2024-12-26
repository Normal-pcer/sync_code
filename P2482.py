from copy import copy
from enum import Enum
from collections import deque
from typing import Union


class Character(Enum):
    _Undefined = '\0'
    _Questionable = '?'
    F_Thief = 'F'
    M_Master = 'M'
    Z_Minister = 'Z'


def reversedCharacterType(original: Character):
    if original == Character.M_Master:
        return Character.M_Master
    elif original == Character.Z_Minister:
        return Character.F_Thief
    elif original == Character.F_Thief:
        return Character.Z_Minister
    else:
        return Character._Undefined


class DamageType(Enum):
    Undefined = 0
    DuelingFailed = 1
    Invading = 2
    Shooting = 3
    Dueling = 4
    Killing = 5


class Label(Enum):
    P_Peach = 'P'
    K_Killing = 'K'
    D_Dodge = 'D'
    Z_Crossbow = 'Z'
    F_Dueling = 'F'
    N_Invasion = 'N'
    W_Arrows = 'W'
    J_Unbreakable = 'J'


class Damage:
    source: "Player"
    amount: int
    type: DamageType

    def __init__(self, source: "Player", amount: int, type: DamageType):
        self.source = source
        self.amount = amount
        self.type = type


maxCardId = 0

cardAction = dict()


class Card:
    owner: Union["Player", None]
    id: int
    label: Label
    used: bool = False

    def __init__(self, label: Label, owner: Union["Player", None] = None, id: int | None = None):
        self.owner = owner
        self.label = label
        if id is None:
            global maxCardId
            maxCardId += 1
            self.id = maxCardId
        else:
            self.id = id

    def __eq__(self, other: Union["Card", str]):
        if isinstance(other, Card):
            return self.id == other.id
        elif isinstance(other, str):
            return self.label == Label(other)
        else:
            raise TypeError("Invalid type for operator==(Card, {})".format(type(other)))

    def copy(self, new_owner: Union["Player", None] = None):
        return Card(self.label, new_owner)

    def abandon(self):
        # print(self.owner.position.index, "abandon", self.label.value)
        if self in self.owner.cards:
            self.owner.cards.remove(self)

    def apply(self):
        if self.label in cardAction:
            res = cardAction[self.label](self)
            # print(self.owner.position.index, "apply", self.label.value, int(res))
            if res:
                self.abandon()
            return res
        else:
            return False


cardQueue = deque[Card]()


def lootCard():
    card = cardQueue.popleft()
    if len(cardQueue) == 0:
        cardQueue.append(card)
    return card


players = list["Player"]()


class RollingIndex:
    index: int

    def __init__(self, index=0):
        self.index = index

    def _next(self):
        assert self.index < len(players)
        self.index += 1
        if self.index >= len(players):
            self.index = 0

    def next(self):
        res = RollingIndex(self.index)
        res._next()
        while players[res.index].isDead():
            res._next()
        return res

    def __eq__(self, other: "RollingIndex"):
        return self.index == other.index

    def __ne__(self, other: "RollingIndex"):
        return self.index != other.index

    def __iadd__(self, other: int):
        for _ in range(other):
            self = self.next()
        return self

    def get(self):
        return players[self.index]


MAX_STRENGTH = 4
ThiefCount = 0
winner = Character._Undefined


class Player:
    character: Character
    impression: Character = Character._Undefined
    strength: int = MAX_STRENGTH
    cards: list[Card]
    position: RollingIndex
    dead: bool = False
    withWeapon: bool = False

    def __init__(self, character: Character):
        self.character = character
        self.cards = list[Card]()
        self.position = RollingIndex()

    def isDead(self) -> bool:
        return self.dead

    def findCard(self, label: Label) -> Card | None:
        for card in self.cards:
            if card.label == label:
                return card
        return None

    def getCard(self, template: Card):
        # print(self.position.index, "gets", template.label.value)
        copy = template.copy(self)
        self.cards.append(copy)

    def characterTypeAfterSquiring(self) -> Character:
        if self.character == Character.F_Thief:
            return Character.F_Thief
        elif self.character in [Character.Z_Minister, Character.M_Master]:
            return Character.Z_Minister
        else:
            return Character._Undefined

    def damaged(self, obj: Damage):
        # print(f"{self.strength} {self.position.index} <- {obj.source.position.index}")
        global winner
        assert not self.isDead()
        if obj.type == DamageType.Killing:
            dodge = self.findCard(Label.D_Dodge)
            if dodge is not None:
                dodge.abandon()
            else:
                self.strength -= obj.amount
        elif obj.type == DamageType.Invading:
            killing = self.findCard(Label.K_Killing)
            if killing is not None:
                killing.abandon()
            else:
                self.strength -= obj.amount
        elif obj.type == DamageType.Shooting:
            dodge = self.findCard(Label.D_Dodge)
            if dodge is not None:
                dodge.abandon()
            else:
                self.strength -= obj.amount
        else:
            self.strength -= obj.amount

        while self.strength <= 0:
            peach = self.findCard(Label.P_Peach)
            if peach is None:
                break
            else:
                peach.apply()

        if self.strength <= 0:
            self.dead = True

        if self.character == Character.M_Master:
            if self.isDead():
                winner = Character.F_Thief

            if obj.source.impression == Character._Undefined and obj.type.value > DamageType.DuelingFailed.value:
                obj.source.impression = Character._Questionable

        hostility = (obj.type.value >= DamageType.Dueling.value)

        if hostility:
            obj.source.impression = Character(max(
                obj.source.impression.value, reversedCharacterType(
                    self.characterTypeAfterSquiring()).value
            ))

        if self.character == Character.F_Thief and self.isDead():
            global ThiefCount
            ThiefCount -= 1
            if ThiefCount <= 0:
                winner = Character.M_Master

        # print(self.strength)
        if self.isDead():
            if self.character == Character.F_Thief:
                if winner != Character._Undefined:
                    return
                for _ in range(3):
                    obj.source.getCard(lootCard())
            elif self.character == Character.Z_Minister and obj.source.character == Character.M_Master:
                obj.source.cards.clear()
                obj.source.withWeapon = False

    def inDueling(self, enemy: "Player"):
        if enemy.character == Character.M_Master and self.character == Character.Z_Minister:
            self.damaged(Damage(enemy, 1, DamageType.DuelingFailed))
            return
        toBeAbandoned = self.findCard(Label.K_Killing)
        if toBeAbandoned is not None:
            toBeAbandoned.abandon()
            enemy.inDueling(self)
        else:
            self.damaged(Damage(enemy, 1, DamageType.DuelingFailed))

    def team(self):
        if self.character == Character.F_Thief:
            return Character.F_Thief
        elif self.character in [Character.Z_Minister, Character.M_Master]:
            return Character.Z_Minister
        else:
            return Character._Undefined

    def showCharacter(self):
        if self.character != Character.M_Master:
            self.impression = self.character

    def drawCards(self):
        for _ in range(2):
            self.getCard(lootCard())

    def index(self):
        if self in players:
            return players.index(self)
        else:
            return -1

    def round(self) -> bool:
        self.drawCards()
        usedKillingCard = 0
        i = 0
        executed = False
        while i < len(self.cards) or executed:
            executed = False
            i = 0
            while i < len(self.cards):
                card = self.cards[i]
                # print(f"Player {card.owner.position.index} Card {i}")
                isKillingCard = (card.label == Label.K_Killing)
                if isKillingCard and usedKillingCard >= 1 and not self.withWeapon:
                    i += 1
                    continue
                if card.apply():
                    if isKillingCard:
                        usedKillingCard += 1
                    executed = True
                    break
                i += 1

            if winner != Character._Undefined:
                return False
            if self.isDead():
                return True
        return True


def tryExecutingUnbreakable(before: Card, original: Player, pos: RollingIndex, tag: bool, depth: int):
    # print(f"try {before.label} {original.position.index}")
    if original.impression.value < Character.F_Thief.value and original.character != Character.M_Master:
        return False
    friendly = False
    if before.label in [Label.F_Dueling, Label.N_Invasion, Label.W_Arrows]:
        friendly = True
    else:
        friendly = not tag

    p = copy(pos)

    def do():
        pl = p.get()
        card = pl.findCard(Label.J_Unbreakable)
        if card is None:
            return
        if card.used:
            return

        card.used = True
        if friendly:
            char = original.team()
            if (char == pl.team()):
                card.abandon()
                pl.showCharacter()
                res = not tryExecutingUnbreakable(
                    card, original, p, friendly, depth + 1)
                return res
        else:
            char = reversedCharacterType(original.team())
            if (char == pl.team()):
                card.abandon()
                pl.showCharacter()
                res = not tryExecutingUnbreakable(
                    card, original, p, friendly, depth + 1)
                return res

        card.used = False

    res = do()
    if res is not None:
        return res
    p += 1
    while p != pos:
        res = do()
        if res is not None:
            return res
        p += 1
    return False


def joinPlayer(member: Player):
    global players
    players.append(member)
    added = players[-1]
    added.position = RollingIndex(len(players) - 1)
    return added


def P_Peach(card: Card):
    if card.owner.strength >= MAX_STRENGTH:
        return False
    else:
        card.owner.strength += 1
        return True


def K_Killing(card: Card):
    target = card.owner.position.next().get()
    imp = target.impression
    user = card.owner.character
    execute = False

    if user == Character.M_Master:
        if imp == Character._Questionable or imp == Character.F_Thief:
            execute = True
    elif user == Character.Z_Minister:
        if imp == Character.F_Thief:
            execute = True
    else:
        if imp == Character.Z_Minister or target.character == Character.M_Master:
            execute = True

    if execute:
        target.damaged(Damage(card.owner, 1, DamageType.Killing))
        return True
    else:
        return False


def D_Dodge(_: Card):
    return False


def Z_Crossbow(card: Card):
    card.owner.withWeapon = True
    return True


def F_Dueling(card: Card):
    user = card.owner.character
    target = RollingIndex(card.owner.position.index)

    if user == Character.M_Master:
        target += 1
        while target.get().impression != Character.F_Thief and target.get().impression != Character._Questionable and target != card.owner.position:
            target += 1
    elif user == Character.Z_Minister:
        target += 1
        while target.get().impression != Character.F_Thief and target != card.owner.position:
            target += 1
    else:
        target += 1
        while target.get().character != Character.M_Master and target != card.owner.position:
            target += 1

    if target == card.owner.position:
        return False
    cur = target.get()
    cur.damaged(Damage(card.owner, 0, DamageType.Dueling))
    if not tryExecutingUnbreakable(card, cur, card.owner.position.next(), False, 0):
        cur.inDueling(card.owner)
    return True


def N_Invasion(card: Card):
    target = card.owner.position.next()
    while target != card.owner.position:
        canceled = tryExecutingUnbreakable(
            card, target.get(), card.owner.position, False, 0)
        if canceled:
            target += 1
            continue
        target.get().damaged(Damage(card.owner, 1, DamageType.Invading))
        if winner != Character._Undefined:
            return True
        target += 1
    return True


def W_Arrows(card: Card):
    target = card.owner.position.next()
    while target != card.owner.position:
        canceled = tryExecutingUnbreakable(
            card, target.get(), card.owner.position, False, 0)
        if canceled:
            target += 1
            continue
        target.get().damaged(Damage(card.owner, 1, DamageType.Shooting))
        if winner != Character._Undefined:
            return True
        target += 1
    return True


def J_Unbreakable(_: Card):
    return False


cardAction = {
    Label.P_Peach: P_Peach,
    Label.K_Killing: K_Killing,
    Label.D_Dodge: D_Dodge,
    Label.Z_Crossbow: Z_Crossbow,
    Label.F_Dueling: F_Dueling,
    Label.N_Invasion: N_Invasion,
    Label.W_Arrows: W_Arrows,
    Label.J_Unbreakable: J_Unbreakable
}


def round() -> bool:
    for player in players:
        # print("-----")
        # for pl in players:
        #     print(pl.impression.value, pl.strength, end=' ')
        #     if pl.isDead():
        #         print("DEAD")
        #     else:
        #         for cd in pl.cards:
        #             print(cd.label.value, end=' ')
        #         print()
        if player.isDead():
            continue
        if not player.round():
            return False
    return True


if __name__ == '__main__':
    N, M = map(int, input().split())
    for _ in range(N):
        inp = input().split()
        typeChar = Character(inp[0][0])
        players.append(Player(typeChar))
        pl = players[-1]

        pl.position = RollingIndex(len(players) - 1)
        if pl.character == Character.F_Thief:
            ThiefCount += 1

        for i in range(1, 5):
            pl.getCard(Card(Label(inp[i])))

    lis = input().split()
    for i in range(M):
        cardQueue.append(Card(Label(lis[i])))

    while round():
        pass

    print(winner.value + 'P')
    for player in players:
        if player.isDead():
            print("DEAD")
        else:
            for cd in player.cards:
                print(cd.label.value, end=' ')
            print()
