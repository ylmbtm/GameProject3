:start
@echo off
color 0A
echo. ╔═════════════════╗
echo. ║  请选择要进行的操作，然后按回车  ║
echo. ║═════════════════║
echo. ║                                  ║
echo. ║     1.启动全部服务器(DEBUG)      ║
echo. ║                                  ║
echo. ║     2.启动全部服务器(RELEASE)    ║
echo. ║                                  ║
echo. ║     3.关闭全部服务器             ║
echo. ║                                  ║
echo. ║     4.启动压力测试工具           ║
echo. ║                                  ║
echo. ║     5.清除屏幕                   ║
echo. ║                                  ║
echo. ║     6.退出命令窗口               ║
echo. ╚═════════════════╝
echo.             
set DebugDir=%cd%\Src\Bin\Debug64\
set ReleaseDir=%cd%\Src\Bin\Release64\
set PressDir=%cd%\Client\\PressureTest\Debug\
:cho
set choice=
set /p choice=          请选择:
IF NOT "%choice%"=="" SET choice=%choice:~0,1%
if /i "%choice%"=="1" start /D %DebugDir%  /MIN  StartServer.bat
if /i "%choice%"=="2" start /D %ReleaseDir% /MIN StartServer.bat
if /i "%choice%"=="3" taskkill /im LoginServer.exe & taskkill /im ProxyServer.exe & taskkill /im DBServer.exe & taskkill /im GameServer.exe & taskkill /im LogServer.exe & taskkill /im LogicServer.exe & taskkill /im PressureTest.exe & taskkill /im AccountServer.exe /im LoginServer_d.exe & taskkill /im ProxyServer_d.exe & taskkill /im DBServer_d.exe & taskkill /im GameServer_d.exe & taskkill /im LogServer_d.exe & taskkill /im LogicServer_d.exe & taskkill /im PressureTest.exe & taskkill /im AccountServer_d.exe
if /i "%choice%"=="4" start /D %ClientDir% %ClientDir%PressureTest.exe
if /i "%choice%"=="5" cls & goto start
if /i "%choice%"=="6" exit
if /i "%choice%"=="7" type %DebugDir%ServerCfg.ini
echo.
goto cho


