import socket
import threading
import os
from os import path

BUFFER = 64
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
        file_name = conn.recv(data_len).decode(FORMAT)
    if path.exists(file_name):
        with open(file_name, 'rb') as f:
            file_data = f.read()
        data_len = len(file_data)
        send_len = str(data_len).encode(FORMAT)
        send_len += b' ' * (BUFFER - len(send_len))
        conn.send(send_len)
        conn.send(file_data)
    else:
        conn.send(("-1").encode(FORMAT))
        print("File not on server. Aborting!")
    print(f"Sent {file_name} Successfully")
def store_file(conn, addr):
    data_len = conn.recv(BUFFER).decode(FORMAT)  #   determine size of file name
    if data_len:
        data_len = int(data_len)
        file_name = conn.recv(data_len).decode(FORMAT)

    data_len = conn.recv(BUFFER).decode(FORMAT)  #   determine size of data
    if data_len:
        data_len = int(data_len)
        data = conn.recv(data_len)
        with open(file_name, 'wb') as f:
            f.write(data)
        print(f"Received {file_name} Successfully")
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