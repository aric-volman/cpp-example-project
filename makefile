CC := clang++
RM := rm

all: libs compile link

libs:
	cd lib;\
	$(CC) -c -fPIC ./src/serverclass.cpp;\
	$(CC) -shared serverclass.o -o libserverclass.so;\
	$(CC) -c -fPIC ./src/clientclass.cpp;\
	$(CC) -shared clientclass.o -o libclientclass.so;\
	$(RM) serverclass.o;\
	$(RM) clientclass.o;\

compile:
	cd src;\
	$(CC) -c server.cpp -o ../server.o;\
	$(CC) -c client.cpp -o ../client.o;\

link:
	mkdir -p bin
	$(CC) -Wall -o ./bin/server server.o -rpath /home/aric/Documents/cpp-example-project/lib -L./lib -lserverclass
	$(CC) -Wall -o ./bin/client client.o -rpath /home/aric/Documents/cpp-example-project/lib -L./lib -lclientclass
	$(RM) server.o;\
	$(RM) client.o;\