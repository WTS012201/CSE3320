import socket
import threading
import os
from os import path

BUFFER = 64
STREAM_BUFF = 10240
HOST = socket.gethostbyname(socket.gethostname())
PORT = 9090
ADDR = (HOST, PORT)
FORMAT = 'utf-8'

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server.bind(ADDR)

def get_file(conn, addr):
    data_len = conn.recv(BUFFER).decode(FORMAT)  #   determine size of file name
    if data_len:
        data_len = int(data_len)
        file_name = conn.recv(data_len)
        file_name = file_name.replace(b'/',b'')
        file_name = file_name.replace(b'\\',b'')
        #file_name = str(file_name).replace('/', '').encode(FORMAT)
    if path.exists(file_name):
        with open(file_name, 'rb') as f:
            data = f.read()
        data_len = len(data)
        send_len = str(data_len).encode(FORMAT)
        send_len += b' ' * (BUFFER - len(send_len))
        conn.send(send_len)

        f_data_stream = bytes()
        for i in range(0, data_len - STREAM_BUFF, STREAM_BUFF):
            f_data_stream = data[i : i + STREAM_BUFF]
            conn.send(f_data_stream)
        start = int(data_len/STREAM_BUFF)*STREAM_BUFF 
        f_data_stream = data[start : data_len]
        conn.send(f_data_stream)
        print(f"Sent Successfully")
    else:
        conn.send(("-1").encode(FORMAT))
        print("File not on server. Aborting!")
def store_file(conn, addr):
    data_len = conn.recv(BUFFER).decode(FORMAT)  #   determine size of file name
    if data_len:
        data_len = int(data_len)
        file_name = conn.recv(data_len)
        file_name = file_name.replace(b'/',b'')
        file_name = file_name.replace(b'\\',b'')
        #file_name = str(file_name).replace('/', '').encode(FORMAT)
    data_len = conn.recv(BUFFER).decode(FORMAT)  #   determine size of data
    if data_len:
        data_len = int(data_len)
        data = bytes()
        for i in range(0, data_len - STREAM_BUFF, STREAM_BUFF):
            data += conn.recv(STREAM_BUFF)
        start = int(data_len%STREAM_BUFF)
        data += conn.recv(start)
        with open(file_name, 'wb') as f:
            f.write(data)
        print(f"Received Successfully")
def handle_client(conn, addr):
    print(f"[NEW CONNECTION] {addr} connected.")
    connected = True
    while connected:
        msg_len = int(conn.recv(BUFFER).decode(FORMAT))
        msg = conn.recv(msg_len).decode(FORMAT)
        if msg == "!DISCONNECT!":
            connected = False
        elif msg == "!GET_FILE!":
            print("Sending File")
            get_file(conn, addr)
        elif msg == "!STORE_FILE!":
            print("Receiving File")
            store_file(conn, addr)
def start():
    server.listen()
    print(f"[LISTENING] Server is listening on {HOST}")
    while True:
        conn, addr = server.accept()
        thread = threading.Thread(target=handle_client, args=(conn,addr))
        thread.start()
        print(f"[ACTIVE CONNECTIONS] {threading.activeCount() -1}")

print("[STARTING] Server is starting...")
start()