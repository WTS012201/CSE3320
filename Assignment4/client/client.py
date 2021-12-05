import socket
from encryption import file_crypt
import os
from os import path

BUFFER = 64
HOST = socket.gethostbyname(socket.gethostname())
PORT = 9090
ADDR = (HOST, PORT)
FORMAT = 'utf-8'

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(ADDR)

crypt = file_crypt("password", "IV123", FORMAT)

def send(data):
    data = data.encode(FORMAT)
    send_len = str(len(data)).encode(FORMAT)
    send_len += b' ' * (BUFFER - len(send_len))
    client.send(send_len)
    client.send(data)
def send_file(data):
    send_len = str(len(data)).encode(FORMAT)
    send_len += b' ' * (BUFFER - len(send_len))
    client.send(send_len)
    client.send(data)
def receive_file(file):
    data_len = client.recv(BUFFER).decode(FORMAT)  #   determine size of file name
    if data_len:
        data_len = int(data_len)
        if data_len == -1:
            print(f"{file} is not on the server!")
            return
        file_data = client.recv(data_len)
    decrypted_data = crypt.decrypt(file_data)
    with open(file, 'wb') as f:
        f.write(decrypted_data)

print("Enter Q to quit session: ")
while True:
    inp = input("Send/Receive(S/R): ")
    if inp.upper() == "S":
        inp = input("File: ")
        if path.exists(inp):
            send("!STORE_FILE!")
            send(inp)
            send_file(crypt.encrypt(inp))
        else:
            print("Not a file!")
    elif inp.upper() == "R":
        inp = input("File: ")
        send("!GET_FILE!")
        send(inp)
        receive_file(inp)
    elif inp.upper() == "Q":
        send("!DISCONNECT!")
        break
    else:
        print("Invalid Input!")

