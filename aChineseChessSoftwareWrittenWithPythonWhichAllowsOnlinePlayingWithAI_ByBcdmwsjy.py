
import sys
import copy
import json
import math
import time
import random
import socket
import threading

import pygame

cx=cy=0
cfx=cfy=ctx=cty=-15
state=0
win=False
show=0
font=None
surf=None
surfx=0
curr=(0,)*256
computerRunning=False
moveResult=0

username='user'
running=False
sending=False
data=""

isOnChessboardList=(
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,
    0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,
    0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,
    0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,
    0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,
    0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,
    0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,
    0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,
    0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,
    0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
)

def isOnChessboard(pos):
    return isOnChessboardList[pos]!=0

def getx(pos):
    return pos&15

def gety(pos):
    return pos>>4

def mousePosToGrid(x,y):
    return (x+3)+((y+3)<<4)

def computerMethodToPlayer(method):
    start,end=method&255,method>>8
    startx,starty=(start&15)-3,(start>>4)-3
    endx,endy=(end&16)-3,(end>>4)-3
    return "%d%d%d%d"%(startx,starty,endx,endy)

def playerMethodTComputer(method):
    startx,starty,endx,endy=method
    return ((startx+3)+((starty+3)<<4))+(((endx+3)+((endy+3)<<4))<<8)

def recv(c,t2):
    global username
    global running
    global curr
    username=input("input the user name:")
    c.send(username.encode("utf-8"))
    t2.start()
    while running:
        time.sleep(0.1)
        try:
            #print("running...")
            data=c.recv(1024).decode("utf-8")
            if len(data)==0:
                continue
            print(data)
            if data=="ok":
                pass
            else:
                state,curr=json.loads(data)
                if state=="AI":
                    computerRunning=True
                elif state=="Player":
                    computerRunning=False
        except Exception as e:
            print(e,"*")
            running=False

def send(c):
    global sending
    global running
    global data
    time.sleep(0.5)
    while running:
        time.sleep(0.1)
        if sending:
            data=str(data)
            if data=="":
                continue
            c.send(data.encode("utf-8"))
            if data=="quit":
                running=False
            data=""
            sending=False

