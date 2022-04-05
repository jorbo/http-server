#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <memory>
#include <unistd.h>
#include <thread>
#include <utility>
#include "tcp_server.h"
void TcpServer::start()
{
    addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; 
    int status = getaddrinfo(this->_ip.c_str(), this->_port.c_str(), &hints, &res);

    int yes=1;
    setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    this->_socket = socket(res->ai_family, res->ai_socktype, 0);
    if(this->_socket == -1)
        perror("Unable to create socket connection: ");

    if(bind(this->_socket,res->ai_addr, res->ai_addrlen) == -1)
        perror("Unable to bind to ip address");

    if(listen(this->_socket, 50) == -1)
        perror("Unable to listen");


    this->_connected = true;



    std::thread thread(&TcpServer::serve_client, this);
    thread.join();

}

void TcpServer::serve_client(){
    char httpheader[8000]   = "HTTP/1.1 200 OK\r\n\n";
    char msg[8000];
    char line[100];
    FILE* indexhtml = fopen("index.html", "r");

    while(fgets(line, 100, indexhtml) != 0){
        strcat(msg, line);
    }
    strcat(httpheader, msg);
    while(this->_connected){        
        
        int client_fd = accept(this->_socket, NULL, NULL);

        if(client_fd == -1)
            perror("Unable to accept client connect: ");


        int bytes_sent;
        bytes_sent = send(client_fd, httpheader, sizeof(httpheader), 0);
        std::cout << "Bytes sent: " << bytes_sent << "\n";
        close(client_fd);
        std::cout << "Spawned thread"  << "\n";
    }
}

void TcpServer::stop(){
    close(this->_socket);
    this->_connected = false;

}

TcpServer::TcpServer(std::string ip, std::string port): _ip(ip), _port(port){}

TcpServer::~TcpServer()
{
    close(this->_socket);
    this->_connected = false;
}


std::ostream& operator<<(std::ostream &os, const TcpServer& tcp_server)
{
    os << "http://" << tcp_server._ip << ":" << tcp_server._port;
    return os;
}

