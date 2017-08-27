#ifndef SOCK_CLIENT
#define SOCK_CLIENT

#include <string>
#include <stdexcept>
#include <sys/socket.h>
#include <arpa/inet.h> 

class TCPClient
{
    sockaddr_in mServer;
    std::string mAddress;
    int mPort;
    int mSock{-1};
    bool mConnected;
public:
    struct CouldNotConnect : public std::runtime_error {
        CouldNotConnect(const std::string& msg) : std::runtime_error(msg.c_str()) {}
    };
    struct NotConnected : public std::runtime_error {
        NotConnected() : std::runtime_error("") {}
    };

    TCPClient(const std::string& addr, int port);
    bool connected() { return mConnected; }
    void connect(); // throws error if not able to connect
    void send(const std::string&);
    std::string receive(); // blocking waiting for message
};

#endif
