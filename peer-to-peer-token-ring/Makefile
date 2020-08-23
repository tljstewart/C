CFLAGS = -g -Wall
CC = gcc

all: bbServer bbClient
s_objects = bbServer.o sfunctions.o
c_objects = bbClient.o cfunctions.o

bbServer: $(s_objects)
	$(CC) -o bbServer $(s_objects)

bbClient: $(c_objects)
	$(CC) -lpthread -o bbClient $(c_objects)

# you can omit the .c files in the prerequsite as in below for test.o and sort.o
# if you omit the recipe as well (the recipe is the command under the prereq)
# make will automatically understand it needs to use the .c file


sfunctions.o: sfunctions.c sfunctions.h
cfunctions.o: cfunctions.c cfunctions.h


.PHONY: clean zipit
clean:
	rm bbClient bbServer $(s_objects) $(c_objects)

zip:
	zip -r stewart_timothy_project2.zip bbClient.c cfunctions.h cfunctions.c bbServer.c sfunctions.h sfunctions.c Makefile screenshots.pdf screenshots README.md protocolDocument.md
