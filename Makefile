CFLAGS=-Wall -g -std=c89

main: data.o vector.o

vectortest: vector.o

clean:
	rm -rf main
	rm -rf data.o
	rm -rf vectortest
	rm -rf vector.o
