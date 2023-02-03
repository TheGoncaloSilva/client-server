cmake_minimum_required(VERSION 3.10)

project(client-server)

# Find Boost library
find_package(Boost REQUIRED COMPONENTS system)

# Set up include directories
include_directories(${Boost_INCLUDE_DIRS})



set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/receiver/main
    ${CMAKE_CURRENT_SOURCE_DIR}/packet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/client.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

# Add the client executable
add_executable(client ${SOURCES})

# Add the server executable
add_executable(server ${SOURCES})

# Link the Boost libraries to the client executable
target_link_libraries(client ${Boost_LIBRARIES})

# Link the Boost libraries to the server executable
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})