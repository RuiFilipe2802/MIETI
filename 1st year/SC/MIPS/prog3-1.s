.data
v1:    .word 7
v2:    .word 2
v3:    .word 3
v4:    .word 4

.text

_start:

lw $t0, v1
lw $t1, v2
lw $t2, v3
lw $t3, v4

div $t0, $t1
mflo $s1
mfhi $s2

mult $t2, $t3
mflo $s3