import socket
import threading
import sys
import time
import tkinter as tk
from tkinter import messagebox, simpledialog
from typing import List, Dict, Optional, Tuple
from uuid import uuid4


class ArrayGUI(tk.Tk):
    def __init__(self, node: "ArraySyncNode"):
        super().__init__()
        self.node = node
        self.title(f"Array Sync - {node.host}:{node.port}")
        self.geometry("600x200")  # 手动设置窗口尺寸

        # 数组显示（手动计算坐标）
        self.array_labels = []
        label_width = 30  # 每个标签宽度
        x_start = 20      # 起始X坐标
        y_pos = 20        # Y坐标

        for i in range(16):
            label = tk.Label(self, text="0", width=4, relief="ridge")
            # 手动计算位置：i*(宽度+间距) + 起始位置
            x_pos = x_start + i * (label_width + 5)
            label.place(x=x_pos, y=y_pos+10)
            self.array_labels.append(label)

            # 添加下标标签
            index_label = tk.Label(self, text=f"[{i}]")
            index_label.place(x=x_pos+5, y=y_pos-15)

        # 控制面板（手动定位）
        self.admin_var = tk.BooleanVar()

        # 管理员复选框
        admin_cb = tk.Checkbutton(
            self,
            text="管理员",
            variable=self.admin_var,
            command=self.toggle_admin
        )
        admin_cb.place(x=20, y=60)

        # 连接节点按钮
        connect_btn = tk.Button(
            self,
            text="连接节点",
            command=self.connect_dialog,
            width=10
        )
        connect_btn.place(x=20, y=100)

        # 修改数值按钮
        modify_btn = tk.Button(
            self,
            text="修改数值",
            command=self.modify_dialog,
            width=10
        )
        modify_btn.place(x=120, y=100)

        # 网络状态标签
        self.status_label = tk.Label(self, text="就绪")
        self.status_label.place(x=20, y=140)

        # 网络线程
        self.after(100, self.check_network)

    def update_array_display(self):
        # 只更新文字内容，位置已经固定
        for i, val in enumerate(self.node.array):
            self.array_labels[i].config(text=str(val))

    def toggle_admin(self):
        self.node.is_admin = self.admin_var.get()
        if self.node.is_admin:
            messagebox.showinfo("状态", "您现在是管理员")

    def connect_dialog(self):
        ip = simpledialog.askstring("连接节点", "输入IP地址:")
        port = simpledialog.askinteger("连接节点", "输入端口:")
        if ip and port:
            self.node.connect_to_peer(ip, port)

    def modify_dialog(self):
        index = simpledialog.askinteger(
            "修改数值", "索引 (0-15):", minvalue=0, maxvalue=15)
        value = simpledialog.askinteger(
            "修改数值", "新值:", minvalue=0, maxvalue=100)
        if index is not None and value is not None:
            if self.node.is_admin:
                self.node.apply_update(index, value)
            else:
                self.node.request_modification(index, value)

    def check_network(self):
        # 处理pending请求
        if self.node.current_request:
            req_id, index, value = self.node.current_request
            if messagebox.askyesno("审批请求", f"允许修改a[{index}]为{value}？"):
                self.node.approve_request(req_id)
            else:
                self.node.reject_request(req_id)
            self.node.current_request = None

        self.update_array_display()
        self.after(100, self.check_network)


