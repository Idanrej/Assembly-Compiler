myprog : main.o first.o second.o utilities.o ops.o fileWriter.o
	gcc -g main.o first.o second.o utilities.o ops.o fileWriter.o -o myprog -lm
main.o : main.c asm.h
	gcc -c main.c -o main.o -lm -Wall -ansi -pedantic
first.o : first.c asm.h
	gcc -c first.c -o first.o  -Wall -ansi -pedantic
second.o : second.c asm.h
	gcc -c second.c -o second.o -Wall -ansi -pedantic
utilities.o : utilities.c asm.h
	gcc -c utilities.c -o utilities.o -Wall -ansi -pedantic
ops.o : ops.c
	gcc -c ops.c -o ops.o -Wall -ansi -pedantic
fileWriter.o : fileWriter.c asm.h
	gcc -c fileWriter.c -o fileWriter.o -Wall -ansi -pedantic

