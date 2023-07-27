	.data		
A:	.half	1 
B:	.half	2 
C:      .half   3
D:	.half   4
E:	.half   5
F:	.half   6
G:	.half   7
H:	.half   8
I:	.half   9

	.text
main:

	lh $s0,A
	lh $s1,B
	add $s0,$s0,$s1
	lh $s1,C
	add $s0,$s0,$s1
	lh $s1,D
	add $s0,$s0,$s1
	lh $s1,E
	add $s0,$s0,$s1
	lh $s1,F
	add $s0,$s0,$s1
	lh $s1,G
	add $s0,$s0,$s1
	lh $s1,H
	add $s0,$s0,$s1
	lh $s1,I
	add $s0,$s0,$s1