cmake_minimum_required(VERSION 3.10)

# The version can also be specified by using VERSION x.x.x
project(ClientServerApp)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
