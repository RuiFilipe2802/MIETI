import sys, string

def cesar(operacao, chave, mensagem):

	#String "mensagem" is converted in list 
	#and iterated to change each element to the new character

	res = list(mensagem)		
	altera = ord(chave.upper()) - ord('A')
	length = len(res)

	if operacao == "enc" :
		for i in range (length):
			res[i] = chr(ord(res[i]) + altera)

		res = "".join(res)

	elif operacao == "dec" :
		for i in range (length):
			res[i] = chr(ord(res[i]) - altera)
		res = "".join(res)
	else :
		print("Bad input")

	return res


if __name__ == "__main__":
	
	res = cesar(sys.argv[1], sys.argv[2], sys.argv[3])
	print(res)
