#pragma once

#include <thread>

#include <boost/noncopyable.hpp>

namespace core
{

class Thread final : private boost::noncopyable
{
public:
    template <typename Callable, typename... Args>
    Thread(std::string&& name, Callable&& fn, Args&&... args)
        : m_thread{[this,
                    name       = std::move(name),
                    fn         = std::forward<Callable>(fn),
                    args_tuple = std::make_tuple(std::forward<Args>(args)...)]()
                   {
                       SetAffinity();
                       SetName(name);
                       std::apply(fn, args_tuple);
                   }}
    {
    }

    ~Thread();

private:
    std::thread m_thread;

    static void SetName(const std::string& name);
    static void SetAffinity();
};

} // namespace core
