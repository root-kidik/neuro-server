FROM ubuntu:25.04

RUN apt-get -y update                                                                                                                                         && \
    apt-get -y upgrade                                                                                                                                        && \
    apt-get -y install                                                                                                                                           \
    ninja-build                                                                                                                                                  \
    clang                                                                                                                                                        \
    cmake                                                                                                                                                        \
    git                                                                                                                                                          \
    git-lfs                                                                                                                                                      \
    python3                                                                                                                                                      \
    python3-pip                                                                                                                                                  \
    libopencv-dev                                                                                                                                                \
    libboost-all-dev                                                                                                                                             \
    lldb                                                                                                                                                         \
    clang-format                                                                                                                                                 \
    clangd                                                                                                                                                       \
    clang-tidy                                                                                                                                                   \
    wget                                                                                                                                                         \
    libopencv-dev                                                                                                                                                \
    lld                                                                                                                                                       && \
    wget https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2404/x86_64/cuda-keyring_1.1-1_all.deb                                                && \
    apt-get -y install ./cuda-keyring_1.1-1_all.deb                                                                                                           && \
    rm cuda-keyring_1.1-1_all.deb                                                                                                                             && \
    apt-get -y update                                                                                                                                         && \
    apt-get -y install cuda-toolkit-12-8                                                                                                                      && \
    wget https://github.com/microsoft/onnxruntime/releases/download/v1.21.0/onnxruntime-linux-x64-gpu-1.21.0.tgz -O /tmp/onnxruntime-linux-x64-gpu-1.21.0.tgz && \
    tar -xf /tmp/onnxruntime-linux-x64-gpu-1.21.0.tgz -C /opt                                                                                                 && \
    rm -rf /tmp/onnxruntime-linux-x64-gpu-1.21.0.tgz                                                                                                          && \
    python3 -m pip install                                                                                                                                       \
    yandex-taxi-testsuite                                                                                                                                        \
    colorama                                                                                                                                                     \
    opencv-python                                                                                                                                                \
    --break-system-packages                                                                                                                                   && \
    apt-get -y autoremove                                                                                                                                     && \
    apt-get -y autoclean                                                                                                                                      && \
    rm -rf /var/lib/apt/lists/*
