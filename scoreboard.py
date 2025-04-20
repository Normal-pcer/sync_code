"""
Scoreboard

一款用于在局域网同步分数的记分板。
"""


from enum import Enum
from time import time
from typing import TypeVar, Dict, List, Optional, Set
from uuid import uuid4
import sys
import json
import socket
import struct
import threading

# 类型别名
T = TypeVar("T")
Score = float  # 使用浮点数存储积分


def set_default(value: Optional[T], default: T) -> T:
    if value is not None:
        return value
    else:
        return default

class InvalidOperationException(Exception):
    """
    用于指示进行了无效操作
    """
    description: str

    def __init__(self, description: Optional[str] = None):
        self.description = set_default(description, "")
    
        super().__init__(self.description)
    
    def __str__(self) -> str:
        return f"InvalidOperationException: {self.description}"

class DataBase:
    """
    用于存储本地的数据

    保证存储在 scores 中的分数为最新值，修改记录仅为辅助。
    """
    class Modification:
        """
        更改一次分数
        """
        amount: Score  # 改变的数值
        timestamp: float  # 时间戳
        modify_id: str  # 修改的 UUID

        def __init__(self, amount: Score, timestamp: Optional[float] = None, modify_id: Optional[str] = None):
            timestamp = set_default(timestamp, time())
            modify_id = set_default(modify_id, str(uuid4()))
            self.amount = amount
            self.timestamp = timestamp
            self.modify_id = modify_id

        def __eq__(self, other: "DataBase.Modification") -> bool:
            return self.modify_id == other.modify_id

        def to_dict(self) -> Dict[str, float | str]:
            return {
                "amount": self.amount,
                "timestamp": self.timestamp,
                "modify_id": self.modify_id,
            }

        @classmethod
        def from_dict(cls, data: dict) -> "DataBase.Modification":
            return cls(
                data["amount"],
                data["timestamp"],
                data["modify_id"],
            )

        def __hash__(self) -> int:
            return hash(self.modify_id)
        
        def __str__(self) -> str:
            return f"Modification({self.amount}, {self.timestamp}, \"{self.modify_id}\")"

    scores: Dict[str, Score]  # 用户名及其分数
    modifications: Dict[str, List[Modification]]  # 用户名及其修改记录
    name: str

    def __init__(self, name: Optional[str] = None):
        name = set_default(name, "anonymous")
        self.scores = {}
        self.modifications = {}
        self.name = name

    def to_dict(self) -> Dict[str, any]:
        return {
            "scores": self.scores,
            "modifications": {key: [
                val.to_dict() for val in val_set
            ] for key, val_set in self.modifications.items()},
        }

    @classmethod
    def from_dict(cls, data: dict) -> "DataBase":
        db = cls()
        db.scores = data["scores"]
        db.modifications = {
            key: {cls.Modification.from_dict(val) for val in val_list}
            for key, val_list in data["modifications"].items()
        }
        print(f"from_dict() -> {db.scores}, {db.modifications}")
        return db

    def merge_with(self, other: "DataBase") -> None:
        """
        合并两个数据库，并就地修改当前对象
        """
        for name in other.scores.keys() | other.modifications.keys():
            if name not in self.scores:
                self.scores[name] = 0
            if name not in self.modifications:
                self.modifications[name] = set()

        # 检查是否有没有做过的修改
        for username, mods in other.modifications.items():
            for mod in mods:
                if mod not in self.modifications[username]:
                    self.modifications[username].add(mod)
                    self.scores[username] += mod.amount
        
        self.save()

    def __eq__(self, other: "DataBase") -> bool:
        return self.to_dict() == other.to_dict()

    def __ne__(self, other: "DataBase") -> bool:
        return not self.__eq__(other)

    def save(self) -> None:
        with open(f"{self.name}.scoreboard.json", "w") as f:
            s = json.dumps(self.to_dict())
            f.write(s)

    def load(self) -> None:
        try:
            data = json.load(open(f"{self.name}.scoreboard.json", "r"))
            data = DataBase.from_dict(data)
            self.scores = data.scores
            self.modifications = data.modifications
            print(f"成功加载数据: {self.scores}, {self.modifications}")
        except FileNotFoundError:
            pass
        except Exception as e:
            print(f"无法加载数据: {repr(e)}")

    def modify(self, name: str, amount: Score) -> None:
        """
        修改分数（测试用）
        """
        if name not in self.scores:
            self.scores[name] = 0
        self.scores[name] += amount

        if name not in self.modifications:
            self.modifications[name] = set()
        self.modifications[name].add(self.Modification(amount))
        self.save()

    def reduce_score(self, name: str, amount: Score) -> None:
        """
        给一个用户扣分

        需要保证这个用户存在，否则抛出异常。
        amount 为基准分数。实际的扣分为：amount ** (1 + cnt / 10)。
        其中 cnt 为最近 72 小时内扣分次数与 10 的较小值。
        """

        print(f"reduce_score({name}, {amount})")

        if name not in self.scores:
            raise InvalidOperationException(f"用户 {name} 不存在")

        # 计算扣分
        current = time()
        cnt = 0
        for mod in self.modifications[name]:
            if current - mod.timestamp < 72 * 3600 and mod.amount < 0:
                cnt += 1

        amount = amount ** (1 + min(cnt, 10) / 10)
        self.scores[name] -= amount
        assert name in self.modifications
        self.modifications[name].add(self.Modification(-amount))
        self.save()

    def show(self) -> None:
        """
        显示当前的分数
        """
        print("分数表：")
        for name, score in sorted(self.scores.items(), key=lambda x: x[1], reverse=True):
            print(f"{name}: {score}")

