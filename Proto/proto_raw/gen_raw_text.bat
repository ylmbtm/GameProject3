@echo off
echo ^@echo off
echo.
echo ..\proto_tool\protogen.exe ^^
for %%i in (*.proto) do echo -i:.\%%i ^^
echo -o:..\protoc-gen-cs\Message.cs -ns:Protocol
