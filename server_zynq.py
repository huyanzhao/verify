#!/usr/bin/python env
# coding:utf-8
import socket
import time

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(("127.0.0.1", 7801))
server.listen(10)
while True:
    conn, addr = server.accept()
    print(addr)
    while True:
        recv = conn.recv(1024)
        print(recv.decode("utf-8"))
        if len(recv) == 0:
            break
        time.sleep(0.01)
        conn.sendall(recv)
    conn.close()
    print(addr, " is disconnect.")