class Message:
    """
    用于节点之间的通信
    """
    class Type(Enum):
        First = "First"  # 节点首次连接，需要附带自己的地址和端口
        Second = "Second"  # 经过恰好一次转发的 First 消息
        ConnectWith = "ConnectWith"  # 请求新节点和自己连接
        SyncData = "SyncData"  # 同步所有数据，用于建立第一次连接时
        UpdateData = "UpdateData"  # 更新数据，用于一个新的节点进入时合并信息
        Modify = "Modify"  # 修改某个分数
        ReduceScore = "ReduceScore"  # 扣分

        def __str__(self) -> str:
            return self.name

    message_type: Type
    data: any

    def __init__(self, message_type: Type, data: Optional[List[str]] = None):
        data = set_default(data, [])
        self.message_type = message_type
        self.data = data

    def __str__(self) -> str:
        assert not (' ' in str(self.message_type))
        return str(self.message_type) + " " + json.dumps(self.data)

    @classmethod
    def from_str(cls, message: str) -> "Message":
        pos = message.find(" ")
        if pos == -1:
            pos = len(message)
        return cls(Message.Type(message[:pos]), json.loads(message[pos+1:]))


class MessageProcesser:
    """
    用于传输消息。

    为了避免过长的消息被以外截断，编码为如下格式：
    <长度> <数据>
    长度为 32 位整数，数据为 UTF-8 编码的字符串。
    """

    @staticmethod
    def send(sock: socket.socket, message: Message) -> None:
        data = str(message).encode()
        length = struct.pack("!I", len(data))
        sock.sendall(length + data)

    @staticmethod
    def recv(sock: socket.socket) -> Message:
        """
        阻塞式接收数据，直到接收到完整的消息。
        """
        length_data = sock.recv(4)
        if not length_data:
            raise ConnectionError("连接已关闭")

        length = struct.unpack("!I", length_data)[0]
        data = b""
        while len(data) < length:
            chunk = sock.recv(length - len(data))
            if not chunk:
                raise ConnectionError("连接已关闭")
            data += chunk

        return Message.from_str(data.decode())


