cmake_minimum_required(VERSION 2.8.9)
project(GameServer)
SET(PROJECT_ROOT_PATH ".")
add_definitions(-std=c++11)
add_definitions(-w)
include_directories(./ ../StaticData ../../Common/Sqlite ../WatcherClient ../PublicData ../ServerEngine  ../ServerEngine/DBInterface ../../Common/protobuf/src ../../Common/Lua ../Message ../LogData ../ServerData ../WatcherClient)
link_directories(../../Common/lib /usr/lib64/mysql)	


#消息
set(PROTO Proto)
FILE(GLOB PROTO "../Message/Game_Define.pb.cc" 
				"../Message/Game_Define.pb.h"
				"../Message/Msg_Base.pb.cc"
				"../Message/Msg_Base.pb.h" 
				"../Message/Msg_Copy.pb.cc" 
				"../Message/Msg_Copy.pb.h"
				"../Message/Msg_Game.pb.cc" 
				"../Message/Msg_Game.pb.h"
				"../Message/Msg_LoginCltData.pb.cc" 
				"../Message/Msg_LoginCltData.pb.h"
				"../Message/Msg_Move.pb.cc" 
				"../Message/Msg_Move.pb.h")


#游戏逻辑相关
set(SCENE_LOGICAL SceneLogic)
FILE(GLOB SCENE_LOGICAL "./SceneLogic/SceneLogic_Arena.cpp"
					"./SceneLogic/SceneLogic_Arena.h"
					"./SceneLogic/SceneLogic_Base.cpp"
					"./SceneLogic/SceneLogic_Base.h"
					"./SceneLogic/SceneLogic_City.cpp"
					"./SceneLogic/SceneLogic_City.h"
					"./SceneLogic/SceneLogic_None.cpp"
					"./SceneLogic/SceneLogic_None.h"
					"./SceneLogic/SceneLogic_Normal.cpp"
					"./SceneLogic/SceneLogic_Normal.h"
					"./SceneLogic/SceneLogic_Team.cpp"
					"./SceneLogic/SceneLogic_Team.h"
					"./SceneLogic/WinCondition.cpp"
					"./SceneLogic/WinCondition.h")

#场景对象
set(SCENE_OBJECT GameObject)
FILE(GLOB SCENE_OBJECT "./GameObject/BuffObject.cpp"
					"./GameObject/BuffObject.h"
					"./GameObject/BulletObject.cpp"
					"./GameObject/BulletObject.h"
					"./GameObject/MapObject.cpp"
					"./GameObject/MapObject.h"
					"./GameObject/SceneObject.cpp"
					"./GameObject/SceneObject.h"
					"./GameObject/SkillObject.cpp"
					"./GameObject/SkillObject.h")

#Staic Data
set(STATIC_DATA StaticData)
FILE(GLOB STATIC_DATA "../StaticData/*.cpp" "../StaticData/*.h")

#Watcher Client
set(WATCHER_CLIENT WatcherClient)
FILE(GLOB WATCHER_CLIENT "../WatcherClient/WatcherClient.cpp" "../WatcherClient/WatcherClient.h")


#header file code
set(HEADER_CODE_FILE HeaderFiles)
FILE(GLOB HEADER_CODE_FILE "./*.h")

#source file code
set(SOURCE_CODE_FILE SourceFiles)
FILE(GLOB SOURCE_CODE_FILE  "./*.cpp")

source_group(Proto FILES ${PROTO} )
source_group(SceneLogic FILES ${SCENE_LOGICAL} )
source_group(GameObject FILES ${SCENE_OBJECT} )
source_group(StaticData FILES ${STATIC_DATA} )
source_group(WatcherClient FILES ${WATCHER_CLIENT} )
source_group(HeaderFiles FILES ${HEADER_CODE_FILE} )
source_group(SourceFiles FILES ${SOURCE_CODE_FILE} )



# Target
add_executable(GameServer 
	${SCENE_LOGICAL}	
	${SCENE_OBJECT}	
	${STATIC_DATA}	
	${WATCHER_CLIENT}	
	${PROTO}	
	${HEADER_CODE_FILE}	
	${SOURCE_CODE_FILE}	
)


if(MSVC)
	target_link_libraries(GameServer ServerEngine protobuf sqlite3 lua  ws2_32)
	ADD_DEFINITIONS(-D_CONSOLE -D_CRT_SECURE_NO_WARNINGS)
else()
	target_link_libraries(GameServer ServerEngine protobuf sqlite3 lua  pthread )
	ADD_DEFINITIONS(-D_CONSOLE -D_CRT_SECURE_NO_WARNINGS)
endif()


if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -pthread -std=c++11")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -pthread -std=c++11")
endif()

