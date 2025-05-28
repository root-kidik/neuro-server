#include <cassert>
#include <csignal>
#include <cstdio>
#include <unistd.h>

#include <Service.hpp>

int main()
{
    sigset_t set;
    int      sig{};
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    pthread_sigmask(SIG_BLOCK, &set, nullptr);

    neuro_server::Service service;

    sigwait(&set, &sig);
}
