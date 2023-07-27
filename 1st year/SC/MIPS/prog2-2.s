
	.data		
v1:	.half	1 
v2:	.half	2 
v3:	.half	4 
v4:     .half   8
v5:     .half   0

	.text
main:

	lh $s0,v1
	lh $s1,v2
	lh $s2,v3
	lh $s3,v4
	add $s0,$s0,$s1
	add $s0,$s0,$s2
	add $s0,$s0,$s3
	sh $s0,v5

