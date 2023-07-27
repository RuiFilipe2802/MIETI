import sys, os, base64
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes

def ecb():
    key = os.urandom(32)
    print("key:  " + str(base64.b64encode(key)))
    cipher = Cipher(algorithms.AES(key), modes.ECB())
    encryptor = cipher.encryptor()
    ct  = encryptor.update(b"Refund of 2000 euros to customer xxxx@gmail.com;")
    ct += encryptor.update(b"Refund of 8500 euros to customer yyyy@gmail.com;")
    print("ct:   " + str(base64.b64encode(ct)))
    decryptor = cipher.decryptor()
    pt = decryptor.update(ct)
    print("pt:   " + str(pt))
    ct2 = ct[:16]+ct[64:96]+ct[48:64]+ct[16:48]
    pt2 = decryptor.update(ct2)
    print("ct2:  " + str(pt2))

if __name__ == "__main__":
    ecb()