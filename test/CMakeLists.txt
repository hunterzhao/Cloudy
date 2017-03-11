set(TEST_SOURCE main.cpp)

add_executable(test ${TEST_SOURCE})
target_link_libraries(test cloud)