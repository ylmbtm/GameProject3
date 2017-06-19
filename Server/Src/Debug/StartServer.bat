@echo off
color 0A
echo.
echo            ================================
echo             请选择要进行的操作，然后按回车
echo            ================================
echo.
echo              1.启动全部服务器
echo.     
echo.             2.关闭全部服务器
echo.
echo.             3.退出命令窗口
echo.
:cho
set choice=
set /p choice=          请选择:
IF NOT "%choice%"=="" SET choice=%choice:~0,1%
if /i "%choice%"=="1" start /MIN ./CenterServer.exe & start /MIN ./LoginServer.exe & start /MIN ./ProxyServer.exe & start /MIN ./DBServer.exe & start /MIN ./GameServer.exe
if /i "%choice%"=="2" taskkill /im CenterServer.exe & taskkill /im LoginServer.exe & taskkill /im ProxyServer.exe & taskkill /im DBServer.exe & taskkill /im GameServer.exe
if /i "%choice%"=="3" exit
goto cho


