#include <string>
static constexpr int MAX_BUF_SIZE = 4096;

class clientclass {
    private:
        int sock;
        int port;
        int bytes;
        char buf[MAX_BUF_SIZE];
        std::string ipaddr;
    public:
        clientclass(int port, std::string ipaddr);
        std::string receive();
        void closeConnection();
        void connectToServer();
        void sendToServer(std::string input);
};