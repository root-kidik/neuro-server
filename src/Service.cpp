#include <Service.hpp>

namespace neuro_server
{

Service::Service()
    : m_io_thread{"IO Context", [this]() { m_io_context.run(); }}
    , m_video_receiver{m_io_context}
    // , m_video_streamer{m_io_context}
{
}

Service::~Service()
{
    m_io_context.stop();
}

} // namespace neuro_server
