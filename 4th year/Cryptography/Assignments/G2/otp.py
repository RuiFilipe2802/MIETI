import sys, string, base64, os

def bitwise_xor(data, key):
	return bytes(m ^ k for m, k in zip(data, key))

def setup_RandomKey(nr_bytes, file_name):

	f = open (file_name, "wb")
	random_bytes = os.urandom(int(nr_bytes))
	f.write(base64.b64encode(random_bytes))
	f.close()


def otp_encode(msg, file_name):
	f = open(file_name, "rb")
	key = base64.b64decode(f.readline())
	msg_barray = bytearray(msg, 'utf-8')

	for i, ch in enumerate (msg_barray): 
		msg_encrypted = base64.b64encode(bitwise_xor(msg_barray, key))

	f.close()
	return msg_encrypted

def otp_decode(msg, file_name):
		
	f = open(file_name, "rb")
	key = base64.b64decode(f.readline())
	msg_barray = base64.b64decode(msg)
	for i, ch in enumerate(msg_barray):
		decrypted_msg = bitwise_xor(msg_barray, key)

	og_msg = ""
	for x in decrypted_msg:
		og_msg += chr(x)
	f.close()
	return og_msg




if __name__ == "__main__":

	if sys.argv[1] == "setup":
		setup_RandomKey(sys.argv[2], sys.argv[3])

	elif sys.argv[1] == "enc":
		res = otp_encode(sys.argv[2], sys.argv[3])
		print( res)
	elif sys.argv[1] == "dec":
		res =otp_decode(sys.argv[2], sys.argv[3])
		print(res)
	else :
		print("Wrong input")