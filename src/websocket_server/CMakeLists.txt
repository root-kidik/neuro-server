set(TARGET_NAME websocket_server)

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
    jpeg_encoder
)
