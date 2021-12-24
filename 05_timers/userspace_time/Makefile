default: mytime

rpscissors.o: mytime.c
	gcc -c mytime.c -o mytime.o

rpscissors: mytime.o
	gcc mytime.o -o mytime 

clean:
	-rm -f mytime.o
	-rm -f mytime
