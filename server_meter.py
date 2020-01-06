#!/usr/bin/python env
# coding:utf-8
import socket

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(("127.0.0.1", 5025))
server.listen(10)
while True:
    conn, addr = server.accept()
    print(addr)
    while True:
        recv = conn.recv(1024)
        print(recv.decode("utf-8"))
        if len(recv) == 0:
            break
        conn.sendall(recv)
    conn.close()
    print(addr, " is disconnect.")
