cmake_minimum_required(VERSION 3.16)
project(websocket_chatserver VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

add_executable(websocket_chatserver src/main.cpp src/chatserver.cpp include/chatserver.h)
target_include_directories(websocket_chatserver PUBLIC include)


find_package(Qt6 REQUIRED COMPONENTS WebSockets)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::WebSockets)



