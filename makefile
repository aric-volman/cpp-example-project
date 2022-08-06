CC := clang++

all: compile

compile:
	$(CC) -o test test.cpp