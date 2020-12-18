"C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\Common7\IDE\devenv.com" ./Server.sln /Build "Debug|x64" > ./BuildResult.txt
IF %ERRORLEVEL% NEQ 0 pause 