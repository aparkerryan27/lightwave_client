cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)

project(lightwave_client
        VERSION 2019.1.0
        DESCRIPTION "add basic planning capabilities to agents"
        LANGUAGES CXX)

string(APPEND CMAKE_CXX_FLAGS " -fno-strict-aliasing -pthread ")

find_package(Dependencies)
install_dependency(https://github.com/germanespinosa/easy-tcp)
include_directories(dependencies/easy-tcp/include)

add_executable(client src/main.cpp)
target_link_libraries( client
        LINK_PUBLIC
        easy-tcp)