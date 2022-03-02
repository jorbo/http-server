#include <iostream>
#include "tcp_server.h"


int main(int argc, char** argv){
    
    TcpServer server("localhost", "727");
    std::cout << server << "\n";
    server.start();

    return 0;
}