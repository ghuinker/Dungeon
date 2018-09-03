Makeall: main clean

main: main.o dungeongen.o
	gcc dungeongen.o main.o -o main

dungeongen.o: dungeongen.c
	gcc -c dungeongen.c -Wall -Werror -ggdb -o dungeongen.o

main.o: main.c
	gcc -c main.c -Wall -Werror -ggdb -o main.o
clean:
	rm -f *.o dungeongen *~
