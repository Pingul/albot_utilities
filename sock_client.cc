#include "sock_client.h"
#include <iostream>

TCPClient::TCPClient(const std::string& addr, int port)
    : mAddress(addr), mPort(port)
{
    mServer.sin_addr.s_addr = inet_addr(mAddress.c_str());
    mServer.sin_family = AF_INET;
    mServer.sin_port = htons(port);
}

void TCPClient::connect()
{
    std::cout << "TCP connect to " << mAddress << ":" << mPort << std::endl;
    if (connected()) return;
    
    mSock = socket(AF_INET, SOCK_STREAM, 0);
    if (mSock == -1) 
        throw CouldNotConnect("socket creation failed");
    if (::connect(mSock, (struct sockaddr *)&mServer , sizeof(mServer)) < 0)
        throw CouldNotConnect("connecting to server failed");
    mConnected = true;

    std::cout << "connection successful" << std::endl;
}

std::string TCPClient::receive()
{
    std::cout << "tcp: receive... ";
    if (!connected())
        throw NotConnected();

    char buf[256];
    ssize_t s = recv(mSock, buf, 256, 0);
    std::cout << std::to_string(s) << " bytes" << std::endl;
    if (s < 0)  {
        throw std::runtime_error("receiving data. Error code: " + std::to_string(s));
    }
    return std::string(buf, s);
}

void TCPClient::send(const std::string& msg)
{
    std::cout << "tcp: send '" << msg << "'" << std::endl;
    if (!connected())
        throw NotConnected();

    if (::write(mSock, msg.c_str(), msg.size()) < 0)
        throw std::runtime_error("couldn't send data");
}

