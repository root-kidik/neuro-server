set(TARGET_NAME jpeg_encoder)

file(GLOB_RECURSE SRC *.cpp)

add_library(${TARGET_NAME} ${SRC})

find_package(OpenCV REQUIRED)

target_include_directories(${TARGET_NAME}
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(${TARGET_NAME}
PUBLIC
    core
    ${OpenCV_LIBS}
    neural_network
)
