#ifndef CONNECTOR__H
#define CONNECTOR__H
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include <stdexcept>
#include <thread>
#include <chrono>

class Connector
{
    public:
        Connector(std::string ip_address_, int port_) : ip_address(ip_address_), port(port_)
        {
            if (inet_pton(AF_INET, ip_address.data(), &sock_in.sin_addr.s_addr) <= 0)
            {
                throw std::runtime_error("invalid address");
            }

            sock_in.sin_family = AF_INET;
            sock_in.sin_port = htons(port);
        }

        void connect()
        {
            int status;

            while (1)
            {
                setup_socket();
                status = ::connect(sock, (sockaddr *)(&sock_in), sizeof(sock_in));
                if (status == 0)
                {
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        }

        ~Connector()
        {
            close(sock);
        }

        int send(std::string str)
        {
            return ::send(sock, str.data(), str.size(), 0);
        }

    private:
        void setup_socket()
        {
            close(sock);
            sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (sock == 0)
            {
                throw std::runtime_error("cannot create socket");
            }
        }
        int sock;
        std::string ip_address;
        int port;
        struct sockaddr_in sock_in;
};

#endif 