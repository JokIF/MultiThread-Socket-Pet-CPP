#include <lib/server/server.h>
#include <lib/loops/loops.h>
#include <thread>
#include <stddef.h>


int main()
{
    size_t buffer_size = 128;
    char *buffer = (char *)malloc(128 * sizeof(char));
    Server server("127.0.0.1", 8000, buffer, buffer_size);
    std::thread accept_thread(accept_data, std::ref(server));

    accept_thread.join();
}