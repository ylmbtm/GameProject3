"C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\Common7\IDE\devenv.com" ./Server.sln /Build "Release|x64" > ./BuildResult.txt
IF %ERRORLEVEL% NEQ 0 pause 