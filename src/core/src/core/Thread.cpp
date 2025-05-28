#include <atomic>
#include <cassert>
#include <cerrno>
#include <system_error>

#include <core/Thread.hpp>

namespace
{
constexpr std::uint8_t kThreadNameMaxLen    = 16;
const std::uint8_t     kHardwareConcurrency = std::thread::hardware_concurrency();
} // namespace

namespace core
{

Thread::~Thread()
{
    assert(m_thread.joinable() && "thread must be joinable");
    m_thread.join();
}

void Thread::SetName(const std::string& name)
{
    assert(name.size() <= kThreadNameMaxLen - 1 && "thread name too long");

    if (const auto ret = pthread_setname_np(pthread_self(), name.data()); ret != 0)
    {
        throw std::system_error(ret, std::system_category(), "can't set name to thread: '" + name + "'");
    }
}

void Thread::SetAffinity()
{
    static std::atomic<std::uint8_t> core_id = 0;

    assert(kHardwareConcurrency != 0 && "hardware_concurrency must be not equal 0");
    const std::uint8_t local_core_id = core_id.fetch_add(1) % kHardwareConcurrency;

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(local_core_id, &cpuset);

    if (const auto ret = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset); ret != 0)
    {
        throw std::system_error(ret, std::system_category(), "can't set thread affinity");
    }
}

} // namespace core
