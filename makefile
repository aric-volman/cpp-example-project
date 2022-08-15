CC := clang++
RM := rm
CXXFLAGS = -Wall -O3

all: libs compile link

libs:
	cd lib;\
	$(CC) $(CXXFLAGS) -c -fPIC ./src/serverclass.cpp;\
	$(CC) $(CXXFLAGS) -shared serverclass.o -o libserverclass.so;\
	$(CC) $(CXXFLAGS) -c -fPIC ./src/clientclass.cpp;\
	$(CC) $(CXXFLAGS) -shared clientclass.o -o libclientclass.so;\
	$(RM) serverclass.o;\
	$(RM) clientclass.o;\

compile:
	cd src;\
	$(CC) $(CXXFLAGS) -c server.cpp -o ../server.o;\
	$(CC) $(CXXFLAGS) -c client.cpp -o ../client.o;\

link:
	mkdir -p bin
	$(CC) $(CXXFLAGS) -o ./bin/server server.o -rpath /home/aric/Documents/cpp-example-project/lib -L./lib -lserverclass
	$(CC) $(CXXFLAGS) -o ./bin/client client.o -rpath /home/aric/Documents/cpp-example-project/lib -L./lib -lclientclass
	$(RM) server.o;\
	$(RM) client.o;\