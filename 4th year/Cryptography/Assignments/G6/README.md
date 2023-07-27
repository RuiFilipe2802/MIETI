Tarefa 1:
	A utilização de hashes para o armazenamento de password é requerido para que mesmo que o ficheiro seja comprometido não seja possivel a recuperação das passwords devido á direção única das funções de hash.

	No entanto o uso direto destas abre a possibilidade a ataques por força bruta/dicionário porque todas as passwords passam exatamente pelo mesmo processo de hash o que leva a resultados iguais.

	Para combater este tipo de fraqueza adicionamos um parametro aleatório salt que quando adicionado á password varia o resultado da hash de cada vez que este é efectuado. Este salt é armazenado em conjunto com a hash, como cada password tem um salt diferente a construçao de dicionário torna-se inviável.

	O importante na segurança é conseguir manter um nível computacionalmente seguro, isto é que seja irrealista o poder computacional necessessário para descobrir o conteudo protegido, no caso das hashs isto é conseguido com o aumento do tempo que a operação demora, neste sentido a utilizaçao de funções de hash mais demoradas tais como o PBKDF2, bcrypt ou scrypt. 

Tarefa 2:
	O método de armazenamento de passwords do Dropbox tem a sua particularidade numa implementação com várias camadas, o núcleo deste sistema é baseado num bcrypt modificado. A password passa por uma função de hash inicial (SHA512) que normaliza o tamanho para 512 bits, de seguida o resultado passa por outra função de hash o bcrypt desta vez, isto para além de aumentar a entropia do resultado devido ao uso da salt única também aumenta o tempo de cálculo da hash, com um custo de 10, finalmente a sáida é encriptada com AES256 e uma chave secreta denominada de pepper. O objetivo disto é criar vários níveis de segurança que, para serem efectivos, requer que o salt e o pepper sejam guardados em sítios diferentes.
	
Tarefa3:
	Tal como refere o artigo, o ataque sofrido pelo Adobe acontece devido á utilização de uma metodologia de guardar passwords apenas por encriptação. Como demonstrado, uma análise rápida sobre os dados conseguidos com o ataque mostra várias gralhas na segurança.

	Uma das caracteristicas mais aparentes é o tamanho das passwords encriptadas, quando utilizada uma função de sentido único este tamanho seria fixo, isto impede a obtenção de informação sobre o tamanho da password, o que não acontece neste caso.

	O mais crítico em todo este processo de armazenamento é provavelmente o facto de toda a segurança estar sobre uma chave apenas, isto é extremamente inresponsável, caso alguem obtenha esta chave ganha acesso a todas as passwords presentes na base de dados.

	O uso de uma cifra por blocos, sofre devido á repetição, a encriptação de mensagens iguais com a mesma chave devolve sempre o mesmo bloco encriptado, se não forem tomadas medidas para encobrir isto como o uso de initialization vectors ou nonce's, para cada item. Isto permite a exploração de uma base de dados grande para encontrar padrões.
	Para além disto o facto da cifra em questão guardar as passwords com um null byte no final ajuda a que um dos padrões identificaveis seja ocasionalmente um bloco de NULL bytes, que significa que a password tem exatamente o nr de bytes do texto cifrado menos o tamanho do bloco. 

	A não encriptação das password hints é deixar ao critério do utilizador a segurança da sua própria password sendo que não estão protegidas de qualquer forma, a hint aqui é pra todos.