class SyncNode:
    """
    用于和其他节点联通，并进行数据同步操作。
    """
    host: str  # 节点的 IP 地址
    port: int  # 节点的端口号
    username: str  # 节点的用户名
    node_id: str  # 用于描述节点的 id，格式为 host:port

    peers: Dict[str, socket.socket]  # 已经建立连接的其他节点
    data: DataBase  # 节点存储的数据

    sock: socket.socket

    def __init__(self, host: str, port: int, username: Optional[str] = None):
        username = set_default(username, f"{host}_{port}")

        self.host = host
        self.port = port
        self.username = username
        self.node_id = f"{host}:{port}"

        self.peers = {}
        self.data = DataBase(username)
        self.data.load()

        # 网络监听
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind((host, port))
        self.sock.listen(5)

        threading.Thread(target=self.accept_connections).start()
        threading.Thread(target=self.input_handler).start()

    def input_handler(self) -> None:
        while True:
            try:
                cmd = input()

                if cmd == "quit":
                    self.sock.close()
                    sys.exit(0)
                elif cmd == "list":
                    print(self.get_known_peers())
                elif cmd.startswith("connect"):
                    host, port = cmd[8:].split()
                    self.connect(host, int(port))
                elif cmd.startswith("modify"):
                    name, amount = cmd[7:].split()
                    self.data.modify(name, float(amount))
                    self.send_message(Message(Message.Type.Modify, [name, amount]))
                elif cmd == "query":
                    print(json.dumps(self.data.to_dict(), indent=4))
                elif cmd.startswith("reduce"):
                    name, amount = cmd[7:].split()
                    self.data.reduce_score(name, float(amount))
                    self.send_message(Message(Message.Type.ReduceScore, [name, float(amount)]))
                elif cmd == "show":
                    self.data.show()
            except Exception as e:
                print(f"发生错误: {repr(e)}")

    def accept_connections(self) -> None:
        """
        接受来自其他节点的连接。
        """
        while True:
            client, _ = self.sock.accept()
            threading.Thread(target=self.handle_message_from,
                             args=(client,)).start()

    def send_message(self, message: Message, targets: Optional[List[socket.socket]] = None) -> None:
        """
        向其他节点发送消息。
        如果没有填写目标节点，则发送给所有节点。
        """
        targets = set_default(targets, self.peers.values())
        for target in targets:
            MessageProcesser.send(target, message)

    def get_known_peers(self) -> List[str]:
        """获取已知的节点列表"""
        return list(self.peers.keys())

    def connect(self, host: str, port: int) -> None:
        """与另一个节点建立连接。"""
        peer_id = f"{host}:{port}"
        if peer_id in self.peers or peer_id == self.node_id:
            return

        try:
            sock = socket.create_connection((host, port), timeout=3)
            self.peers[peer_id] = sock

            self.send_message(Message(Message.Type.First, [self.node_id]))
        except socket.error as e:
            print(f"无法连接到 {host}:{port}: {e}")

    def handle_message_from(self, source: socket.socket) -> None:
        """相应来自其他客户端的消息"""
        def handle_single_message(message: Message) -> None | str:
            match message.message_type:
                case Message.Type.First:
                    # 认识一个新的节点
                    new_id: str = message.data[0]
                    if new_id == self.node_id:
                        return
                    if new_id not in self.peers:
                        # 转发，告诉其他节点
                        self.send_message(
                            Message(Message.Type.Second, [new_id]))

                        # 自己与其建立连接
                        host, port = new_id.split(":")
                        sock = socket.create_connection((host, int(port)))
                        self.peers[new_id] = sock

                        # 分享现在的数据
                        data = self.data.to_dict()
                        self.send_message(
                            Message(Message.Type.SyncData, [json.dumps(data)]), [sock])

                    print("peers: ", self.peers)
                case Message.Type.Second:
                    # 新节点，经过一次转发
                    new_id: str = message.data[0]
                    if new_id == self.node_id:
                        return

                    # 正向联系
                    if new_id not in self.peers:
                        host, port = new_id.split(":")
                        sock = socket.create_connection((host, int(port)))
                        self.peers[new_id] = sock

                        # 请求反向联系
                        self.send_message(
                            Message(Message.Type.ConnectWith, [self.node_id]), [sock])
                    print("peers: ", self.peers)
                case Message.Type.ConnectWith:
                    # 和给定的节点连接
                    new_id: str = message.data[0]
                    if new_id not in self.peers:
                        host, port = new_id.split(":")
                        sock = socket.create_connection((host, int(port)))
                        self.peers[new_id] = sock
                    print("peers: ", self.peers)
                case Message.Type.Modify:
                    self.data.modify(message.data[0], float(message.data[1]))
                case Message.Type.SyncData:
                    # 同步数据
                    data = json.loads(message.data[0])
                    db = DataBase.from_dict(data)
                    self.data.merge_with(db)

                    if self.data != db:
                        # 如果数据不同，则更新
                        self.send_message(
                            Message(Message.Type.UpdateData, [json.dumps(self.data.to_dict())]))
                    self.data.save()
                case Message.Type.UpdateData:
                    # 当前会无条件信任对方的数据
                    data = json.loads(message.data[0])
                    db = DataBase.from_dict(data)
                    self.data = db
                    self.data.save()
                case Message.Type.ReduceScore:
                    # 扣分
                    name = message.data[0]
                    amount = float(message.data[1])
                    self.data.reduce_score(name, amount)
                case _:
                    print(f"未知消息类型: {message.message_type}")
        try:
            while True:
                try:
                    message = MessageProcesser.recv(source)
                except ConnectionError:
                    print("连接已关闭")
                    break

                print(f"data: {message}")
                response = handle_single_message(message)
                if response is not None:
                    source.send(response.encode())
        finally:
            source.close()


if __name__ == "__main__":
    node = None
    try:
        if len(sys.argv) != 3:
            ip, port = input("ip, port: ").split()
        else:
            ip, port = sys.argv[1:]

        node = SyncNode(ip, int(port))
    except EOFError:
        del node
        exit()
