add_definitions(-DCloud_EXPORT)
set(SRC_SOURCE 
       coqueue.cpp
       coroutine.cpp
       schedule.cpp
       cloudmessage.cpp
       cloudserver.cpp
       cloudclient.cpp
       loop.cpp
       cloudtcphandle.cpp
       cloudsession.cpp
       )

add_library(cloud STATIC ${SRC_SOURCE})
target_link_libraries(cloud)
add_library(cloud_so SHARED ${SRC_SOURCE})
set_target_properties(cloud_so PROPERTIES OUTPUT_NAME "cloud")

