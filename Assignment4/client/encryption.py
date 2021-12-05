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
    def read(self, file):
        with open(file, 'rb') as f:
            orig_file = f.read()
        return orig_file
    def write(self, file, data):
        with open(file, 'wb') as e:
            e.write(data)
    def encrypt(self, file_name):
        cipher = AES.new(self.key, self.format, self.pad(self.IV))
        orig_file = self.pad(self.read(file_name))
        return cipher.encrypt(orig_file)
    def decrypt(self, file_data):
        decipher = AES.new(self.key, self.format, self.pad(self.IV))
        return decipher.decrypt(file_data)