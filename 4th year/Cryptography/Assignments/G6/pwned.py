import sys, os, base64, time
from cryptography.fernet import Fernet
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import hashes, hmac, padding
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes



def find_hashed(dictionary, hashed_res):
	d={}
	attack_f = open("attack.txt", "a")

	with open(dictionary) as f:
		for line in f:
			line = line.strip("\n")
			
			digest = hashes.Hash(hashes.SHA1())
			digest.update(line.encode('utf-8'))
			password_hash = digest.finalize()
			d[password_hash.hex()[6:]] = line
		
	with open(hashed_res) as fh:
		for lineh in fh:
			linen = lineh.rstrip()
			if linen[6:] in d:
				print(d.get(linen[6:]) + ";" + lineh )
				attack_f.write(d.get(linen[6:]) + ";" + lineh )

	f.close()



if __name__ == "__main__":
	find_hashed(sys.argv[1], sys.argv[2])
