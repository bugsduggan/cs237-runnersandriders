CFLAGS=-Wall -g -std=c89

main: data.o vector.o util.o

clean:
	rm -rf main
	rm -rf data.o
	rm -rf util.o
	rm -rf vector.o
