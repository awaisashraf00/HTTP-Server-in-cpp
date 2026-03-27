#include "HTML_METHODS.h"


namespace HTML
{    
    void handle_request(int client_socket, char buffer[]){
        
        std::string request(buffer);

        std::istringstream stream(request);
        std::string method, path, version;
        stream >> method >> path >> version;
        
        std::string body;
        
        if(method==to_string(HttpMethod::GET)){
            if (path == "/") {
                FILE * response_file = fopen("./src/index.html", "r");
                if (response_file) {
                    char temp[2048];
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

        
    }

    std::unordered_map<std::string, std::string> Parse_Request(char request[])
    {
        std::cout<<"\n\n\n";

        std::unordered_map<std::string, std::string> headers;
        std::string line;
        
        std::istringstream stream(request);
        std::string method, path, version;
        stream >> method >> path >> version;
        
        std::getline(stream, line); 

        while (std::getline(stream, line) && line != "\r") {
            size_t colon = line.find(':');
            if (colon != std::string::npos) {
                std::string key = line.substr(0, colon);
                std::string value = line.substr(colon + 1);
                // Trim whitespace
                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t\r") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t\r") + 1);
                headers[key] = value;
            }
        }
        std::string body;
        
        if (headers.find("Content-Length") != headers.end()) {
            int content_length = std::stoi(headers["Content-Length"]);
            body.resize(content_length);
            stream.read(&body[0], content_length); // Read exact number of bytes
        }

        
        std::cout << "POST to path: " << path << "\n";
        std::cout << "Headers:\n";
        for (auto& [k,v] : headers) std::cout << k << ": " << v << "\n";
        std::cout << "Body: " << body << "\n";
        
        return headers;
    }



    std::string to_string(HttpMethod response)
    {

        switch (response)
        {
        case HttpMethod::GET:
            return "GET";
            break;
        case HttpMethod::HEAD:
            return "HEAD";
            break;
        case HttpMethod::POST:
            return "POST";
            break;
        case HttpMethod::PUT:
            return "PUT";
            break;
        case HttpMethod::DELETE:
            return "DELETE";
            break;
        case HttpMethod::CONNECT:
            return "CONNECT";
            break;        
        }
        return std::string();
    }
} 

