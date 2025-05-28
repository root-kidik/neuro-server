#pragma once

#include <VideoReceiver.hpp>
#include <VideoStreamer.hpp>

#include <core/Thread.hpp>

namespace neuro_server
{

class Service
{
public:
    Service();
    ~Service();

private:
    boost::asio::io_context m_io_context;

    VideoReceiver m_video_receiver;
    // VideoStreamer m_video_streamer;

    core::Thread m_io_thread;
};

} // namespace neuro_server
