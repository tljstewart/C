CFLAGS = -g -Wall -ansi -pedantic
CC = gcc

objects = myshell.o parse.o

myshell: $(objects)
	$(CC) -o myshell $(objects)

# you can omit the .c files in the prerequsite as in below for test.o and sort.o
# if you omit the recipe as well (the recipe is the command under the prereq)
# make will automatically understand it needs to use the .c file


parse.o: parse.c parse.h

.PHONY: clean zipit
clean: 
	rm myshell $(objects) 

zip:
	zip -r project1.zip myshell.c parse.h parse.c README.pdf Makefile 
