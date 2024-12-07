from socket import getaddrinfo, socket, gethostname, AF_INET, SOCK_STREAM
from threading import Thread
from time import sleep

from tkinter import Tk, Text, StringVar, Entry, Button, END

username = 'user'
running = False
sending = False
window = Tk()
window.title('Chat Client')
window.geometry('1024x720')
text = Text(window, width=65, height=32)
text.config(state='disable')
text.pack(padx=5, pady=5)


def send_message():
    global sending
    sending = True


var = StringVar()
var.set("")
entry = Entry(window, width=58, textvariable=var)
entry.place(x=10, y=440)
btn = Button(window, width=5, text='发送', command=send_message)
btn.place(x=425, y=435)


def recv(c, t2):
    global username
    global running
    username = input("input the user name:")
    c.send(username.encode("utf-8"))
    t2.start()
    while running:
        sleep(0.1)
        try:
            data = c.recv(1024).decode("utf-8")
            # print(data)
            # print(type(data))
            text.config(state='normal')
            text.insert(END, data+'\n')
            text.config(state='disable')
            # if not data:
            #    break
        except Exception as e:
            print(e)
            running = False


def send(c):
    global sending
    global running
    sleep(0.5)
    while running:
        sleep(0.1)
        if sending:
            if var.get() == "":
                continue
            # text.config(state='normal')
            # data=username+' : '+var.get()+'\n'
            # text.insert(END,data)
            # text.config(state='disable')
            data = var.get()
            c.send(data.encode("utf-8"))
            var.set("")
            if data == "quit":
                running = False
            sending = False


if __name__ == "__main__":
    ip = "10.50.203.73"
    ip, port = input('输入服务端的ip和端口,用":"分割:').split(':')
    port = int(port)
    addrs = getaddrinfo(gethostname(), None)
    client = socket(AF_INET, SOCK_STREAM)

    try:
        client.connect((ip, port))
        running = True
        t2 = Thread(target=send, args=(client,))
        t1 = Thread(target=recv, args=(client, t2))
        t1.start()
        window.mainloop()
        t1.join()
        t2.join()
    except Exception as e:
        print(e)
    finally:
        print("连接已被关闭")
        client.close()
