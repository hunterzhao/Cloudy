set(SERVER_SOURCE main.cpp)
set(CLIENT_SOURCE client_test.cpp)

add_executable(test ${SERVER_SOURCE})
target_link_libraries(test cloud uv)

add_executable(client ${CLIENT_SOURCE})
target_link_libraries(client cloud uv)