CFLAGS=-Wall -g -std=c89

main: data.o

clean:
	rm -rf main
	rm -rf data.o
