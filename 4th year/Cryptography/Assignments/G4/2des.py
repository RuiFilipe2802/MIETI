import sys, os, base64
from Crypto.Cipher import DES

# adiciona espaços ao fim da mensagem: tornar a tamanho da mensagem um multiplo de 8
# -- um espaço é sempre adicionado
def pad(msg):
    return msg + ((" " * 8)[0:(8 - ((len(msg)) % 8))])

def des_enc(key1, key2, msg):
    kb1  = base64.b64decode(key1)
    kb2  = base64.b64decode(key2)
    ivb1 = os.urandom(8)
    ivb2 = os.urandom(8)
    mp   = pad(msg)
    cipher1 = DES.new(kb1, DES.MODE_CFB, iv=ivb1)
    cipher2 = DES.new(kb2, DES.MODE_CFB, iv=ivb2)
    ct1 = cipher1.encrypt(mp.encode('utf-8'))
    ct2 = cipher2.encrypt(ct1)
    return base64.b64encode(cipher1.iv + cipher2.iv + ct2)

def des_dec(key1, key2, ivsct):
    kb1   = base64.b64decode(key1)
    kb2   = base64.b64decode(key2)
    ivsct = base64.b64decode(ivsct)
    ivb1  = ivsct[0:8]
    ivb2  = ivsct[8:16]
    ctb   = ivsct[16:]
    cipher1 = DES.new(kb1, DES.MODE_CFB, iv=ivb1)
    cipher2 = DES.new(kb2, DES.MODE_CFB, iv=ivb2)
    ct1 = cipher2.decrypt(ctb)
    pt  = cipher1.decrypt(ct1)
    return pt

def des_gen(number_of_bytes):
    k = os.urandom(number_of_bytes)
    return base64.b64encode((b'\x00' * (8-number_of_bytes)) + k)

if __name__ == "__main__":
    if sys.argv[1] == "enc":
        c = des_enc(sys.argv[2], sys.argv[3], sys.argv[4])
        print(c)
    elif sys.argv[1] == "dec":
        c = des_dec(sys.argv[2], sys.argv[3], sys.argv[4])
        print(c)
    elif sys.argv[1] == "gen":
        c = des_gen(int(sys.argv[2]))
        print(c)