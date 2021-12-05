from Crypto.Cipher import AES
import hashlib
# pip install pycryptodome
class file_crypt:
    def __init__(self, password, IV, FORMAT):
        self.key = hashlib.sha256(password.encode(FORMAT)).digest()
        self.IV = IV.encode(FORMAT)
        self.format = AES.MODE_CBC
    def pad(self, data):
        while len(data)%16 != 0:
            data = data + b" "
        return data
    def encrypt(self, file_data):
        cipher = AES.new(self.key, self.format, self.pad(self.IV))
        return cipher.encrypt(self.pad(file_data))
    def decrypt(self, file_data):
        decipher = AES.new(self.key, self.format, self.pad(self.IV))
        return decipher.decrypt(file_data)