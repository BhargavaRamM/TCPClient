#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "../src/TCPClient.hpp"

int main(int argc, char** argv) {
    std::string host = "192.168.1.101";
    TCPClient tcpClient(host, 49156);
    tcpClient.connect();
    std::vector<uint8_t> data;
    data.reserve(16384);

    std::thread t1([&]() {
        // 
	// This thread is created to send request messages to server.
	// Usually, data we send is GPB (Google Protocol Buffer) encoded Messages.
	// Proto files are required to encode messages.
	// CodedInputStream and ArrayInputStream are used to wrap data buffer
	// Encoded message is then sent to server.
	// 

	std::vector<uint8_t> request(1024);

	// assume we have data encoded using proto files and stored it in data.
	while (true) {
		auto bytes_sent = tcpClient.send_message(data.data(), data.size());
		if (bytes_sent < 0)
			throw std::runtime_error("Send Failed!");
		sleep(2);
	}
    });

    while (true) {
	//  google::protobuf::io::CodedInputStream cis(data.data(), 16384);
        data.clear();
        auto bytes_recv = tcpClient.receive_message(data.data(), 16384);
        if (bytes_recv <= 0) {
            std::cerr << "ERROR reading from socket" << std::endl;
        } else {
            std::cout << "Received - " << bytes_recv << " bytes " << std::endl;
	    //
	    // Received data from server.
	    // Parse it and make use of it.
	    //
        }
    }
    t1.join();
    return 0;
}
