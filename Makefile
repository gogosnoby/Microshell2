microshell: main.o
	gcc -ansi -Wall -o microshell main.c
clean:
	rm -f microshell.o microshell
