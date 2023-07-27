Tarefa :
		Para que a comunicação no chat consiga ser confidencial é utilizado a implementação de criptografia simétrica Fernet, esta utiliza o AES em modo CBC o que proporciona uma boa segurança criptográfica, o AES é uma cifra rápida mas forte também, neste caso de mensagens instantaneas é importante que haja alguma rapidez, a maior limitação do uso desta implementação é que não demonstra bytes não autenticados, no entanto o tamanho de dados aqui será relativamente pequenos o que é ideal.  
		A segurança desta implementação requer a anonimidade da chave, como neste caso utilizamos um acordo Diffie-Hellman parte da chave nunca terá de sair do computador do utilizador, o restante, a parte pública da chave é passada por uma função de derivação de chaves e o resultado é então usado como chave de encriptação na implementação acima referida, desta forma mesmo que a password pública seja de alguma forma comprometida é praticamente impossivel chegar á password original por falta de um elemento na equação.
		
Problemas na implementação:
		-Falta de conhecimentos em programação concorrencial em python.
		-Problemas na reversão de versão do python para uma menor que 3.8, resolvido com métodos novos async/await para as corrotinas, a substituir @asyncio.coroutine/yield from respectivamente.
