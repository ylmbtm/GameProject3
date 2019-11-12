Game Engine Frame
===============
跨平台的多进程游戏服务器框架，网络层分别使用SocketApi(ServerEgine), Boost Asio(ServerEgine_asio), Libuv(ServerEgine_libuv)三种方式实现， 框架内使用共享内存，无锁队列，对象池，内存池来提高服务器性能。

还包含一个不断完善的Unity3D游戏Demo，游戏包含大量完整资源，坐骑，宠物，伙伴，装备, 这些均己实现上阵和穿戴, 并可进入副本战斗，多人玩法也己实现，持续开发中。

email : ylmbtm@163.com   QQ群 : 45139128 

####服务器编译启动说明
#####1.编译(windows)
	在群里下载lib.7z并解压, 把解压出来的lib文件拷贝到目录\Server\Common\lib下面，然后用VisualStudio2017打开工程进行编译。
#####2.安装mysql
	在机器上安装mysql数据，安装完成之后，新建用户root,密码设置为123456 (这样省得改配制文件，配制文件默认如此)。
	然后在群文件里找到并下载db_create.sql文件，在数据库中执行这个文件，这样数据库表就创建完成。
#####3.启动服务器
	在完成上面两步这后， 在\Server目录下有一个StartServer.bat文件，执行bat文件， 然后按提示操作。

####客户端编译启动说明
#####1.安装unity
	在群里下载UnityDownloadAssistant-2017.1.0f3.exe文件， 然后执行文件完成unity2017的安装。
#####2.编译客户端
	在群里下载Client2019_09_29.7z并解压到本地任意的目录即可,然后用unity2017打开客户端工程,即会自动编译。
#####3.启动客户端
	打开启动场景 \Assets\Scenes\Launcher.unity 直接启动即可。
	如果客户端与服务器不在同一台机器上，可在inspector面板中将loginip指向服务器所在的ip地址。

####服务器角色说明
	登录服务器(LoginServer)  说明 : 接受玩家的登录连接， 处理登录请求消息。
	账号服务器(AccountServer)说明 : 处理账号登录的验证，新账号的创建，账号的数据库保存。
	中心服务器(CenterServer) 说明 : 用于跨服活动， 跨服战需求。
	逻辑服务器(LogicServer)  说明 : 处理玩家角色的逻辑数据，处理玩家角色的一般逻辑功能。
	游戏服务器(GameServer)   说明 : 处理玩家移动同步，技能，buff等作战功能。
	数据服务器(DBServer)     说明 : 作为逻辑服和mysql数据库之间的代理服务器，负责定期将玩家的数据写入数据库。
	网关服务器(ProxyServer)  说明 : 作为客户端和逻辑服，战场服之间的中转服务器，主要负责消息的转发。
	日志服务器(LogServer)    说明 : 日志服务器 主要负责逻辑服运营日志的写入mysql数据库。
	监视服务器(WatchServer)  说明 : 主要负责接受WEB后台的控制命令， 控制服务器。

####体验客户端效果(加群可以获取全部客户端代码及资源)

![login1](https://wx2.sinaimg.cn/mw690/79045675gy1fvdn1uoqa8j20pg0etabd.jpg)
![login2](https://wx2.sinaimg.cn/mw690/79045675gy1fvdn1uqpclj20wg0fxgn8.jpg)
![login3](https://wx4.sinaimg.cn/mw690/79045675gy1fvdn1uqq84j20wd0fydi0.jpg)
![login4](https://wx3.sinaimg.cn/mw690/79045675gy1fvdn1urjzaj20wd0gedia.jpg)
![login5](https://wx2.sinaimg.cn/mw690/79045675gy1fvdn1uq19vj20wh0fwdhn.jpg)
