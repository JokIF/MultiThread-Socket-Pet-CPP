#include <lib/render/cli_render.h>
#include <lib/utils/validator.h>
#include <lib/buffer/buffer.h>
#include <lib/connect/connector.h>
#include <lib/loops/loops.h>
#include <thread>
#include <mutex>
#include <string>


int main()
{
    CLIRender render;
    render.add_validator(new LenghtValidator(64));
    render.add_validator(new IsDigitValidator);

    std::mutex mtx;
    BufferManager<std::string> buffer(mtx);

    Connector connector("127.0.0.1", 8000);

    std::thread get_user_thread(get_from_user_loop, std::ref(buffer), std::ref(render));
    std::thread sender_thread(post_calculate_sender, std::ref(buffer), std::ref(connector));
    
    get_user_thread.join();
    sender_thread.join();
}