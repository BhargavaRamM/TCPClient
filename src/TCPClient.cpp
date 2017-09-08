// 
// created by bmallem on 9/07/17.
//

// sockets
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <iostream>
#include <string>
#include <vector>
#include <cstring>

#include "TCPClient.hpp"

TCPClient::TCPClient(const std::string &host, uint16_t port) {
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket < 0)
      throw std::runtime_error("Socket Not Created!");
    memset(reinterpret_cast<char *>(&m_server_addr), 0, sizeof(m_server_addr));
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_port = htons(port);
    in_addr m_host_ip;
    hostent* m_host_hp;
    if (!inet_aton(host.c_str(), &m_host_ip))
        throw std::runtime_error("Error parsing host Ip");
    if ((m_host_hp = gethostbyaddr((const void*)&m_host_ip, sizeof m_host_ip, AF_INET)) == NULL)
      throw std::runtime_error("No Sensor associated with host ip");
    std::cout << "Sensor Name: " << m_host_hp->h_name << std::endl;
    bcopy(m_host_hp->h_addr, reinterpret_cast<char *>(&m_server_addr.sin_addr.s_addr),
          static_cast<size_t >(m_host_hp->h_length));
}

TCPClient::~TCPClient() {
    std::cout << "Closing Socket!" << std::endl;
    close(m_socket);
}

void TCPClient::connect() {
    // connect to sensor
    if (!m_socket)
      throw std::runtime_error("Socket not created!");
    if (::connect(m_socket, reinterpret_cast<sockaddr *>(&m_server_addr), sizeof (m_server_addr)) < 0)
        throw std::runtime_error("Connection Not Established!");
    std::cout << "Connection to Sensor established!" << std::endl;
}

uint64_t TCPClient::send_message(const uint8_t* message, uint64_t message_size) {
    auto result = send(m_socket, message, message_size, 0);
    if (result < 0)
      std::cerr << "Unable to send message to sensor" << std::endl;
    return static_cast<uint64_t >(result);
}

uint64_t TCPClient::receive_message(uint8_t *data, uint64_t data_size) {
    auto result = recv(m_socket, data, data_size, 0);
    std::cout << "Received message of Size: " << result << std::endl;
    if (result < 0)
      std::cerr << "recv() Failed!" << std::endl;
    return static_cast<uint64_t >(result);
}

void TCPClient::resolve(in_addr *address, const std::string &host) {
    int rc, err;
    struct hostent result;
    struct hostent *result_ptr;
    auto buffer_size = 4096U;
    auto buffer = malloc(buffer_size);
    while ((rc = gethostbyname_r(host.c_str(), &result, reinterpret_cast<char *>(buffer),
                                  buffer_size, &result_ptr, &err)) == ERANGE) {
        /* expand buf */
        buffer_size *= 2;
        auto tmp = realloc(buffer, buffer_size);
        if (tmp == NULL) {
            free(buffer);
	    std::cerr << "Cannot allocate buffer for resolving hostname" << std::endl;
        } else {
            buffer = tmp;
        }
    }
    memcpy(reinterpret_cast<void *>(address), result.h_addr_list[0],
           static_cast<size_t>(result.h_length));
    free(buffer);
    if (rc != 0 || result_ptr == NULL) {
      std::cerr << "Cannot resolve hostname" << std::endl;;
    }
}
