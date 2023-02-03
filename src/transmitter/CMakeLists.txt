cmake_minimum_required(VERSION 3.10)

project(server)

#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED True)

execute_process ( COMMAND dirname ${CMAKE_SOURCE_DIR} 
            OUTPUT_VARIABLE PROJECT_ROOT_DIR
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_ROOT_DIR}/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT_DIR}/bin)

# Set up include directories
include_directories(${Boost_INCLUDE_DIRS})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../shared
)

# Do this with a foreach in shared
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/communication.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main_server.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/server.cpp
)

# Add the server executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Find Boost library
#find_package(Boost REQUIRED COMPONENTS system)
#find_package(Boost REQUIRED system thread)

# Link the Boost libraries to the server executable
target_link_libraries(${PROJECT_NAME} ${PROJECT_ROOT_DIR}/lib/boost_1_81_0)
