from Crypto.Cipher import DES
# 64-bit plaintext and master key
plaintext = bytes.fromhex("0123456789ABCDEF")
master_key = bytes.fromhex("133457799BBCDFF1")
# DES encryption
cipher = DES.new(master_key, DES.MODE_ECB)
ciphertext = cipher.encrypt(plaintext)
# DES decryption
decipher = DES.new(master_key, DES.MODE_ECB)
decrypted = decipher.decrypt(ciphertext)
# Display results
print("Plaintext :", plaintext.hex().upper())
print("Master Key :", master_key.hex().upper())
print("Ciphertext :", ciphertext.hex().upper())
print("Decrypted :", decrypted.hex().upper())
if decrypted == plaintext:
print("DES Encryption and Decryption Successful")