def showSubtitles(s):
    global show,surf,surfx
    if len(s)<=0:
        show=0
        return
    else:
        show=1
        siz=min(100,600//len(s))
        font=pygame.font.SysFont("SimHei",siz)
        surf=font.render(s,1,(255,0,0))
        surfx=350-len(s)*siz/2

def gameOver(c):
    global win

    win=True
    showSubtitles(c)

def getChess(chess):
    if chess==12:
        return "車",(255,0,0)
    elif chess==11:
        return "馬",(255,0,0)
    elif chess==10:
        return "相",(255,0,0)
    elif chess==9:
        return "仕",(255,0,0)
    elif chess==8:
        return "帥",(255,0,0)
    elif chess==13:
        return "炮",(255,0,0)
    elif chess==14:
        return "兵",(255,0,0)
    elif chess==20:
        return "車",(0,0,0)
    elif chess==19:
        return "馬",(0,0,0)
    elif chess==18:
        return "相",(0,0,0)
    elif chess==17:
        return "仕",(0,0,0)
    elif chess==16:
        return "将",(0,0,0)
    elif chess==21:
        return "炮",(0,0,0)
    elif chess==22:
        return "卒",(0,0,0)
    else:
        return None,None

def createMap(update=True):
    global show

    screen.fill((255,200,55))

    for i in range(10):
        pygame.draw.line(screen,(0,0,0),(100,50+50*i),(500,50+50*i))

    for i in range(9):
        pygame.draw.line(screen,(0,0,0),(100+50*i,50),(100+50*i,250))

    for i in range(9):
        pygame.draw.line(screen,(0,0,0),(100+50*i,300),(100+50*i,500))

    pygame.draw.line(screen,(0,0,0),(100,250),(100,300))
    pygame.draw.line(screen,(0,0,0),(500,250),(500,300))

    pygame.draw.line(screen,(0,0,0),(250,50),(350,150))
    pygame.draw.line(screen,(0,0,0),(350,50),(250,150))
    pygame.draw.line(screen,(0,0,0),(250,400),(350,500))
    pygame.draw.line(screen,(0,0,0),(350,400),(250,500))

    chf=pygame.font.SysFont("SimHei",50)
    hjf=pygame.font.SysFont("SimHei",50)
    qzf=pygame.font.SysFont("SimHei",30)

    chs=chf.render("楚河",1,(0,0,0))
    screen.blit(chs,(150,250))

    hjs=hjf.render("汉界",1,(0,0,0))
    screen.blit(hjs,(350,250))

    for pos in range(256):
        if isOnChessboard(pos) and curr[pos]!=0:
            i,j=gety(pos)-3,getx(pos)-3
            chess=curr[pos]

            name,color=getChess(chess)
            if name is not None:
                pygame.draw.circle(screen,(255,215,0),(100+50*j,50+50*i),20,0)
                pygame.draw.circle(screen,color,(100+50*j,50+50*i),20,1)

    if cx>=0 and cy>=0:
        color=(255,0,0) if computerRunning else (0,255,0) 
        pygame.draw.circle(screen,color,(100+50*cx,50+50*cy),5,0)

    for pos in range(256):
        if isOnChessboard(pos) and curr[pos]!=0:
            i,j=gety(pos)-3,getx(pos)-3
            chess=curr[pos]

            name,color=getChess(chess)
            if name is not None:
                qzs=qzf.render(name,1,color)
                screen.blit(qzs,(85+50*j,35+50*i))

    if show:
        screen.blit(surf,(surfx,225))

    pygame.display.update()

def hq():
    global data,sending
    sending=True
    data="hq"

def startNewGame():
    global data,sending
    sending=True
    data="start"

def main():
    global cx,cy,cfx,cfy,ctx,cty,state,data,sending

    while True:
        clock.tick(60)
        for event in pygame.event.get():
            if event.type==pygame.QUIT:
                sending=True
                data="quit"
                pygame.quit()
                sys.exit()

            if not computerRunning:
                if event.type==pygame.KEYDOWN:
                    if event.key==ord("z"):
                        hq()
                    elif event.key==ord("n"):
                        startNewGame()

        if not win:
            mx,my=pygame.mouse.get_pos()
            if 90<mx<510 and 40<my<510:
                cx=int(round(mx/50,0))-2
                cy=int(round(my/50,0))-1
                if not computerRunning:
                    if moveResult!=0:
                        if moveResult==1:
                            gameOver("你赢了!")
                        elif moveResult==2:
                            gameOver("电脑长打作负,你赢了!")
                        elif moveResult==3:
                            gameOver("超过自然限着作和")
                        elif moveResult==-1:
                            gameOver("你输了!")
                        elif moveResult==-2:
                            gameOver("长打作负,你输了!")
                        elif moveResult==-3:
                            pass
                    if pygame.mouse.get_pressed()[0]:
                        if cx!=cfx or cy!=cfy:
                            if 8<=curr[mousePosToGrid(cx,cy)]<=14:
                                cfx,cfy=cx,cy
                                state=1
                            elif state==1:
                                ctx,cty=cx,cy
                                state=2
                        if state==2:
                            tmp=playerMethodTComputer((cfx,cfy,ctx,cty))
                            cfx=cfy=ctx=cty=-15
                            state=0
                            data="move %d"%tmp
                            sending=True
                            print(data)

        createMap()


if __name__=="__main__":
    ip,port=input('输入服务端的ip和端口,用":"分割:').split(':')
    port=int(port)
    addrs=socket.getaddrinfo(socket.gethostname(),None)
    client=socket.socket(socket.AF_INET,socket.SOCK_STREAM)

    try:
        client.connect((ip,port))
        running=True
        t2=threading.Thread(target=send,args=(client,))
        t1=threading.Thread(target=recv,args=(client,t2))
        t1.start()
        clock=pygame.time.Clock()
        pygame.display.set_caption("象棋")
        pygame.init()
        size=600,600
        screen=pygame.display.set_mode(size)
        main()
        t1.join()
        t2.join()
    except Exception as e:
        print(e)
    finally:
        print("连接已被关闭")
        client.close()
