#ifndef SERVER__H
#define SERVER__H
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <stdexcept>


class Server
{
    public:
        Server(std::string ip_address, int port, char *buffer_, size_t buffer_size_) : gl_buffer(buffer_), buffer_size(buffer_size_)
        {
            server_sock = socket(AF_INET, SOCK_STREAM, 0);
            if (server_sock == 0)
            {
                throw std::runtime_error("cannot create socket");
            }
            if (inet_pton(AF_INET, ip_address.data(), &server_sock_in.sin_addr.s_addr) <= 0)
            {
                throw std::runtime_error("invalid ip address");
            }
            server_sock_in.sin_family = AF_INET;
            server_sock_in.sin_port = htons(port);
            if (::bind(server_sock, (sockaddr *)(&server_sock_in), sizeof(server_sock_in)) < 0)
            {
                throw std::runtime_error("bind failed");
            }
        }
        ~Server()
        {
            ::close(client_sock);
            ::close(server_sock);
        }
        std::string get_data()
        {
            return std::string(gl_buffer);
        }
        void accept()
        {
            if (::listen(server_sock, 1) == -1)
            {
                throw std::runtime_error("listen failed");
            }

            client_sock = ::accept(server_sock, NULL, NULL);
            if (client_sock == 0)
            {
                throw std::runtime_error("accept failed");
            }
        }
        int recv()
        {
            if (client_sock == 0 || !check_connection())
            {
                close(client_sock);
                this->accept();
                return this->recv();
            }
            return ::recv(client_sock, (char *)gl_buffer, buffer_size, 0);
        }

    private:
        bool check_connection()
        {
            int error;
            socklen_t error_size = sizeof(error);
            int res = getsockopt(client_sock, SOL_SOCKET, SO_ERROR, &error, &error_size);
            return true ? res == 0 && error == 0 : false;
        }

        char *gl_buffer = nullptr;
        size_t buffer_size = 0;
        int server_sock = 0;
        int client_sock = 0;
        struct sockaddr_in server_sock_in;
};


#endif 