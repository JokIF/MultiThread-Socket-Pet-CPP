#include <lib/loops/loops.h>
#include <lib/server/server.h>
#include <string>
#include <iostream>


void accept_data(Server& server)
{
    server.accept();
    while (1)
    {
        int status = server.recv();
        if (status == 0)
        {
            server.accept();
            continue;
        }
        std::string data = server.get_data();
        if (data.length() > 2 && std::stoi(data) % 32 == 0)
        {
            std::cout << "Данные получены\n";
        }
        else
        {
            std::cout << "Ошибка в данных\n";
        }
    }
}