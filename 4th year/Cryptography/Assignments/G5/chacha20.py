import sys, os, base64, time
from cryptography.fernet import Fernet
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import hashes, hmac, padding
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives.kdf.hkdf import HKDF


#This provides a way two generate 2 derivated keys from a single one without being able to return to the first
def key_deriv(key, size, salt):
    hkdf = HKDF(hashes.SHA256(), length= size, salt= salt, info = None)
    return hkdf.derive(key)


def fernet_manual_dec(key, token, salt):

    key_hmac = key_deriv(key, 16, salt)
    key_enc = key_deriv(key, 32, salt)
   
    token_bytes = base64.urlsafe_b64decode(token)

    version = token_bytes[0]
    timestamp = time.gmtime(int.from_bytes(token_bytes[1:9], byteorder='big'))
    nonce = token_bytes[9:25]
    ct = token_bytes[25:len(token_bytes)-32]
    tag = token_bytes[len(token_bytes)-32:len(token_bytes)]

    # print("  m key_hmac: " + str(base64.urlsafe_b64encode(key_hmac)))
    # print("  m key_enc:  " + str(base64.urlsafe_b64encode(key_enc)))
    # print("  m version:  " + str(version))
    # print("  m timestamp:" + time.strftime('%X %x %Z',timestamp))
    # print("  m Nonce:       " + str(base64.urlsafe_b64encode(nonce)))
    # print("  m ct:       " + str(base64.urlsafe_b64encode(ct)))
    # print("  m tag:      " + str(base64.urlsafe_b64encode(tag)))

 
    # decrypt to plaintext and tag
    cipher = Cipher(algorithms.ChaCha20(key = key_enc,nonce =nonce), mode = None)
    decryptor = cipher.decryptor()
    pt_tag = decryptor.update(ct) + decryptor.finalize()
    unpadder = padding.PKCS7(128).unpadder()
    pt = unpadder.update(pt_tag[0 : len(pt_tag)-32]) + unpadder.finalize()



    # check the mac from plain text vs tag
    h = hmac.HMAC(key_hmac, hashes.SHA256(), backend=default_backend())
    h.update(pt)
    ctag = h.finalize()
    if(ctag != tag):
        print("hmac tags don't match")
        return

    #print("  m message:  " + str(pt))
    return pt.decode('utf-8')

def fernet_manual_enc(key, m, salt):
    #Key derivation from master key to avoid sharing a key to authenticate and 
    key_hmac = key_deriv(key, 16, salt)
    key_enc = key_deriv(key, 32, salt)

    version = 126
    timestamp = int(time.time())
    time_tk = timestamp.to_bytes(8, 'big')
    nonce1 = os.urandom(16)

    #HMAC plain text as tag
    h = hmac.HMAC(key_hmac, hashes.SHA256(), backend=default_backend())
    h.update(m.encode('utf-8'))
    tag = h.finalize()
    #then

    #cypher plaintext and tag 
    padder = padding.PKCS7(128).padder()
    ct = padder.update(m.encode('utf-8')) + padder.finalize()
    cipher = Cipher(algorithms.ChaCha20(key = key_enc, nonce = nonce1), mode=None)
    encryptor = cipher.encryptor()
    ct = encryptor.update(ct+tag)
    
    #build packet
    token_bytes1 = bytearray(26+len(ct)+32)
   
    token_bytes1[0] = version
    token_bytes1[1:9] = time_tk
    token_bytes1[9:25] = nonce1
    token_bytes1[25:len(token_bytes1)-32] = ct
    token_bytes1[len(token_bytes1)-32:len(token_bytes1)] = tag

 

    # print("  m key_hmac: " + str(base64.urlsafe_b64encode(key_hmac)))
    # print("  m key_enc:  " + str(base64.urlsafe_b64encode(key_enc)))
    # print("  m version:  " + str(version))
    # print("  m timestamp:" + time.strftime('%X %x %Z',time.gmtime(int.from_bytes(time_tk, byteorder = 'big' ))))
    # print("  m nonce:       " + str(base64.urlsafe_b64encode(nonce1)))
    # print("  m ct:       " + str(base64.urlsafe_b64encode(ct)))
    # print("  m tag:      " + str(base64.urlsafe_b64encode(tag)))


    token1 = base64.urlsafe_b64encode(token_bytes1)


    return token1


if __name__ == "__main__":
    key = os.urandom(48)
    slt = os.urandom(16)
    
    token2 = fernet_manual_enc(key, sys.argv[1], slt) 
    m2 = fernet_manual_dec(key, token2, slt)
    print("token2:             " + str(token2))
    print("decrypted message2: " + str(m2))