	.data
A:   .word  15
B:   .word  20
X:   .word  0
H:   .word  3

	.text

	main: 

lw $s0,A
lw $s1,B
lw $s2,X

	blt $s0,$s1,etiqueta1
	beq $s0,$s1,etiqueta3
	move $s7, $s1
	j etiqueta2

etiqueta1:
	move $s7,$s0

etiqueta2:
	sw $s7,X

etiqueta3:
	sw $s4,-1