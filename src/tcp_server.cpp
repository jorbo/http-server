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
#include "tcp_server.h"
void TcpServer::start()
{
    addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; 
    int status = getaddrinfo("localhost", this->_port.c_str(), &hints, &res);
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
    while(this->_connected){
        sockaddr_storage client_sockfd;
        memset(&client_sockfd, 0, sizeof(client_sockfd));
        uint32_t addr_size = sizeof(client_sockfd);
        int new_fd;
        char s[INET6_ADDRSTRLEN];
        new_fd = accept(this->_socket, (struct sockaddr *) &client_sockfd, &addr_size);
        inet_ntop(client_sockfd.ss_family, get_in_addr((struct sockaddr *)&client_sockfd), s, sizeof s);
        printf("server: got connection from %s\n", s);
        if(new_fd == -1)
            perror("Unable to accept client connect: ");

        char* msg = "Hello, World!";
        int len, bytes_sent;
        len = strlen(msg);
        bytes_sent = send(new_fd, msg, len, 0);
        std::cout << "Bytes sent: " << bytes_sent << "\n";
    }

}

TcpServer::TcpServer(std::string ip, std::string port)
{
    this->_ip = ip;
    this->_port = port;
}

TcpServer::~TcpServer()
{
    close(this->_socket);
    this->_connected = false;
}


std::ostream& operator<<(std::ostream &os, const TcpServer& tcp_server)
{
    os << "ip: " << tcp_server._ip << "\n" << "port: " << tcp_server._port;
    return os;
}

