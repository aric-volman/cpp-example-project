#include <string>

class clientclass {
    private:
        int sock;
        int port;
        std::string ipaddr;
    public:
        clientclass(int port, std::string ipaddr);
        int connectToServer();
        int getSock();
};