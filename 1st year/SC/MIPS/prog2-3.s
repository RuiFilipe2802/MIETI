
	.data		
v1:	.byte	1 
v2:	.byte	2 
v3:	.byte	4 
v4:     .byte   8
v5:     .byte   0

	.text
main:

	lb $s0,v1
	lb $s1,v2
	lb $s2,v3
	lb $s3,v4
	add $s0,$s0,$s1
	add $s0,$s0,$s2
	add $s0,$s0,$s3
	sb $s0,v5


