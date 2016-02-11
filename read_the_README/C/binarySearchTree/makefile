all: lab3p2

lab3p2: lab3p2.o lab3p2f1.o lab3p2f2.o lab3p2f3.o
	gcc lab3p2.o lab3p2f1.o lab3p2f2.o lab3p2f3.o -o lab3p2

lab3p2.o: lab3p2.c
	gcc -c lab3p2.c

lab3p2f1.o: lab3p2f1.c
	gcc -c lab3p2f1.c

lab3p2f2.o: lab3p2f2.c
	gcc -c lab3p2f2.c

lab3p2f3.o: lab3p2f3.c
	gcc -c lab3p2f3.c

clean:
	rm -rf *.o lab3p2
