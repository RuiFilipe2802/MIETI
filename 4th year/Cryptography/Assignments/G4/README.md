MITM:
	O meet in the middle attack simplifica a acçao de encontrar as chaves utilizadas. Para isto recorre à criação de um dicionário com todas as possibilidades da primeira chave, de seguida em vez de tentar cifrar novamente com todas as hipóteses da chave 2 (brute force) desencripta o cypher text original com todas as chaves dois possiveis enquanto compara o resultado com o do dicionario, quando encontrar uma igualdade sabemos que é uma das possiveis combinações. 
	Desta forma em vez de 2¹⁶ * 2¹⁶ possiveis combinações de K1 e K2 só percorremos 2¹⁶ + 2¹⁶ reduzindo drasticamente o tempo do ataque.