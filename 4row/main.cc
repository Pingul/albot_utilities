#include <iostream>
#include <string>
#include "sock_client.h"

#include "four_row.h"

void playServer(int port)
{
    std::cout << "running" << std::endl;
    TCPClient client("127.0.0.1", port);
    try {
        client.connect();
    } catch (TCPClient::CouldNotConnect& e) {
        std::cout << "Coult not connect" << std::endl;
        return;
    }

    while (true) {
        std::cout << 1 << std::endl;
        std::string msg = client.receive();
        std::cout << 2 << std::endl;
        std::cout << msg << std::endl;
        std::cout << "-- Search -- " << std::endl;
        Board b(msg);
        b.print();
        Minmax s(b, Board::Mine);
        s.print();
        std::string m = std::to_string(s.bestMove());
        std::cout << "Taking move: " << m << std::endl;
        std::cout << "-- End search " << std::endl;
        client.send(m);
        //client.send("1");
    }
}

void test()
{
    //Board b("0 0 0 0 0 -1 0 0 0 0 1 1 0 0 0 0 0 -1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0");
    Board b("0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0");
    //Board b2(b, Board::Opp, {3, 3, 6, 3, 5, 3});
    b.print();
    Minmax s(b, Board::Opp);
    s.print();
}

int main(int argc, char* argv[])
{
	int port = 4000;
    if (argc > 1)
        port = std::stoi(std::string(argv[1]));
    playServer(port);
	//test();
};
