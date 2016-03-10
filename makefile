#This is a comment, we save g++ in a variable CC.
CC=g++
#Another comment, a flag giving all them warnings 
CFLAGS=-Wall
all:
	$(CC) $(CFLAGS) main.cpp -o strut
