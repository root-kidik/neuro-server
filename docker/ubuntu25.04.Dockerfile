FROM ubuntu:25.04

RUN apt-get -y update                                                                                                                       && \
    apt-get -y upgrade                                                                                                                      && \
    apt-get -y install                                                                                                                         \
    ninja-build                                                                                                                                \
    clang                                                                                                                                      \
    cmake                                                                                                                                      \
    git                                                                                                                                        \
    python3                                                                                                                                    \
    python3-pip                                                                                                                                \
    libboost-all-dev                                                                                                                           \
    lldb                                                                                                                                       \
    clang-format                                                                                                                               \
    clangd                                                                                                                                     \
    clang-tidy                                                                                                                                 \
    lld                                                                                                                                     && \
    python3 -m pip install                                                                                                                     \
    yandex-taxi-testsuite                                                                                                                      \
    colorama                                                                                                                                   \
    --break-system-packages                                                                                                                 && \
    apt-get -y autoremove                                                                                                                   && \
    apt-get -y autoclean                                                                                                                    && \
    rm -rf /var/lib/apt/lists/*
