FROM ubuntu:24.04

RUN apt-get -y update                                                                                                                                           && \
    apt-get -y upgrade                                                                                                                                          && \
    apt-get -y install                                                                                                                                             \
    ninja-build                                                                                                                                                    \
    clang                                                                                                                                                          \
    cmake                                                                                                                                                          \
    git                                                                                                                                                            \
    git-lfs                                                                                                                                                        \
    python3                                                                                                                                                        \
    python3-pip                                                                                                                                                    \
    libopencv-dev                                                                                                                                                  \
    libboost-all-dev                                                                                                                                               \
    lldb                                                                                                                                                           \
    clang-format                                                                                                                                                   \
    clangd                                                                                                                                                         \
    clang-tidy                                                                                                                                                     \
    wget                                                                                                                                                           \
    libopencv-dev                                                                                                                                                  \
    lld                                                                                                                                                         && \
    wget https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2404/x86_64/cuda-keyring_1.1-1_all.deb                                                  && \
    apt-get -y install ./cuda-keyring_1.1-1_all.deb                                                                                                             && \
    rm cuda-keyring_1.1-1_all.deb                                                                                                                               && \
    apt-get -y update                                                                                                                                           && \
    apt-get -y install cuda-toolkit-12-8                                                                                                                        && \
    wget https://github.com/microsoft/onnxruntime/releases/download/v1.21.0/onnxruntime-linux-x64-gpu-1.21.0.tgz -O /tmp/onnxruntime-linux-x64-gpu-1.21.0.tgz   && \
    tar -xf /tmp/onnxruntime-linux-x64-gpu-1.21.0.tgz -C /opt                                                                                                   && \
    rm -rf /tmp/onnxruntime-linux-x64-gpu-1.21.0.tgz                                                                                                            && \
    cp -r /opt/onnxruntime-linux-x64-gpu-1.21.0 /opt/onnxruntime-linux-x64-rocm-1.21.0                                                                          && \
    rm -rf /opt/onnxruntime-linux-x64-rocm-1.21.0/lib/libonnxruntime.so.1.21.0                                                                                  && \
    rm -rf /opt/onnxruntime-linux-x64-rocm-1.21.0/lib/libonnxruntime_providers_shared.so                                                                        && \
    rm -rf /opt/onnxruntime-linux-x64-rocm-1.21.0/lib/libonnxruntime_providers_tensorrt.so                                                                      && \
    rm -rf c/libonnxruntime_providers_cuda.so                                                                                                                   && \
    apt-get -y install "linux-headers-$(uname -r)" "linux-modules-extra-$(uname -r)"                                                                            && \
    apt-get -y install python3-setuptools python3-wheel                                                                                                         && \
    wget https://repo.radeon.com/amdgpu-install/6.4/ubuntu/noble/amdgpu-install_6.4.60400-1_all.deb                                                             && \
    apt-get -y install ./amdgpu-install_6.4.60400-1_all.deb                                                                                                     && \
    rm -rf ./amdgpu-install_6.4.60400-1_all.deb                                                                                                                 && \
    apt-get -y update                                                                                                                                           && \
    apt-get -y install rocm                                                                                                                                     && \
    python3 -m pip install onnxruntime-rocm -f https://repo.radeon.com/rocm/manylinux/rocm-rel-6.4.1/ --break-system-packages                                   && \
    cp /usr/local/lib/python3.12/dist-packages/onnxruntime/capi/*.so /opt/onnxruntime-linux-x64-rocm-1.21.0/lib                                                 && \
    cp /usr/local/lib/python3.12/dist-packages/onnxruntime/capi/libonnxruntime.so.1.21.0 /opt/onnxruntime-linux-x64-rocm-1.21.0/lib                             && \
    cp /usr/local/lib/python3.12/dist-packages/onnxruntime_rocm.libs/librocm_smi64-17ea3081.so.7.5.60401 /usr/lib                                               && \
    python3 -m pip install                                                                                                                                         \
    yandex-taxi-testsuite                                                                                                                                          \
    colorama                                                                                                                                                       \
    opencv-python                                                                                                                                                  \
    --break-system-packages                                                                                                                                     && \
    apt-get -y autoremove                                                                                                                                       && \
    apt-get -y autoclean                                                                                                                                        && \
    rm -rf /var/lib/apt/lists/*
