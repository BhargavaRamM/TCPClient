// sockets
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <string>
#include <vector>
#include <cstring>

class TCPClient {
 public:
    TCPClient(const std::string& host, uint16_t port);
    virtual ~TCPClient();
    void connect();
    uint64_t send_message(const uint8_t* data, uint64_t data_size);
    uint64_t receive_message(uint8_t *data, uint64_t data_size);

 private:
    void resolve(in_addr *address, const std::string &host);
    int m_socket;
    sockaddr_in m_server_addr;
};
