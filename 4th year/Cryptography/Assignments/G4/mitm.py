import sys, os, base64
from Crypto.Cipher import DES

def des_find(nr_bits, msg, ivsct):
    
    ivsct = base64.b64decode(ivsct)
    iv1 = ivsct[0:8]
    iv2 = ivsct[8:16]
    og_ct =ivsct[16:16+len(msg)]
    iv3 = ivsct[16:]
    nr_bytes = nr_bits//8
    dicionario_kb1 = {}


    it = range ((2**nr_bits)-1)
    for  i in it:
        k1 = (i).to_bytes(8, 'big')
        kb1 = base64.b64encode(k1)
        cipher_try = DES.new(k1,DES.MODE_CFB, iv = iv1)
        ct1_d = cipher_try.encrypt(msg.encode('utf-8'))
        
        dicionario_kb1[ct1_d] = kb1


    for  i in it:
        
        k2 = (i).to_bytes(8, 'big')
        kb2 = base64.b64encode(k2)

        cipher_try2 = DES.new(k2,DES.MODE_CFB, iv = iv2)
        cd = cipher_try2.decrypt(og_ct)
        
        
        
        if cd in dicionario_kb1 :
            possible_k1 =base64.b64decode(dicionario_kb1.get(cd))
            possible_k2 = base64.b64decode(kb2)
            cypher1 = DES.new(possible_k1, DES.MODE_CFB, iv = iv1)
            cypher2 = DES.new(possible_k2, DES.MODE_CFB, iv = iv2)

            pt = cypher2.decrypt(iv3)
            pt2 = cypher1.decrypt(pt)

            print('FOUND POSSIBLE KEY: k1=' + str(dicionario_kb1.get(cd)),', k2=' +str(kb2),', pt=' + str(pt2))


if __name__ == "__main__":
    des_find(int(sys.argv[1]), sys.argv[2], sys.argv[3])
