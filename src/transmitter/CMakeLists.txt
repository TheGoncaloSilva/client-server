cmake_minimum_required(VERSION 3.10)

project(server VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#if ( CMAKE_COMPILER_IS_GNUCC )
#	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -ggdb -Wall -D_FILE_OFFSET_BITS=64 -Werror -fmax-errors=1")
#endif()
find_package(Boost COMPONENTS program_options log log_setup REQUIRED)
# Add -Werror for unused variables errors
set(CMAKE_CXX_FLAGS "-Wall -Wextra -fPIC -fopenmp  -DBOOST_LOG_DYN_LINK    -fmax-errors=1")
set(PROJECT_LIB ${PROJECT_LIB} ${Boost_LOG_LIBRARY} ${Boost_LOG_SETUP_LIBRARY})

set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -L${PROJECT_ROOT_DIR}/client-server/lib")

execute_process ( COMMAND dirname ${CMAKE_SOURCE_DIR} 
            OUTPUT_VARIABLE PROJECT_ROOT_DIR
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_ROOT_DIR}/client-server/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_ROOT_DIR}/client-server/bin)

# Include CMake variables in source code
#configure_file(client.h client.h)

# Set up include directories
include_directories(${PROJECT_ROOT_DIR}/client-server/include)

include_directories(${Boost_INCLUDE_DIRS})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../shared
)

# Do this with a foreach in shared
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../shared/communication.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../shared/logging.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../shared/seeds.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main_server.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/server.cpp
)

# Add the server executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Find Boost library
find_package(Boost REQUIRED COMPONENTS system)
#find_package(Boost REQUIRED system thread)

# Link the Boost libraries to the server executable
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME} -Wl,--start-group  ${PROJECT_LIB} -Wl,--end-group)

enable_testing()