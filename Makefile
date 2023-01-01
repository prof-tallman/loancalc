
all: console_util.o mortgage.c
	gcc -Wall -Wextra -pedantic -std=c99 mortgage.c console_util.o -o mortgage.exe


console_util.o: console_util.c
	gcc -Wall -Wextra -pedantic -std=c99 -c console_util.c -o console_util.o


.PHONY: clean
clean:
	rm console_util.o
	rm mortgage.exe
