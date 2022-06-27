cmake_minimum_required(VERSION 2.8.9)
project(ProxyServer)
SET(PROJECT_ROOT_PATH ".")
include_directories(./ ../ServerEngine ../ServerEngine/DBInterface ../../Common/protobuf/src ../Message ../LogData ../ServerData ../PublicData ../WatcherClient)
link_directories(../../Common/lib /usr/lib64/mysql)	


#消息
set(PROTO Proto)
FILE(GLOB PROTO "../Message/Msg_Copy.pb.cc" 
                "../Message/Msg_Copy.pb.cc" 
                "../Message/Msg_Base.pb.cc" 
                "../Message/Msg_Base.pb.h"
                "../Message/Msg_Game.pb.cc" 
                "../Message/Msg_Game.pb.h"
                "../Message/Msg_ID.pb.h"
                "../Message/Msg_ID.pb.h")


#header file code
set(HEADER_CODE_FILE HeaderFiles)
FILE(GLOB HEADER_CODE_FILE "./*.h")

#source file code
set(SOURCE_CODE_FILE SourceFiles)
FILE(GLOB SOURCE_CODE_FILE "./*.cpp")

#Watcher Client
set(WATCHER_CLIENT WatcherClient)
FILE(GLOB WATCHER_CLIENT "../WatcherClient/WatcherClient.cpp" "../WatcherClient/WatcherClient.h")

source_group(Proto FILES ${PROTO} )
source_group(WatcherClient FILES ${WATCHER_CLIENT} )
source_group(HeaderFiles FILES ${HEADER_CODE_FILE} )
source_group(SourceFiles FILES ${SOURCE_CODE_FILE} )

# Target
add_executable(ProxyServer
    ${PROTO}    
    ${WATCHER_CLIENT}   
    ${HEADER_CODE_FILE} 
    ${SOURCE_CODE_FILE} 
)

if(MSVC)
    target_link_libraries(ProxyServer ServerEngine protobuf ws2_32)
    ADD_DEFINITIONS(-D_CONSOLE -D_CRT_SECURE_NO_WARNINGS)
else()
    target_link_libraries(ProxyServer ServerEngine protobuf pthread)
    ADD_DEFINITIONS(-D_CONSOLE -D_CRT_SECURE_NO_WARNINGS)
endif()


if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -pthread -std=c++11")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -pthread -std=c++11")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -O3 -std=c++11")
endif()

