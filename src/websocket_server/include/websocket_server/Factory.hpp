#pragma once

#include <memory>

#include <websocket_server/IWebsocketServer.hpp>

namespace websocket_server
{

std::unique_ptr<IWebsocketServer> MakeWebsocketServer();

}
