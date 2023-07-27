import sys, os, base64
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes

def bit_alteration(text , bit):

    res = bytearray(text)
    res[bit>>3] ^= 1<<(bit&0x7)
    return res

def ecb(message, corrupt_b):

    bit = int(corrupt_b)
    key = os.urandom(32)
    iv = os.urandom(16)
    print("key:  " + str(base64.b64encode(key)))
    
    cipher = Cipher(algorithms.AES(key), modes.CBC(iv))
    encryptor = cipher.encryptor()

    ot = (message.encode('utf-8'))  #original message not cyphered
    ct = encryptor.update(message.encode('utf-8'))  #cyphered text from message
    
    print("ct:   " + str(base64.b64encode(ct)))

    ct2 = bit_alteration(ct,bit)        #cyphered text with altered bit
    ot2 = bit_alteration(ot,bit)        #original text with altered bit

    decryptor = cipher.decryptor()
    dt = decryptor.update(ct2)      #Decrypted text 
    
    print("\ndt:  "+str(dt))
    print("ot2: "+str(ot2))

if __name__ == "__main__":
    ecb(sys.argv[1], sys.argv[2])