#include <string>
static constexpr int MAX_BUF_SIZE = 4096;

class serverclass {
    private:
        int listeningSock;
        int clientSock;
        int port;
        char buf[MAX_BUF_SIZE];
        int bytes;
        std::string ipaddr;
    public:
        serverclass(int port, std::string ipaddr);
        std::string receive();
        void closeConnection();
        void listenAndConnect();
        void sendToClient(std::string message);
};