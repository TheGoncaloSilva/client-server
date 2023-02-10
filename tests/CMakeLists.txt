
project(ClientServer_tests)

# Get google test
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

include_directories(${PROJECT_ROOT_DIR}/client-server/include)

set(SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/seeds_test.cpp
    )

add_library(tests STATIC
        seeds_test.cpp
    )   

add_executable(seeds_test ${CMAKE_CURRENT_SOURCE_DIR}/seeds_test.cpp)
target_link_libraries(seeds_test gtest ../include/seeds)

#target_link_libraries(seeds_test
#        PRIVATE
#            GTest::GTest
#            seeds
#    )

add_test(seeds_test seeds_test)

