"""
Scoreboard

一款用于在局域网同步分数的记分板。
"""


from enum import Enum
from time import time, strftime, localtime
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

def log(*message) -> None:
    """
    日志输出
    """
    print(f"[DEBUG {strftime('%H:%M:%S', localtime())}]", *message, file=sys.stderr)

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

from tkinter import *
from tkinter import ttk, messagebox, simpledialog
import sys

class ScoreboardGUI:
    def __init__(self, node):
        self.node = node
        self.root = Tk()
        self.root.title(f"Scoreboard - {node.username} ({node.host}:{node.port})")
        
        # 初始化UI组件
        self.create_widgets()
        self.setup_refresh()
        self.root.protocol("WM_DELETE_WINDOW", self.on_close)

    def create_widgets(self):
        # 主布局框架
        main_frame = ttk.Frame(self.root, padding=10)
        main_frame.grid(row=0, column=0, sticky=(N, S, E, W))

        # 分数列表
        self.tree = ttk.Treeview(main_frame, columns=('score',), selectmode='browse')
        self.tree.heading('#0', text='用户名')
        self.tree.heading('score', text='分数')
        self.tree.column('score', width=100, anchor='e')
        self.tree.grid(row=0, column=0, columnspan=3, padx=5, pady=5)

        # 操作按钮
        ttk.Button(main_frame, text='添加用户', command=self.add_user).grid(row=1, column=0, pady=5)
        ttk.Button(main_frame, text='修改分数', command=self.modify_score).grid(row=1, column=1, pady=5)
        ttk.Button(main_frame, text='扣分', command=self.reduce_score).grid(row=1, column=2, pady=5)
        ttk.Button(main_frame, text='赠送分数', command=self.give_score).grid(row=2, column=0, pady=5)
        
        # 网络连接部分
        conn_frame = ttk.LabelFrame(main_frame, text='网络连接', padding=10)
        conn_frame.grid(row=3, column=0, columnspan=3, sticky=(E, W))
        
        ttk.Label(conn_frame, text="IP:").grid(row=0, column=0)
        self.ip_entry = ttk.Entry(conn_frame, width=15)
        self.ip_entry.grid(row=0, column=1)
        
        ttk.Label(conn_frame, text="Port:").grid(row=0, column=2)
        self.port_entry = ttk.Entry(conn_frame, width=8)
        self.port_entry.grid(row=0, column=3)
        
        ttk.Button(conn_frame, text="连接", command=self.connect_node).grid(row=0, column=4, padx=5)
        
        # 已连接节点列表
        self.peer_list = Listbox(conn_frame, width=40, height=4)
        self.peer_list.grid(row=1, column=0, columnspan=5, pady=5)

    def setup_refresh(self):
        self.refresh_scores()
        self.refresh_peers()
        self.root.after(2000, self.setup_refresh)

    def refresh_scores(self):
        # 清空现有数据
        for item in self.tree.get_children():
            self.tree.delete(item)
        
        # 插入新数据
        for name, score in sorted(self.node.data.scores.items(), 
                                 key=lambda x: x[1], reverse=True):
            self.tree.insert('', 'end', text=name, values=(f"{score:.2f}",))

    def refresh_peers(self):
        self.peer_list.delete(0, END)
        for peer in self.node.get_known_peers():
            self.peer_list.insert(END, peer)

    def add_user(self):
        name = simpledialog.askstring("添加用户", "请输入用户名:")
        if name:
            try:
                self.node.data.join(name)
                self.node.send_message(Message(Message.Type.Modify, [name, "0"]))
            except InvalidOperationException as e:
                messagebox.showerror("错误", str(e))

    def modify_score(self):
        selected = self.tree.selection()
        if not selected:
            messagebox.showwarning("提示", "请先选择一个用户")
            return
        
        name = self.tree.item(selected[0], 'text')
        amount = simpledialog.askfloat("修改分数", f"请输入要修改的数值（当前分数：{self.node.data.scores[name]:.2f}）")
        if amount is not None:
            self.node.data.modify(name, amount)
            self.node.send_message(Message(Message.Type.Modify, [name, str(amount)]))

    def reduce_score(self):
        selected = self.tree.selection()
        if not selected:
            messagebox.showwarning("提示", "请先选择一个用户")
            return
        
        name = self.tree.item(selected[0], 'text')
        amount = simpledialog.askfloat("扣分", "请输入基准扣分数值:")
        if amount is not None:
            try:
                self.node.data.reduce_score(name, amount)
                self.node.send_message(Message(Message.Type.ReduceScore, [name, str(amount)]))
            except InvalidOperationException as e:
                messagebox.showerror("错误", str(e))

    def give_score(self):
        source = simpledialog.askstring("赠送分数", "请输入赠送方用户名:")
        target = simpledialog.askstring("赠送分数", "请输入接收方用户名:")
        amount = simpledialog.askfloat("赠送分数", "请输入赠送分数:")
        
        if source and target and amount:
            try:
                self.node.data.give_score(source, target, amount)
                self.node.send_message(Message(Message.Type.GiveScore, 
                                            [source, target, str(amount)]))
            except InvalidOperationException as e:
                messagebox.showerror("错误", str(e))

    def connect_node(self):
        ip = self.ip_entry.get()
        port = self.port_entry.get()
        if ip and port:
            try:
                self.node.connect(ip, int(port))
            except ValueError:
                messagebox.showerror("错误", "端口号必须为数字")
            except Exception as e:
                messagebox.showerror("连接错误", str(e))

    def on_close(self):
        self.node.sock.close()
        self.root.destroy()


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
        punish: bool = False  # 标识这次分数修改是惩罚性的，导致额外扣分

        def __init__(self, amount: Score, timestamp: Optional[float] = None, modify_id: Optional[str] = None, punish: bool = False):
            timestamp = set_default(timestamp, time())
            modify_id = set_default(modify_id, str(uuid4()))
            self.amount = amount
            self.timestamp = timestamp
            self.modify_id = modify_id
            self.punish = punish

        def __eq__(self, other: "DataBase.Modification") -> bool:
            return self.modify_id == other.modify_id

        def to_dict(self) -> Dict[str, float | str]:
            return {
                "amount": self.amount,
                "timestamp": self.timestamp,
                "modify_id": self.modify_id,
                "punish": self.punish,
            }

        @classmethod
        def from_dict(cls, data: dict) -> "DataBase.Modification":
            return cls(
                data["amount"],
                data["timestamp"],
                data["modify_id"],
                data["punish"],
            )

        def __hash__(self) -> int:
            return hash(self.modify_id)

        def __str__(self) -> str:
            return f"Modification({self.amount}, {self.timestamp}, \"{self.modify_id}\")"

    scores: Dict[str, Score]  # 用户名及其分数
    modifications: Dict[str, List[Modification]]  # 用户名及其修改记录
    free_amount: Dict[str, Score]  # 免费赠送分数的额度
    free_amount_prev_update: float  # 上次更新免费分数的时间
    FREE_AMOUNT_UPDATE_INTERVAL: int = 7 * 24 * 3600  # 一周更新一次
    DEFAULT_FREE_AMOUNT: Score = 10.0  # 每周的免费赠送额度
    name: str

    def __init__(self, name: Optional[str] = None):
        name = set_default(name, "anonymous")
        self.scores = {}
        self.modifications = {}
        self.free_amount = {}
        self.free_amount_prev_update = 0.0
        self.name = name

    def to_dict(self) -> Dict[str, any]:
        return {
            "scores": self.scores,
            "modifications": {key: [
                val.to_dict() for val in val_set
            ] for key, val_set in self.modifications.items()},
            "free_amount": self.free_amount,
            "free_amount_prev_update": self.free_amount_prev_update,
        }

    @classmethod
    def from_dict(cls, data: dict) -> "DataBase":
        db = cls()
        db.scores = data["scores"]
        db.modifications = {
            key: {cls.Modification.from_dict(val) for val in val_list}
            for key, val_list in data["modifications"].items()
        }
        db.free_amount = data["free_amount"]
        db.free_amount_prev_update = data["free_amount_prev_update"]
        log(f"from_dict() -> {db.scores}, {db.modifications}")
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

        log(f"reduce_score({name}, {amount})")

        if name not in self.scores:
            raise InvalidOperationException(f"用户 {name} 不存在")

        # 计算扣分
        current = time()
        cnt = 0
        for mod in self.modifications[name]:
            if current - mod.timestamp < 72 * 3600 and mod.punish:
                cnt += 1

        amount = amount ** (1 + min(cnt, 10) / 10)
        self.scores[name] -= amount
        assert name in self.modifications
        self.modifications[name].add(self.Modification(-amount, punish=True))
        self.save()

    def show(self) -> None:
        """
        显示当前的分数
        """
        print("分数表：")
        for name, score in sorted(self.scores.items(), key=lambda x: x[1], reverse=True):
            print(f"{name}: {score:.2f}")

    def show_detail(self, name: str) -> None:
        """
        显示一个人的分数详情
        """
        if name not in self.scores:
            print(f"{name} 不存在")
            return
        print(f"{name} 的分数详情：")
        print(f"分数: {self.scores[name]:.2f}")
        print("修改记录：")
        for mod in sorted(self.modifications[name], key=lambda x: x.timestamp):
            t = strftime("%Y-%m-%d %H:%M:%S", localtime(mod.timestamp))
            print(f"{t}: {mod.amount:.2f} ({mod.modify_id})")

        print(f"免费额度：{self.free_amount[name]:.2f}")

    def give_score(self, source: str, target: str, amount: Score) -> None:
        """
        给一个用户加分
        保证 target 的加分等于 amount。
        """
        # 尝试更新免费额度
        current = time()

        if (current // self.FREE_AMOUNT_UPDATE_INTERVAL != 
            self.free_amount_prev_update // self.FREE_AMOUNT_UPDATE_INTERVAL):
            for name in self.scores.keys():
                self.free_amount[name] = self.DEFAULT_FREE_AMOUNT
            self.free_amount_prev_update = current
        
        if source not in self.scores:
            raise InvalidOperationException(f"用户 {source} 不存在")
        if target not in self.scores:
            raise InvalidOperationException(f"用户 {target} 不存在")
        
        free = min(self.free_amount[source], amount)
        self.free_amount[source] -= free
        other = amount - free
        self.scores[source] -= other / 3
        self.scores[target] += amount

        # 记录修改
        self.modifications[source].add(self.Modification(-other / 3))
        self.modifications[target].add(self.Modification(amount))

        print(f"{source} 赠送了 {target} {amount} 分。")
        print(f"{source}: {self.scores[source]:.2f} -> {self.scores[source]:.2f}")
        print(f"{target}: {self.scores[target]:.2f} -> {self.scores[target]:.2f}")
        print(f"{source} 剩余免费赠送额度：{self.free_amount[source]:.2f}")

    def join(self, name: str) -> None:
        """
        加入一个新用户
        """
        if name in self.scores:
            raise InvalidOperationException(f"用户 {name} 已经存在")
        self.scores[name] = 0
        self.modifications[name] = set()
        self.free_amount[name] = self.DEFAULT_FREE_AMOUNT
        self.save()

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
        GiveScore = "GiveScore"  # 赠送分数

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
                    self.send_message(
                        Message(Message.Type.Modify, [name, amount]))
                elif cmd == "query":
                    print(json.dumps(self.data.to_dict(), indent=4))
                elif cmd.startswith("reduce"):
                    name, amount = cmd[7:].split()
                    self.data.reduce_score(name, float(amount))
                    self.send_message(
                        Message(Message.Type.ReduceScore, [name, float(amount)]))
                elif cmd == "show":
                    self.data.show()
                elif cmd.startswith("show"):
                    name = cmd[5:]
                    self.data.show_detail(name)
                elif cmd.startswith("give"):
                    source, target, amount = cmd[5:].split()
                    self.data.give_score(source, target, float(amount))
                    self.send_message(
                        Message(Message.Type.GiveScore, [source, target, float(amount)]))
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

            self.send_message(Message(Message.Type.First, [self.node_id, self.username]))
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
                            Message(Message.Type.Second, [new_id, message.data[1]]))

                        # 自己与其建立连接
                        host, port = new_id.split(":")
                        sock = socket.create_connection((host, int(port)))
                        self.peers[new_id] = sock

                        # 分享现在的数据
                        data = self.data.to_dict()
                        self.send_message(
                            Message(Message.Type.SyncData, [json.dumps(data)]), [sock])

                    log("peers: ", self.peers)
                    self.data.join(message.data[1])
                case Message.Type.Second:
                    # 新节点，经过一次转发
                    new_id: str = message.data[0]
                    new_name: str = message.data[1]
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
                    log("peers: ", self.peers)
                    self.data.join(new_name)
                case Message.Type.ConnectWith:
                    # 和给定的节点连接
                    new_id: str = message.data[0]
                    if new_id not in self.peers:
                        host, port = new_id.split(":")
                        sock = socket.create_connection((host, int(port)))
                        self.peers[new_id] = sock
                    log("peers: ", self.peers)

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
                case Message.Type.GiveScore:
                    # 赠送分数
                    source = message.data[0]
                    target = message.data[1]
                    amount = float(message.data[2])
                    self.data.give_score(source, target, amount)
                case _:
                    print(f"未知消息类型: {message.message_type}")
        try:
            while True:
                try:
                    message = MessageProcesser.recv(source)
                except ConnectionError:
                    print("连接已关闭")
                    break

                log(f"data: {message}")
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
        gui = ScoreboardGUI(node)
        gui.root.mainloop()
    except EOFError:
        del node
        exit()
