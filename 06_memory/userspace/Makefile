default: memtest

memtest.o: memtest.c
	gcc -c memtest.c -o memtest.o

memtest: memtest.o
	gcc memtest.o -o memtest 

clean:
	-rm -f memtest.o
	-rm -f memtest
