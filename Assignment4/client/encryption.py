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
        return decipher.decrypt(self.pad(file_data)).rstrip()

"""
FORMAT = 'utf-8'
crypt = file_crypt("password", "IV", FORMAT)
with open("test.png", 'rb') as f:
    f_data = f.read()
f_data_encrypt = crypt.encrypt(f_data)

f_data_stream = bytes()
print(len(f_data_encrypt))
STREAM_BUFF = 10240
for i in range(0, len(f_data_encrypt) - STREAM_BUFF, STREAM_BUFF):
    f_data_stream += f_data_encrypt[i : i + STREAM_BUFF]
start = int(len(f_data_encrypt)/STREAM_BUFF)*STREAM_BUFF 
end = int(len(f_data_encrypt))
f_data_stream += f_data_encrypt[start : end]
print(len(f_data_stream))


f_data_decrypt = crypt.decrypt(f_data_stream)
with open("test2.png", 'wb') as f:
    f.write(f_data_decrypt)
"""
