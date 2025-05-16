all: main

func.o: func.c func.h
	gcc -c func.c

interface.o: interface.c interface.h
	gcc -c interface.c

main: main.c func.o interface.o
	gcc main.c func.o  interface.o -o main

run: main.exe
	.\main.exe