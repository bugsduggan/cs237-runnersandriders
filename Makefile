CFLAGS=-g -std=c89

main: node.o track.o course.o entrant.o event.o vector.o util.o

clean:
	rm -rf main
	rm -rf vector.o
	rm -rf util.o
	rm -rf node.o
	rm -rf track.o
	rm -rf course.o
	rm -rf entrant.o
	rm -rf event.o
