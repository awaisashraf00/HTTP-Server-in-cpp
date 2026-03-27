#include "Socket.h"
#include "HTML_METHODS.h"
std::mutex clients_mutex;

class Server {

private:

    int server_fd;
    sockaddr_in* address;
    socklen_t addrlen;
    Socket_Util::Accept_client* clients[10];
    int total_clients{0};

public:
    
    Server(int port) {
        int opt = 1;
        server_fd = Socket_Util::CreateSocket();
        address = Socket_Util::CreateAdrress(port);

        addrlen = sizeof(*address);
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        std::cout << "Server FD: " << server_fd << "\n" <<"\n";

        if (::bind(server_fd, (sockaddr*)address, addrlen) < 0) {
            perror("bind failed");
            exit(1);
        }

        if (::listen(server_fd, 10) < 0) {
            perror("listen failed");
        }
    }
    
    
    void Accept_new_Clients() {
        
        while (true) {

            struct Socket_Util::Accept_client *client_socket =  Socket_Util::Client_Socket(server_fd);
            
            
            if (client_socket == nullptr || client_socket->reciving_socket < 0) {
                perror("Client_Socket failed");
                continue;
            }else{
                std::cout << "\nNew client connected with FD: " << client_socket->reciving_socket << "\n";
                if(total_clients<10){

                    std::lock_guard<std::mutex> lock(clients_mutex);
                    clients[total_clients++] = client_socket;
                }
            }
            std::thread multiple_clients(&Server::recieve_data,this,client_socket->reciving_socket);
            multiple_clients.detach();
        }
        
    }
    
    void recieve_data(int socket) {
        std::cout << "\n Thread using FD: " << socket << "\n";
        for(;;){

            char buffer[2048] = {0};
            int bytes = ::recv(socket, buffer, 2048, 0);
            
            if (bytes <= 0) {
                perror("recieve failed form here");
                remove_client(socket);
                close(socket);
                return;
                
            }else{
                std::cout << ":: Message from client # :: " << socket-3 << "\n\n";
                
                HTML::handle_request(socket, buffer);
                std::unordered_map<std::string,std::string> request_data = HTML::Parse_Request(buffer);
                
                if(request_data["Connection"] != "keep-alive"){
                    remove_client(socket);
                    close(socket);
                    break;
                }
                
                buffer[bytes] = '\0';   
            }   
        }
    }

    void remove_client(int client_socket){
        std::lock_guard<std::mutex> lock(clients_mutex);
        for(int i = 0; i < total_clients; i++){
            if(clients[i]->reciving_socket == client_socket){
                delete clients[i];

                // shift left
                for(int j = i; j < total_clients - 1; j++){
                    clients[j] = clients[j+1];
                }

                total_clients--;
                break;
            }
        }
    
    }

    ~Server() {
        shutdown(server_fd, SHUT_RDWR);
        free(address);
    }
};

int main() {
    
    Server *server = new Server(8000);
    
    server->Accept_new_Clients();
    
    delete server;

    return 0;
}