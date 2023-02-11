
project(ClientServer_tests)

enable_testing()

# Get Google test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin/tests)

# Set up include directories
include_directories(${Boost_INCLUDE_DIRS})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/shared
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/client
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/server
	${CMAKE_CURRENT_SOURCE_DIR}/../include
)

set(SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/../src/shared/seeds.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/seeds_test.cpp
    )

add_library(tests STATIC
        seeds_test.cpp
    )   

# Add the client executable
add_executable(test_seeds ${SOURCES})
# Add the server executable
#add_executable(${PROJECT_NAME} seeds_test.cpp)

target_link_libraries(test_seeds ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} )

#target_link_libraries(seeds_test
#        PRIVATE
#            GTest::GTest
#            seeds
#    )

add_test(test_seeds test_seeds)

