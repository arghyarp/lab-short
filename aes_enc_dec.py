from Crypto.Cipher import DES
from Crypto.Util.Padding import pad, unpad
# Plaintext and 64-bit master key
plaintext = "polar bear"
master_key = b"8bytekey"
# Convert plaintext to bytes
data = plaintext.encode("utf-8")
# DES encryption
cipher = DES.new(master_key, DES.MODE_ECB)
ciphertext = cipher.encrypt(pad(data, 8))
# DES decryption
decipher = DES.new(master_key, DES.MODE_ECB)
decrypted = unpad(decipher.decrypt(ciphertext), 8)
# Display results
print("Original Plaintext :", plaintext)
print("Master Key
:", master_key.decode())
print("Plaintext (Hex)
:", data.hex().upper())
print("Ciphertext (Hex)
:", ciphertext.hex().upper())
print("Decrypted Text
:", decrypted.decode())
if decrypted == data:
print("DES Encryption and Decryption Successful")
