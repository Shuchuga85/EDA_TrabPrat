all: main

func.o: func.c func.h
	gcc -c func.c

main: main.c func.o
	gcc main.c func.o -o main

run: main.exe
	.\main.exe