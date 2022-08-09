CC := clang++
RM := rm

all: libs compile link

libs:
	$(CC) -c -fPIC serverclass.cpp
	$(CC) -shared serverclass.o -o libserverclass.so

compile:
	$(CC) -c server.cpp
	$(CC) -o client.o client.cpp

link:
	$(CC) -Wall -o server server.o -rpath ./ -L./ -lserverclass
