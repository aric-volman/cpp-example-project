CC := clang++

all: compile

compile:
	$(CC) -o server server.cpp
	$(CC) -o client client.cpp