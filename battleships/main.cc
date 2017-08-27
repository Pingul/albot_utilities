#include <iostream>
#include <sock_client.h>
#include <string>
#include <cstdlib>

#include "battleships.hh"

void playRandom(int port)
{
    TCPClient client("127.0.0.1", port);
    try {
        client.connect();
    } catch (TCPClient::CouldNotConnect& e) {
        std::cout << "Coult not connect" << std::endl;
        return;
    }

    auto msg = client.receive();
    std::cout << msg;
    std::string init_state = "0110000000000040000000004555550000400000000040000000000000000000033300002220000000000000000000000000";
    client.send(init_state);
    for (;;) {
        std::cout << client.receive();
        client.send(std::to_string(rand() % 100));
    }
}

void play(int port)
{
    TCPClient client("127.0.0.1", port);
    try {
        client.connect();
    } catch (TCPClient::CouldNotConnect& e) {
        std::cout << "Coult not connect" << std::endl;
        return;
    }

    int n = 0;
    battleship::Logic logic;
    while (true) {
        ++n;
        auto msg = client.receive();
        logic.parseBoardState(msg);
        std::cout << "======================" << std::endl;
        std::cout << "==== MOVE " << n << std::endl;
        std::cout << "======================" << std::endl;
        logic.print();
        auto action = logic.nextAction();
        client.send(action);
    }
}

void test() 
{
    battleship::Logic logic;
    logic.print();
    std::cout << "ACTION: " << logic.nextAction() << std::endl;
    logic.print();
    logic.parseBoardState("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    logic.print();
    std::cout << "ACTION: " << logic.nextAction() << std::endl;
    logic.print();
}

int main(int argc, char* argv[])
{
    int port = 4000;
    if (argc > 1) {
        port = std::stoi(std::string(argv[1]));
        playRandom(port);
    }
    play(port);
}
