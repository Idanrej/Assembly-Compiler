;file ps.as first pass erors

.entry LIST
.extern W
MAIN: add r3,LIST  extra after
LO.P: prn #48
	lea W,r4
	in r6   
	mov r3,K
	sub r1,r4
	bne END
	cmp K,#-6
	bne &END
	mov &R,r4
	mov r1,r0
	dec W
	cmp r9,r5
	cmp r5,r5
	prn r8
.entr MAIN
	jmp &LOOP
	add L3,&L3
END:	stop
STR:	.string "abcd
	.string accc"
	.data 1,,2
LIST:	.data 6 -9
	.data -100,
K:	.data 3.1
.extern L3

