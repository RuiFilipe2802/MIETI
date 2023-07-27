ECB: 
	- O ataque representado denomina-ce de code book attack e baseia-se na repetição de blocos a que está sujeita uma cifra feita com o modo de operação ECB. Partes de texto iguais resultaram em blocos encryptados iguais, facilita o descobrimento de padrões.
CBC:
	-Caso seja alterado o bit 336, o valor 8500 seria alterado para 9500.
	
	-Este modo de operação reutiliza o cyphertext proveniente da encriptação do bloco anterior como Initialization Vector(IV) para o bloco seguinte isto possibilita a alteração de um bit num bloco sem modificar os restantes.

	-O programa demonstra a diferença entre o uso ou não da cifra no modo de operação CBC quando alterado um bit escolhido pelo utilizador

CFB:
	-Alterando o bit 82 conseguimos alterar a quantia de 2000 para 6000 e é danificado todo o bloco seguinte.

	-Este modo de operação utiliza 

OFB: 
	-Neste caso a alteração de um bit apenas modifica o mesmo sendo o restante da mensagem inalterado.
	-Este modo funciona de forma semelhante ao CFB mas em vez de utilizar o cypher text é utilizado o output da encriptação no bloco inicial (IV) tem as suas vantagens 