.entry MAIN
.entry STR
MAIN: add r1,r2
LOOP: prn #23
	inc r4
	inc r5
	bne LOOP
	bne &LOOP
M:	sub r3,r4
	.data 100,12
	.data -4,6
STR:	.string "my prog"
G:	.string "hey there"
.extern R
.extern P
	dec R
	cmp P,#-9
	jmp M
	lea R,r4
	stop