class ArraySyncNode:
    def __init__(self, host: str, port: int):
        # 网络配置
        self.host: str = host
        self.port: int = port
        self.peers: List[socket.socket] = []
        # 新增管理员相关状态
        self.is_admin: bool = False
        self.admin_peers: List[Tuple[str, int]] = []
        self.pending_requests: Dict[str, threading.Event] = {}
        self.request_results: Dict[str, bool] = {}
        self.pending_requests: Dict[str, Tuple[int, int]] = {}  # 新增请求参数存储

        # 数据存储
        self.array: List[int] = [0] * 16
        self.load_data()

        # 网络监听
        self.sock: socket.socket = socket.socket(
            socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind((host, port))
        self.sock.listen(5)

        # 图形界面
        self.gui = ArrayGUI(self)
        self.current_request = None

        threading.Thread(target=self.accept_connections, daemon=True).start()
        self.input_handler()

    def load_data(self) -> None:
        try:
            with open(f"{self.host}_{self.port}.data", 'r') as f:
                self.array = list(map(int, f.read().split()))
        except FileNotFoundError:
            self.save_data()

    def save_data(self) -> None:
        with open(f"{self.host}_{self.port}.data", 'w') as f:
            f.write(' '.join(map(str, self.array)))

    def accept_connections(self) -> None:
        while True:
            client, addr = self.sock.accept()
            # 关键修复：将新连接加入peers列表
            self.peers.append(client)
            threading.Thread(target=self.handle_client, args=(client,)).start()

    def handle_client(self, client: socket.socket) -> None:
        try:
            while True:
                data = client.recv(1024).decode()
                if not data:
                    break

                print(data)
                import os
                os.system("start calc")

                # 处理各种消息类型
                if data.startswith("SYNC"):
                    client.send(
                        ('CURRENT ' + ' '.join(map(str, self.array))).encode())
                elif data.startswith("ADMIN_SET"):
                    _, admin_host, admin_port = data.split()
                    self.admin_peers.append((admin_host, int(admin_port)))
                elif data.startswith("REQUEST_UPDATE"):
                    self.handle_update_request(client, data)
                elif data.startswith("RESPONSE_UPDATE"):
                    self.handle_update_response(data)
                elif data.startswith("UPDATE"):
                    _, index, value = data.split()
                    self.apply_update(int(index), int(value))
                elif data.startswith("REQUEST_APPROVE"):
                    _, req_id, index, value = data.split()
                    if req_id in self.pending_requests:
                        self.apply_update(int(index), int(value))
                        del self.pending_requests[req_id]
                elif data.startswith("REQUEST_REJECT"):
                    _, req_id = data.split()
                    if req_id in self.pending_requests:
                        print(f"请求 {req_id} 被拒绝")
                        del self.pending_requests[req_id]
        finally:
            client.close()

    def approve_request(self, req_id: str) -> None:
        """管理员同意请求"""
        if req_id in self.pending_requests:
            index, value = self.pending_requests[req_id]
            self.apply_update(index, value)
            self.propagate(f"REQUEST_APPROVE {req_id} {index} {value}")

    def reject_request(self, req_id: str) -> None:
        """管理员拒绝请求"""
        if req_id in self.pending_requests:
            self.propagate(f"REQUEST_REJECT {req_id}")
            del self.pending_requests[req_id]

    def handle_update_request(self, client: socket.socket, data: str) -> None:
        """处理来自普通用户的修改请求"""
        _, req_id, index, value = data.split()
        if self.is_admin:
            self.current_request = (req_id, index, value)
        else:
            client.send(f"RESPONSE_UPDATE {req_id} 0".encode())

    def handle_update_response(self, data: str) -> None:
        """处理来自管理员的审批响应"""
        _, req_id, result = data.split()
        if req_id in self.pending_requests:
            self.request_results[req_id] = bool(int(result))
            self.pending_requests[req_id].set()

    def apply_update(self, index: int, value: int, propagate: bool = True) -> None:
        """应用数组修改"""
        if 0 <= index < 16 and value <= 100:
            self.array[index] = value
            self.save_data()
            print(f"数组已更新：{self.array}")
            if propagate:
                self.propagate(f"UPDATE {index} {value}")

    def propagate(self, message: str, exclude: Optional[socket.socket] = None) -> None:
        """改进的广播方法"""
        dead_peers = []
        print("propagate")
        print("peers ", self.peers)
        for peer in self.peers:
            try:
                if peer != exclude:
                    peer.send(message.encode())
            except:
                dead_peers.append(peer)

        # 清理失效连接
        for peer in dead_peers:
            self.peers.remove(peer)

    def connect_to_peer(self, host: str, port: int) -> None:
        """连接到其他节点并建立双向通信"""
        if (host, port) == (self.host, self.port):
            return  # 禁止连接自己

        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            sock.connect((host, port))
            # 关键修复：将新连接加入peers列表
            self.peers.append(sock)
            threading.Thread(target=self.handle_client, args=(sock,)).start()

            # 同步初始数据
            sock.send("SYNC".encode())
            response = sock.recv(1024).decode()
            if response.startswith("CURRENT"):
                new_array = list(map(int, response.split()[1:]))
                self.array = new_array
                self.save_data()
        except Exception as e:
            print(f"连接失败：{e}")

    def request_modification(self, index: int, value: int) -> None:
        """发起修改请求"""
        req_id = str(uuid4())
        self.pending_requests[req_id] = (index, value)  # 存储请求参数
        event = threading.Event()
        self.pending_requests[req_id] = event
        self.request_results[req_id] = False

        # 广播请求给所有节点
        self.propagate(f"REQUEST_UPDATE {req_id} {index} {value}")

        # 等待响应（最长10秒）
        event.wait(10)
        if self.request_results.get(req_id, False):
            self.apply_update(index, value)
            print("修改已通过")
        else:
            print("修改被拒绝或超时")

        del self.pending_requests[req_id]
        del self.request_results[req_id]

    def input_handler(self) -> None:
        time.sleep(0.5)
        while True:
            cmd = input(
                "\n命令选项:\n"
                "1. join <IP> <PORT>\n"
                "2. modify <INDEX> <VALUE>\n"
                "3. query <INDEX>\n"
                "4. admin <1/0>\n> "
            )

            if cmd.startswith("join"):
                _, host, port = cmd.split()
                self.connect_to_peer(host, int(port))

            elif cmd.startswith("modify"):
                _, index, value = cmd.split()
                if self.is_admin:
                    self.apply_update(int(index), int(value))
                else:
                    print("发起修改请求...")
                    self.request_modification(int(index), int(value))

            elif cmd.startswith("query"):
                _, index = cmd.split()
                print(f"a[{index}] = {self.array[int(index)]}")

            elif cmd.startswith("admin"):
                _, status = cmd.split()
                self.is_admin = bool(int(status))
                if self.is_admin:
                    print("您现在是管理员")
                    self.propagate(f"ADMIN_SET {self.host} {self.port}")
                else:
                    print("您现在是普通用户")


if __name__ == "__main__":
    try:
        if len(sys.argv) != 3:
            print("Usage: python script.py <IP> <PORT>")
            sys.exit(1)

        ArraySyncNode(sys.argv[1], int(sys.argv[2]))
    except EOFError:
        exit()
