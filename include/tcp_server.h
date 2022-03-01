#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__
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
#include <iostream>
class TcpServer {
    private:
        std::string _ip;    
        std::string _port;    
        int _socket;

        bool _connected;
        void *get_in_addr(struct sockaddr *sa)
{
            if (sa->sa_family == AF_INET) {
                return &(((struct sockaddr_in*)sa)->sin_addr);
            }

            return &(((struct sockaddr_in6*)sa)->sin6_addr);
        }

    public:        
        TcpServer(std::string ip = "127.0.0.1", std::string port="80");   
        ~TcpServer();
        friend std::ostream& operator<<(std::ostream &os, const TcpServer& tcp_server);

        void start();
        void stop();

        
};
#endif // __TCP_SERVER_H__