default: rpscissors

rpscissors.o: rpscissors.c
	gcc -c rpscissors.c -o rpscissors.o

rpscissors: rpscissors.o
	gcc rpscissors.o -lssl -lcrypto -o  rpscissors 

clean:
	-rm -f rpscissors.o
	-rm -f rpscissors
