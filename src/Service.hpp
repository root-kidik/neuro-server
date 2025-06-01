#pragma once

#include <condition_variable>
#include <queue>

#include <frame_buffer/Factory.hpp>
#include <udp_server/Factory.hpp>
#include <websocket_server/Factory.hpp>
#include <yolo/Factory.hpp>

#include <core/Thread.hpp>

namespace neuro_server
{

class Service final
    : public frame_buffer::IFrameConsumer
    , private boost::noncopyable
{
public:
    Service();
    ~Service() override;

    void OnFrame(std::vector<std::uint8_t>&& frame) override;

private:
    std::queue<std::vector<std::uint8_t>> m_frames_queue;

    std::unique_ptr<frame_buffer::IFrameBuffer>         m_frame_buffer;
    std::unique_ptr<udp_server::IUdpServer>             m_udp_server;
    std::unique_ptr<websocket_server::IWebsocketServer> m_websocket_server;
    std::shared_ptr<yolo::IYolo>                        m_yolo;

    std::atomic<bool>       m_is_runned;
    std::mutex              m_mutex;
    std::condition_variable m_cv;
    core::Thread            m_thread;
};

} // namespace neuro_server
