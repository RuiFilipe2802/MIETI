.data		
x:	.half	0x11F 
y:	.half	0x111
z:	.half   0

	.text
main:

	lh $s0,x
	lh $s1,y
	add $s2,$s0,$s1
	sh $s2,z
	sub $s2,$s0,$s1
	sh $s2,z
