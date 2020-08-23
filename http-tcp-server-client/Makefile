CFLAGS = -g -Wall
CC = gcc

s_objects = httpServer.o sfunctions.o
c_objects = httpClient.o cfunctions.o

httpServer: $(s_objects)
	$(CC) -o httpServer $(s_objects)

httpClient: $(c_objects)
	$(CC) -o httpClient $(c_objects)

# you can omit the .c files in the prerequsite as in below for test.o and sort.o
# if you omit the recipe as well (the recipe is the command under the prereq)
# make will automatically understand it needs to use the .c file


sfunctions.o: sfunctions.c sfunctions.h
cfunctions.o: cfunctions.c cfunctions.h


.PHONY: clean zipit
clean:
	rm httpClient httpServer $(s_objects) $(c_objects)

zip:
	zip -r project1.zip httpClient.c cfunctions.h cfunctions.c httpServer.c sfunctions.h sfunctions.c Makefile screenshots images index.html README.md protocolDocument.md
