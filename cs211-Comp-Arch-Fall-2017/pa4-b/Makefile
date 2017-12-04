all:first

first: first.c
		gcc -g -Wall -Werror -lm -fsanitize=address -c Queue.c
		gcc first.c -o first -Wall -Werror -fsanitize=address -lm

clean:
		rm -r first
