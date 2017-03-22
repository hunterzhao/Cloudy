set(SERVER_SOURCE main.cpp)
set(CLIENT_SOURCE client_test.cpp)

add_executable(server ${SERVER_SOURCE})
target_link_libraries(server cloud uv )

add_executable(client ${CLIENT_SOURCE})
target_link_libraries(client cloud uv)