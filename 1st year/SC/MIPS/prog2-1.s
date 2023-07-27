
	.data		
v1:	.word	1 
v2:	.word	2 
v3:	.word	4 
v4:     .word   8
v5:     .word   0

	.text
main:

	lw $s0,v1
	lw $s1,v2
	lw $s2,v3
	lw $s3,v4
	add $s0,$s0,$s1
	add $s0,$s0,$s2
	add $s0,$s0,$s3
	sw $s0,v5


