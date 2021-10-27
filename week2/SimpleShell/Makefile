all: lshtest

lshtest : main.o lsh.o
	gcc -o lshtest lsh.o main.o

lsh.o : lsh.c lsh.h

main.o : main.c lsh.h

clean:
	rm *.o lsh