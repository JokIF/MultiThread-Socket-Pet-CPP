#include <iostream>
#include <string>
#include <lib/buffer/buffer.h>
#include <lib/render/cli_render.h>
#include <lib/connect/connector.h>
#include <lib/sum_executer/sum_exec.h>
#include <lib/utils/validator.h>


void get_from_user_loop(BufferManager<std::string>& buffer, CLIRender<>& render)
{
    std::string str;
    while(1)
    {
        try
        {
            str = render.render(std::cin);
        }
        catch (ValidateError& ve)
        {
            std::cout << ve.what() << '\n';
            continue;
        }
        buffer.await_push(str);
    }
}

void post_calculate_sender(BufferManager<std::string>& buffer, Connector& conn)
{
    conn.connect();
    std::string data;
    uint64_t sum_;

    while (1)
    {
        data = buffer.await_pop();
        std::cout << "output: " << data << '\n';
        sum_ = sum_all_digit<uint64_t>(data);
        conn.send(std::to_string(sum_));
    } 
}