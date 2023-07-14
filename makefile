# name: Nathan Koop
# lab section: X03L
# assignment: Assignment 2
# due date: March 19th
# file purpose: Compiles all files needed for the test executable.


#Establish the makefile variables.
CC = gcc
CFLAGS = -Wall -ansi -std=c99

#Creates all executables
all: test memManager.o

#Create the memManager object
memManager.o: memManager.c memManager.h
	$(CC) -c $<

#Create the test program executable
test: memManager.o test.c
	$(CC) $(CFLAGS) -g $^ -o $@
	chmod -cf 777 ./$@
	./$@
	chmod -cf 666 testFile1.txt
	chmod -cf 666 testFile2.txt
	chmod -cf 666 testFile3.txt
	mv testFile1.txt ./Test
	mv testFile2.txt ./Test
	mv testFile3.txt ./Test

#Create the tarball of the assignment directory
A2.tar.gz:
	tar -czvf A2.tar.gz makefile A2
	chmod -cf 666 A2.tar.gz

#Remove all .o files
clean:
	rm *.o

