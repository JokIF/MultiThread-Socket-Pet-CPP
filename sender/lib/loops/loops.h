#ifndef LOOPS__H
#define LOOPS__H
#include <lib/buffer/buffer.h>
#include <lib/connect/connector.h>
#include <lib/render/cli_render.h>

void get_from_user_loop(BufferManager<std::string>&, CLIRender<>& render);

void post_calculate_sender(BufferManager<std::string>&, Connector&);

#endif