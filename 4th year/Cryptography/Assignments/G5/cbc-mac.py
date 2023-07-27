import sys, os, base64
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives import padding

def bitwise_xor(data, key):
    return bytes(m ^ k for m, k in zip(data, key))

def cbcmac_auth(m_bytes, k):
    #https://cryptography.io/en/latest/hazmat/primitives/padding.html?highlight=padding#module-cryptography.hazmat.primitives.padding
    padder = padding.PKCS7(128).padder()
    padded_m = padder.update(m_bytes) + padder.finalize()
    iv = bytearray(16)
    cipher = Cipher(algorithms.AES(k), modes.CBC(iv))
    encryptor = cipher.encryptor()
    ct = encryptor.update(padded_m) + encryptor.finalize()
    tag = ct[-16:len(ct)]
    return tag

def cbcmac_verify(t1, m_bytes, k):
    #https://cryptography.io/en/latest/hazmat/primitives/padding.html?highlight=padding#module-cryptography.hazmat.primitives.padding
    padder = padding.PKCS7(128).padder()
    padded_m = padder.update(m_bytes) + padder.finalize()
    iv = bytearray(16)
    cipher = Cipher(algorithms.AES(k), modes.CBC(iv))
    encryptor = cipher.encryptor()
    ct = encryptor.update(padded_m) + encryptor.finalize()
    t2 = ct[-16:len(ct)]
    return t1 == t2

def cbcmac_lengthextension_example(m1, m2):
    key = os.urandom(32)
    tag1 = cbcmac_auth(m1.encode('utf-8'),key)
    tag2 = cbcmac_auth(m2.encode('utf-8'),key)
    print(m1, end=" ; tag : ")
    print(base64.b64encode(tag1))
    print(m2, end= " ; tag : ")
    print(base64.b64encode(tag2))
    # check if tag1 verifies with m1, m2 / tag2 with m1, m2 :
    r1 = cbcmac_verify(tag1, m1.encode('utf-8'), key)
    r2 = cbcmac_verify(tag1, m2.encode('utf-8'), key)
    r3 = cbcmac_verify(tag2, m1.encode('utf-8'), key)
    r4 = cbcmac_verify(tag2, m2.encode('utf-8'), key)
    print("tag1 + m1: " + str(r1))
    print("tag1 + m2: " + str(r2))
    print("tag2 + m1: " + str(r3))
    print("tag2 + m2: " + str(r4))

    # create a m3 (based on m1 and m2 that verifies with tag2)

    # tips: - pad m1 using pkcs7
    padder = padding.PKCS7(128).padder()
    padded_m1 = padder.update(m1.encode('utf-8')) + padder.finalize()
    #       - do not pad m2
    #       - xor first block of m2 with tag1
    m2_tag1 = bitwise_xor(m2[0:16].encode('utf-8'), tag1)
    #       - Concatenate the padded M with the XOR of M' and tag1
    # m3 = padded_m1 + m2_tag1
    m3 = padded_m1 + m2_tag1 + m2[16:].encode('utf-8')

    r5 = cbcmac_verify(tag2, m3, key)
    print("tag2 + m3: " + str(r5))
    #This works because we countered the Exclusive OR done by the encryption with its own tag

if __name__ == "__main__":
    cbcmac_lengthextension_example(sys.argv[1], sys.argv[2])
