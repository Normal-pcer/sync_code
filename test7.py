import socket
import threading
import sys
import time

class ArraySyncNode:
    def __init__(self, host, port):
        # 网络配置
        self.host = host
        self.port = port
        self.peers = []  # 已连接的节点
        
        # 数据存储
        self.array = [0] * 16
        self.load_data()
        
        # 网络监听
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind((host, port))
        self.sock.listen(5)
        
        # 启动线程
        threading.Thread(target=self.accept_connections, daemon=True).start()
        self.input_handler()

    def load_data(self):
        try:
            with open(f"{self.host}_{self.port}.data", 'r') as f:
                self.array = list(map(int, f.read().split()))
        except:
            self.save_data()

    def save_data(self):
        with open(f"{self.host}_{self.port}.data", 'w') as f:
            f.write(' '.join(map(str, self.array)))

    def accept_connections(self):
        while True:
            client, addr = self.sock.accept()
            threading.Thread(target=self.handle_client, args=(client,)).start()

    def handle_client(self, client):
        try:
            while True:
                data = client.recv(1024).decode()
                if not data: break
                
                if data.startswith("SYNC"):
                    client.send(('CURRENT ' + ' '.join(map(str, self.array))).encode())
                elif data.startswith("UPDATE"):
                    _, index, value = data.split()
                    self.apply_update(int(index), int(value))
                    self.propagate_update(client, int(index), int(value))
        finally:
            client.close()

    def apply_update(self, index, value):
        if 0 <= index < 16 and value <= 100:
            self.array[index] = value
            self.save_data()
            print(f"数组已更新：{self.array}")

    def propagate_update(self, source_client, index, value):
        for peer in self.peers.copy():
            try:
                if peer != source_client:
                    peer.send(f"UPDATE {index} {value}".encode())
            except:
                self.peers.remove(peer)

    def connect_to_peer(self, host, port):
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
        except Exception as e:
            print(f"连接失败：{e}")

    def input_handler(self):
        time.sleep(0.5)  # 等待服务器启动
        while True:
            cmd = input("\n命令选项:\n1. join <IP> <PORT>\n2. modify <INDEX> <VALUE>\n3. query <INDEX>\n> ")
            
            if cmd.startswith("join"):
                _, host, port = cmd.split()
                self.connect_to_peer(host, int(port))
            
            elif cmd.startswith("modify"):
                _, index, value = cmd.split()
                self.apply_update(int(index), int(value))
                self.propagate_update(None, int(index), int(value))
            
            elif cmd.startswith("query"):
                _, index = cmd.split()
                print(f"a[{index}] = {self.array[int(index)]}")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("使用方法: python script.py <IP> <PORT>")
        sys.exit(1)
        
    node = ArraySyncNode(sys.argv[1], int(sys.argv[2]))
