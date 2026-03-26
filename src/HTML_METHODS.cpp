#include "HTML_METHODS.h"
#include <iostream>
namespace HTML
{    
    void handle_request(int client_socket, char buffer[]){
        
        std::string request(buffer);
        std::cout << "HTTP Request:\n" << request << "\n\n"; 
        std::istringstream stream(request);
    
        std::string method, path, version;
        stream >> method >> path >> version;
        
        std::string body;
        
        if (path == "/") {
            FILE * response_file = fopen("./src/index.html", "r");
            if (response_file) {
                char temp[1024];
                size_t bytes;
                while ((bytes = fread(temp, 1, sizeof(temp), response_file)) > 0) {
                    body.append(temp, bytes);
                }
                fclose(response_file);
            } else {
                
                body = "<h1>500 Internal Server Error</h1>";
            }
        }
        else {
            body = "<h1>404 Not Found</h1>";
        }
        
        std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + std::to_string(body.size()) + "\r\n"
        "Connection: close\r\n"
        "\r\n" 
        +body;

        ::send(client_socket, response.c_str(), response.size(), 0);
        
    }
} // namespace HTML

