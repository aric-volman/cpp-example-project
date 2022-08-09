#include <string>

class serverclass {
    private:
        int listeningSock;
        int clientSock;
        int port;
        std::string ipaddr;
    public:
        serverclass(int port, std::string ipaddr);
        int listenAndConnect();
        int getListeningSock();
        int getClientSock();
};