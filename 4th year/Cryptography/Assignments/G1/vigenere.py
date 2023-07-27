import sys, string

def vigenere(operacao, chave, mensagem):

	#String "mensagem" is converted in list 
	#and iterated to change each element to the new character
	
	res = list(mensagem)
	key = list(chave)		

	ascii_upper = set (string.ascii_uppercase) 
	ascii_all = set(string.ascii_uppercase + string.ascii_lowercase)
	
	#Key is repeated until its length matches message's
	#this wnsures that Vigneres cypher is done correctly
	length = len(res)
	oglength = len(chave)

	while len(key) != length:
		i = len(key) - oglength
		key.append(key[i])

		


	if operacao == "enc" :
		for i in range (length):
			altera = ord(key[i].upper()) - ord('A')

			chk = res[i].islower()
			res[i] = chr(ord(res[i]) + altera)
			#When altered to encode the ASCII code for the letter always rises
			#so we got to check for upper bounds

			#Checks if letter is lower case before and after "encode"
			#this should prevent ASCCI code errors!
			
			#(update) it doesn't correct every OBE(out of bounds error) because
			#islower() function only returns false if its a upper case letter,
			# otherwise, as with symbols, it returns true

			if res[i].islower() != chk or (ord(res[i]) > 90 and ord(res[i]) < 97):
				
					res[i] = chr(64 + ( ord(res[i]) - ord('Z') ) )			

		res = "".join(res)

	elif operacao == "dec" :
		for i in range (length):
			altera = ord(key[i].upper()) - ord('A')

			chk = res[i].islower()
			res[i] = chr(ord(res[i]) - altera)

			#Verifies lower limits of the ASCII code value for it to be a letter
			if res[i].islower() != chk or (ord(res[i]) > 90 and ord(res[i]) < 97) or (ord(res[i]) < 65):
				res[i] = chr(123 - ( ord('a') - ord(res[i]) ) )
			

		res = "".join(res)
	else :
		print("Bad input")

	return res


if __name__ == "__main__":
	res = vigenere(sys.argv[1], sys.argv[2], sys.argv[3])
	print(res)
