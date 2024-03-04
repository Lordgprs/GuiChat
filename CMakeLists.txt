cmake_minimum_required(VERSION 3.12)
project(Chat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libchatdb)
add_subdirectory(libchatsettings)
add_subdirectory(client)
add_dependencies(client libchatdb)
add_dependencies(client libchatsettings)
add_subdirectory(server)
add_dependencies(server libchatdb)
add_dependencies(server libchatsettings)
