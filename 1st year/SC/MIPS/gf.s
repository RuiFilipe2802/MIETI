# UNTITLED PROGRAM

	.data	
I:	.word 	1
LIMIT:	.word	20
NEWLINE: .asciiz "\n"

	.text

main:
	lw $s1, I
	li $s2, 1
	lw $s3, LIMIT	

start: 
	li  $v0, 1 		# system call code for print_int
	move $a0, $s3 		# move g to argument 0
	syscall 		# print it

	li $v0, 4 		# identificador da chamada ao sistema para escrever uma string
	la $a0, NEWLINE 	# move endereço de A para $a0 (argumento necessario para a funcao print_str)
	syscall 		# chamada ao sistema

	beq $s1,$s3,end
	sub $s3, $s3, $s2
	j start
end:

# END OF PROGRAM
