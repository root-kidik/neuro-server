set(TARGET_NAME frame_buffer)

file(GLOB_RECURSE SRC *.cpp)

add_library(${TARGET_NAME} ${SRC})

target_include_directories(${TARGET_NAME}
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${TARGET_NAME}
PUBLIC
    core
    udp_server
)
