#include "Socket.h"

namespace Socket_Util{

    int CreateSocket() {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("socket creation failed");
            exit(1);
        }
        return sock;
    }
    
    struct Accept_client * Client_Socket(int server_side_socket){
        sockaddr_in client_address{};
        socklen_t client_len = sizeof(client_address);
        int client_socket = accept(server_side_socket, (sockaddr*)&client_address, &client_len);
        
        if (client_socket < 0) {
            return nullptr;
        }
        
        struct Accept_client *client = (struct Accept_client*)malloc(sizeof(struct Accept_client));
        
        
        client->reciving_socket = client_socket;
        client->reciving_address = client_address;
        
        return client;
    }
    
    struct sockaddr_in * CreateAdrress(int port, const char * ip){
        
        struct sockaddr_in * address = (struct sockaddr_in*)malloc(sizeof(sockaddr_in));
        
        memset(address, 0, sizeof(sockaddr_in));   
        
        address->sin_family = AF_INET;
        address->sin_port = htons(port);
        
        if(ip == nullptr || strlen(ip) == 0){    
            address->sin_addr.s_addr = INADDR_ANY;
        }else{
            if (inet_pton(AF_INET, ip, &(address->sin_addr)) <= 0) {
                perror("inet_pton failed");
            }
        }
        
        return address;
    }
}