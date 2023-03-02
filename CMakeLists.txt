cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)

project(lightwave_client
        VERSION 2019.1.0
        DESCRIPTION "add basic planning capabilities to agents"
        LANGUAGES CXX)

string(APPEND CMAKE_CXX_FLAGS " -fno-strict-aliasing -pthread ")

####
#### DEPENDENCIES
####

find_package (Dependencies QUIET)

if (NOT ${Dependencies_FOUND})
    if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/DependenciesConfig.cmake)
        file(DOWNLOAD https://raw.githubusercontent.com/germanespinosa/dependencies/main/DependenciesConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/DependenciesConfig.cmake)
    endif()
    set(Dependencies_DIR "${CMAKE_CURRENT_BINARY_DIR}")
    find_package (Dependencies REQUIRED)
endif()

####
#### END DEPENDENCIES
####
install_git_dependency(easy-tcp https://github.com/germanespinosa/easy-tcp CMAKE_PROJECT #can be used as a passive import, this flag compiles this in cmake
        INCLUDE_DIRECTORIES include #sort of merge a project into my own
        IMPORT_TARGETS easy-tcp) #brings over the target created by the CMake project
include_directories(dependencies/easy-tcp/include)

add_executable(client src/main.cpp)
target_link_libraries( client
        LINK_PUBLIC
        easy-tcp)