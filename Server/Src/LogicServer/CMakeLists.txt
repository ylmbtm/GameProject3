cmake_minimum_required(VERSION 2.8.9)
project(LogicServer)
SET(PROJECT_ROOT_PATH ".")
add_definitions(-std=c++11)
add_definitions(-w)
include_directories(./ ../ServerEngine ../ServerEngine/DBInterface ../PublicData ../StaticData ../../Common/SQLite ../ServerData ../../Common/Mysql ../../Common/protobuf/src ../Message ../LogData ../ServerData ../WatcherClient)
link_directories(../../Common/lib /usr/lib64/mysql)	


#消息
set(PROTO Proto)
FILE(GLOB PROTO "../Message/Game_Define.pb.cc" 
				"../Message/Game_Define.pb.h"
				"../Message/Msg_Account.pb.cc" 
				"../Message/Msg_Account.pb.h"
				"../Message/Msg_Base.pb.cc" 
				"../Message/Msg_Base.pb.h"
				"../Message/Msg_Copy.pb.cc" 
				"../Message/Msg_Copy.pb.h"
				"../Message/Msg_Game.pb.cc" 
				"../Message/Msg_Game.pb.h"
				"../Message/Msg_ID.pb.cc" 
				"../Message/Msg_ID.pb.h"
				"../Message/Msg_LoginCltData.pb.cc" 
				"../Message/Msg_LoginCltData.pb.h"
				"../Message/Msg_LoginDBData.pb.cc" 
				"../Message/Msg_LoginDBData.pb.h"
				"../Message/Msg_Payment.pb.cc" 
				"../Message/Msg_Payment.pb.h"
				"../Message/Msg_RetCode.pb.cc" 
				"../Message/Msg_RetCode.pb.h"
				"../Message/Msg_Role.pb.cc" 
				"../Message/Msg_Role.pb.h")


#Staic Data
set(STATIC_DATA StaticData)
FILE(GLOB STATIC_DATA "../StaticData/*.cpp" "../StaticData/*.h")

#header file code
set(HEADER_CODE_FILE HeaderFiles)
FILE(GLOB HEADER_CODE_FILE "./*.h")

#Watcher Client
set(WATCHER_CLIENT WatcherClient)
FILE(GLOB WATCHER_CLIENT "../WatcherClient/WatcherClient.cpp" "../WatcherClient/WatcherClient.h")


#source file code
set(SOURCE_CODE_FILE SourceFiles)
FILE(GLOB SOURCE_CODE_FILE "./*.cpp")


source_group(Proto FILES ${PROTO} )
source_group(StaticData FILES ${STATIC_DATA} )
source_group(WatcherClient FILES ${WATCHER_CLIENT} )
source_group(HeaderFiles FILES ${HEADER_CODE_FILE} )
source_group(SourceFiles FILES ${SOURCE_CODE_FILE} )



# Target
add_executable(LogicServer
    ${PROTO}	
	${STATIC_DATA}	
	${WATCHER_CLIENT}	
	${HEADER_CODE_FILE}	
	${SOURCE_CODE_FILE}	
)


if(MSVC)
	target_link_libraries(LogicServer ServerEngine protobuf libmysql ws2_32 sqlite3)
	ADD_DEFINITIONS(-D_CONSOLE -D_CRT_SECURE_NO_WARNINGS)
else()
	target_link_libraries(LogicServer ServerEngine protobuf mysqlclient pthread sqlite3)
	ADD_DEFINITIONS(-D_CONSOLE -D_CRT_SECURE_NO_WARNINGS)
endif()


if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -pthread -std=c++11")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -pthread -std=c++11")
endif()

