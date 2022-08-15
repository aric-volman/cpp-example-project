#include <string>

class clientclass {
    private:
        int sock;
        int port;
        int bytes;
        char buf[4096];
        std::string ipaddr;
    public:
        clientclass(int port, std::string ipaddr);
        void connectToServer();
        void sendToServer(std::string input);
        void receive();
        void closeConnection();
        int getSock();
        int getBytes();
        char* getBuf();
};