# UNTITLED PROGRAM

	.data	
Q: 	.asciiz 	"Insira um valor inteiro: "
A: 	.asciiz 	"O valor da soma e: "
C: 	.word	0

	.text

main:
	li $v0, 4 			# identificador da chamada ao sistema para escrever uma string
	la $a0, Q 			# move endereco de Q para $a0 (argumento necessario para a funcao print_str)
	syscall 			# chamada ao sistema

	li $v0, 5 			# identificador da chamada ao sistema para ler um inteiro
	syscall 			# chamada ao sistema
	move $s1, $v0 		# move de $v0 (valor lido da consola) para $s1
	
	li $v0, 4 			# identificador da chamada ao sistema para escrever uma string
	la $a0, Q 			# move endereco de Q para $a0 (argumento necessario para a funcao print_str)
	syscall 			# chamada ao sistema

	li $v0, 5 			# identificador da chamada ao sistema para ler um inteiro
	syscall 			# chamada ao sistema
	move $s2, $v0 		# move de $v0 (valor lido da consola) para 	$s2
	
	add $s3, $s1, $s2 
	sw $s3, C 

	li $v0, 4 			# identificador da chamada ao sistema para escrever uma string
	la $a0, A 			# move endereco de A para $a0 (argumento necessario para a funcao print_str)
	syscall 			# chamada ao sistema
	
	li $v0, 1 			# identificador da chamada ao sistema para print integer
	move $a0, $s3 		# move $s1 para $a0 (argumento necessario para a funcao print_int)
	syscall 			# chamada ao sistema

# END OF PROGRAM
