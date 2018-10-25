.\proto_tool\protoc.exe -I=./proto_raw --go_out=./protoc-gen-go ./proto_raw/Game_Define.proto
.\proto_tool\protoc.exe -I=./proto_raw --go_out=./protoc-gen-go ./proto_raw/Msg_Bag.proto
.\proto_tool\protoc.exe -I=./proto_raw --go_out=./protoc-gen-go ./proto_raw/Msg_Copy.proto
.\proto_tool\protoc.exe -I=./proto_raw --go_out=./protoc-gen-go ./proto_raw/Msg_Game.proto
.\proto_tool\protoc.exe -I=./proto_raw --go_out=./protoc-gen-go ./proto_raw/Msg_ID.proto
.\proto_tool\protoc.exe -I=./proto_raw --go_out=./protoc-gen-go ./proto_raw/Msg_LoginCltData.proto
.\proto_tool\protoc.exe -I=./proto_raw --go_out=./protoc-gen-go ./proto_raw/Msg_LoginDBData.proto
.\proto_tool\protoc.exe -I=./proto_raw --go_out=./protoc-gen-go ./proto_raw/Msg_Move.proto
.\proto_tool\protoc.exe -I=./proto_raw --go_out=./protoc-gen-go ./proto_raw/Msg_RetCode.proto
rem xcopy  .\protoc-gen-go\*.* ..\Server\Src\Message\ /S /C /Y
pause