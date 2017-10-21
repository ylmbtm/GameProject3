.\proto_tool\protoc.exe -I=./proto_raw --cpp_out=./protoc-gen-cpp ./proto_raw/Game_Define.proto
.\proto_tool\protoc.exe -I=./proto_raw --cpp_out=./protoc-gen-cpp ./proto_raw/Msg_Bag.proto
.\proto_tool\protoc.exe -I=./proto_raw --cpp_out=./protoc-gen-cpp ./proto_raw/Msg_Copy.proto
.\proto_tool\protoc.exe -I=./proto_raw --cpp_out=./protoc-gen-cpp ./proto_raw/Msg_Game.proto
.\proto_tool\protoc.exe -I=./proto_raw --cpp_out=./protoc-gen-cpp ./proto_raw/Msg_ID.proto
.\proto_tool\protoc.exe -I=./proto_raw --cpp_out=./protoc-gen-cpp ./proto_raw/Msg_LoginCltData.proto
.\proto_tool\protoc.exe -I=./proto_raw --cpp_out=./protoc-gen-cpp ./proto_raw/Msg_LoginDBData.proto
.\proto_tool\protoc.exe -I=./proto_raw --cpp_out=./protoc-gen-cpp ./proto_raw/Msg_Move.proto
.\proto_tool\protoc.exe -I=./proto_raw --cpp_out=./protoc-gen-cpp ./proto_raw/Msg_RetCode.proto
xcopy  .\protoc-gen-cpp\*.* ..\Server\Src\Message\ /S /C /Y
pause