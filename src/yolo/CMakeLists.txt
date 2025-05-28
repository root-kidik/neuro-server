set(TARGET_NAME yolo)

set(ONNXRUNTIME_VERSION 1.21.0)
set(ONNXRUNTIME_TYPE "onnxruntime-linux-x64-gpu-${ONNXRUNTIME_VERSION}")
set(ONNXRUNTIME_DIR "/opt/${ONNXRUNTIME_TYPE}")

file(GLOB_RECURSE SRC *.cpp)

add_library(${TARGET_NAME} ${SRC})

find_package(OpenCV REQUIRED)
find_package(CUDA REQUIRED)

target_include_directories(${TARGET_NAME}
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CUDA_INCLUDE_DIRS}
    ${ONNXRUNTIME_DIR}/include
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(${TARGET_NAME} 
PUBLIC 
    ${OpenCV_LIBS}
PRIVATE 
    ${ONNXRUNTIME_DIR}/lib/libonnxruntime.so
    ${CUDA_LIBRARIES}
)
