all: lsh

lsh : main.o lsh.o
	gcc -o lsh lsh.o main.o

lsh.o : lsh.c lsh.h

main.o : main.c lsh.h

clean:
	rm *.o lsh