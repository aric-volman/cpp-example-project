#include <string>

class serverclass {
    private:
        int listeningSock;
        int clientSock;
        int port;
        char buf[4096];
        int bytes;
        std::string ipaddr;
    public:
        serverclass(int port, std::string ipaddr);
        int listenAndConnect();
        int receive();
        int getListeningSock();
        int getClientSock();
        void closeConnection();
        char* getBuf();
        int getBytes();
};