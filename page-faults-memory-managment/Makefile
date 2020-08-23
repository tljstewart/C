CFLAGS = -g -O0 -Wall -lrt
CC = gcc

objects = matrix.o functions.o

matrix: $(objects)
	$(CC) -O0 -o matrix $(objects)

# you can omit the .c files in the prerequsite as in below for test.o and sort.o
# if you omit the recipe as well (the recipe is the command under the prereq)
# make will automatically understand it needs to use the .c file


functions.o: functions.c functions.h

.PHONY: clean zip
clean: 
	rm matrix $(objects) 

zip:
	zip -r project3.zip pageFaultProblem.pdf runtimeExperiment.pdf matrix.c functions.c functions.h  README.md Makefile 
