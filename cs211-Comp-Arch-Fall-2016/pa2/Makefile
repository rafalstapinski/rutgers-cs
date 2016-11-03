all: hello

hello:
		gcc -Wall -g -o calc calc.c
		gcc -Wall -g -o format format.c -lm

calc:
	gcc -Wall -g -o calc calc.c

format:
	gcc -Wall -g -o format format.c -lm

clean:
	rm calc
	rm format
