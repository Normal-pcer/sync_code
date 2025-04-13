"""
Scoreboard

一款用于在局域网同步分数的记分板。
"""


from enum import Enum
import sys
from time import time
from typing import TypeVar, Dict, List, Optional, Set
from uuid import uuid4
import json
import socket
import threading

# 类型别名
T = TypeVar("T")
Score = float  # 使用浮点数存储积分


def set_default(value: Optional[T], default: T) -> T:
    if value is not None:
        return value
    else:
        return default


class DataBase:
    """
    用于存储本地的数据
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

    scores: Dict[str, Score]  # 用户名及其分数
    modifications: Dict[str, Set[Modification]]  # 用户名及其修改记录

    def __init__(self):
        self.scores = {}
        self.modifications = {}

    def to_dict(self) -> Dict[str, any]:
        return {
            "scores": self.scores,
            "modifications": self.modifications,
        }

    @classmethod
    def from_dict(cls, data: dict) -> "DataBase":
        db = cls()
        db.scores = data["scores"]
        return db

    def merge_with(self, other: "DataBase") -> None:
        """
        合并两个数据库，并就地修改当前对象
        """
        for username, score in other.scores.items():
            if username not in self.scores:
                self.scores[username] = score

        for username, mods in other.modifications.items():
            for mod in mods:
                if mod not in self.modifications[username]:
                    self.modifications[username].add(mod)
                    self.scores[username] += mod.amount


class Message:
    """
    用于节点之间的通信
    """
    class Type(Enum):
        First = "First"  # 节点首次连接，需要附带自己的地址和端口
        Second = "Second"  # 经过恰好一次转发的 First 消息
        ConnectWith = "ConnectWith"  # 请求新节点和自己连接  
        Modify = "Modify"  # 修改某个分数

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

    def __init__(self, host: str, port: int, username: str):
        self.host = host
        self.port = port
        self.username = username
        self.node_id = f"{host}:{port}"

        self.peers = {}
        self.data = DataBase()

        # 网络监听
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind((host, port))
        self.sock.listen(5)

        threading.Thread(target=self.accept_connections).start()
        threading.Thread(target=self.input_handler).start()

    def input_handler(self) -> None:
        while True:
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
                if name not in self.data.scores:
                    self.data.scores[name] = 0
                self.data.scores[name] += float(amount)
                self.send_message(Message(Message.Type.Modify, [name, amount]))
            elif cmd == "query":
                for name, score in self.data.scores.items():
                    print(f"{name}: {score}")
    def accept_connections(self) -> None:
        """
        接受来自其他节点的连接。
        """
        while True:
            client, _ = self.sock.accept()
            threading.Thread(target=self.handle_message_from, args=(client,)).start()

    def send_message(self, message: Message, targets: Optional[List[socket.socket]] = None) -> None:
        """
        向其他节点发送消息。
        如果没有填写目标节点，则发送给所有节点。
        """
        targets = set_default(targets, self.peers.values())
        for target in targets:
            target.send(str(message).encode())
    
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
                        self.send_message(Message(Message.Type.Second, [new_id]))

                        # 自己与其建立连接
                        host, port = new_id.split(":")
                        sock = socket.create_connection((host, int(port)))
                        self.peers[new_id] = sock
                    
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
                        self.send_message(Message(Message.Type.ConnectWith, [self.node_id]), [sock])
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
                    name, amount = message.data
                    if name not in self.data.scores:
                        self.data.scores[name] = 0
                    self.data.scores[name] += float(amount)
                case _:
                    pass
        try:
            while True:
                data = source.recv(1024).decode()
                if not data:
                    break

                print(f"data: {data}")
                message = Message.from_str(data)
                response = handle_single_message(message)
                if response is not None:
                    source.send(response.encode())
        finally:
            source.close()

if __name__ == "__main__":
    try:
        if len(sys.argv) != 3:
            ip, port = input("ip, port: ").split()
        else:
            ip, port = sys.argv[1:]

        SyncNode(ip, int(port), "user")
    except EOFError:
        exit()
