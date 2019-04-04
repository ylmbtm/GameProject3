cd proto_raw
call gen_raw_text.bat>.\gen_cs_real.bat
call gen_cs_real.bat
del gen_cs_real.bat
cd ..
xcopy  .\protoc-gen-cs\*.* ..\Client\Assets\Scripts\Network\ /S /C /Y
xcopy  .\protoc-gen-cs\*.* ..\Client_2\Assets\Scripts\Network\ /S /C /Y
pause