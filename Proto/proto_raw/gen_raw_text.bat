@echo off
echo ^@echo off
echo.
echo protogen.exe ^^
for %%i in (*.proto) do echo -i:.\proto_raw\%%i ^^
echo -o:.\protoc-gen-cs\Message.cs -ns:Protocol
