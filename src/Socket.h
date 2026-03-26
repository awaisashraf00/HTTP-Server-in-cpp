#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include <thread>
#include <mutex>

namespace Socket_Util{

    int CreateSocket();
    
    struct Accept_client{    
        int reciving_socket;
        socklen_t client_len = sizeof(reciving_address);
        sockaddr_in reciving_address{};
    };
    
    struct Accept_client * Client_Socket(int server_side_socket);
    
    struct sockaddr_in * CreateAdrress(int port,const char * ip = nullptr);
    
}
#endif