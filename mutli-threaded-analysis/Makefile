#if want more strict warnings use -pedantic in CFLAGS
#add -ansi before submission... however, if i use ansi... then clock_gettime() does not work

CFLAGS = -g -Wall -pthread -lrt  #may need -lc
CC = gcc

objects = mt-collatz.o functions.o

mt-collatz: $(objects)
	$(CC) -o mt-collatz $(objects) $(CFLAGS)

# you can omit the .c files in the prerequsite as in below for test.o and sort.o
# if you omit the recipe as well (the recipe is the command under the prereq)
# make will automatically understand it needs to use the .c file


functions.o: functions.c functions.h

.PHONY: clean zip
clean: 
	rm mt-collatz $(objects) 

zip:
	zip -r project2.zip analysis.pdf mt-collatz.c functions.c functions.h  README.md documentation.pdf Makefile getstats.sh racecondition.pg threads-vs-time.pg asubi.pg histogram.pg
