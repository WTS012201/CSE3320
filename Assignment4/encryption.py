from Crypto.Cipher import AES
import hashlib

class file_crypt:
    def __init__(self, password, IV, FORMAT):
        self.key = hashlib.sha256(password.encode(FORMAT)).digest()
        self.IV = IV.encode(FORMAT)
        self.format = AES.MODE_CBC
        self.cipher = AES.new(self.key, self.format, self.pad(self.IV))
        self.decipher = AES.new(self.key, self.format, self.pad(self.IV))
    def pad(self, data):
        while len(data)%16 != 0:
            data = data + b" "
        return data
    def read(self, file):
        with open(file, 'rb') as f:
            orig_file = f.read()
        return orig_file
    def write(self, file, data):
        with open(file, 'wb') as e:
            e.write(data)
    def encrypt(self, file_name):
        orig_file = self.pad(self.read(file_name))
        encrypt_file = self.cipher.encrypt(orig_file)
        self.write(file_name, encrypt_file)
    def decrypt(self, file_name):
        encrypt_file = self.pad(self.read(file_name))
        decrypt_file = self.decipher.decrypt(encrypt_file)
        self.write(file_name, decrypt_file.rstrip())
file_c = file_crypt("password", "IV123", "utf-8")

while True:
    inp = input("Encrypt/Decrypt(E/D): ")
    if inp.upper() == "E":
        inp = input("File: ")
        file_c.encrypt(inp)
    elif inp.upper() == "D":
        inp = input("File: ")
        file_c.decrypt(inp)
    elif inp.upper() == "Q":
        inp = input("File: ")
        file_c.decrypt(inp)
    else:
        print("Invalid Input!")