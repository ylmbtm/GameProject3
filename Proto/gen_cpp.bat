.\proto_tool\protoc.exe -I=./proto_raw --cpp_out=./protoc-gen-cpp ./proto_raw/*.proto
xcopy  .\protoc-gen-cpp\*.* ..\Server\Src\Message\ /S /C /Y
pause