import socket
import threading
import sys
import time
from typing import List, Dict, Optional, Tuple
from uuid import uuid4

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

        # 数据存储
        self.array: List[int] = [0] * 16
        self.load_data()

        # 网络监听
        self.sock: socket.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind((host, port))
        self.sock.listen(5)
        
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
            threading.Thread(target=self.handle_client, args=(client,)).start()

    def handle_client(self, client: socket.socket) -> None:
        try:
            while True:
                data = client.recv(1024).decode()
                if not data:
                    break

                # 处理各种消息类型
                if data.startswith("SYNC"):
                    client.send(('CURRENT ' + ' '.join(map(str, self.array))).encode())
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
        finally:
            client.close()

    def handle_update_request(self, client: socket.socket, data: str) -> None:
        """处理来自普通用户的修改请求"""
        print("handle_update_request", data)
        _, req_id, index, value = data.split()
        print("is_admin: ", self.is_admin)
        if self.is_admin:
            choice = input(f"收到修改请求 {req_id}: 将a[{index}]设为{value}？(y/n) ")
            client.send(f"RESPONSE_UPDATE {req_id} {'1' if choice.lower() == 'y' else '0'}".encode())
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
        """广播消息给所有节点"""
        for peer in self.peers.copy():
            try:
                if peer != exclude:
                    peer.send(message.encode())
            except:
                self.peers.remove(peer)

    def connect_to_peer(self, host: str, port: int) -> None:
        """连接到其他节点"""
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            sock.connect((host, port))
            sock.send("SYNC".encode())
            response = sock.recv(1024).decode()
            
            if response.startswith("CURRENT"):
                new_array = list(map(int, response.split()[1:]))
                self.array = new_array
                self.save_data()
                print(f"已同步数据：{new_array}")
                
            self.peers.append(sock)
            threading.Thread(target=self.handle_client, args=(sock,)).start()
            
            # 同步管理员状态
            if self.is_admin:
                self.propagate(f"ADMIN_SET {self.host} {self.port}")
        except Exception as e:
            print(f"连接失败：{e}")

    def request_modification(self, index: int, value: int) -> None:
        """发起修改请求"""
        req_id = str(uuid4())
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
    if len(sys.argv) != 3:
        print("Usage: python script.py <IP> <PORT>")
        sys.exit(1)
        
    ArraySyncNode(sys.argv[1], int(sys.argv[2]))
