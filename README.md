TCPClient

Sample Client code that connects to a server by sending a request message.
Receives data from server, processes it. 

Note: 
This code is part of Utilities that we use in our projects to communicate with 
servers/sensors.
Server should handle the requests received and send data accordingly. All the data is usually encoded in GPB format. To decode, you should have 
corresponding proto files. 

BUILD:  
```
mkdir cmake-build-debug
cd cmake-build-debug
cmake ..
make
./tcpclient
```

Note: To build, `cmake >= 3.5` is required.  
