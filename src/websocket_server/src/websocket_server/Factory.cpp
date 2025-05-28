#include <websocket_server/Factory.hpp>
#include <websocket_server/WebsocketServer.hpp>

namespace websocket_server
{

std::unique_ptr<IWebsocketServer> MakeWebsocketServer()
{
    return std::make_unique<WebsocketServer>();
}

} // namespace websocket_server
