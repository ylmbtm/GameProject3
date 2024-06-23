Game Engine Frame
===============
跨平台的多进程游戏服务器框架，网络层分别使用SocketApi, Boost Asio, Libuv三种方式实现， 
框架内使用共享内存，无锁队列，对象池，内存池来提高服务器性能。

还有一个不断完善的Demo客户端，游戏包含大量完整资源，坐骑，宠物，伙伴，装备, 这些均可上阵和穿戴, 并可进入副本战斗，多人玩法也己实现,
Demo客户端地址:https://github.com/ylmbtm/DemoClient

 #### 服务器部署启动说明
 ##### 1.编译
	Windows: 用VisualStudio2017以上版本打开打开解决方案直接进行编译。
	Linux  : 执行buildall.sh脚本进行编译  
##### 2.安装mysql
	在机器上安装mysql数据,推荐版mysql5.7版, 安装完成之后，如果不想改配制文件,新建用户root,密码设为123456 (配制文件默认如此)。
	然后工程里找到db_create.sql文件，在数据库中执行这个文件，这样完成数据库表就创建。
##### 3.启动服务器
	在完成上面两步这后， 在\Server目录下有一个StartServer.bat文件，执行bat文件， 然后按提示操作。

#### 服务器角色说明
	登录服务器(LoginServer)  说明 : 接受玩家的登录连接， 处理登录请求消息。
	账号服务器(AccountServer)说明 : 处理账号登录的验证，新账号的创建，账号的数据库保存。
	中心服务器(CenterServer) 说明 : 用于跨服活动， 跨服战需求。
	逻辑服务器(LogicServer)  说明 : 处理玩家角色的逻辑数据，处理玩家角色的一般逻辑功能。
	游戏服务器(GameServer)   说明 : 处理玩家移动同步，技能，buff等作战功能。
	数据服务器(DBServer)     说明 : 作为逻辑服和mysql数据库之间的代理服务器，负责定期将玩家的数据写入数据库。
	网关服务器(ProxyServer)  说明 : 作为客户端和逻辑服，战场服之间的中转服务器，主要负责消息的转发。
	日志服务器(LogServer)    说明 : 日志服务器 主要负责逻辑服运营日志的写入mysql数据库。
	监视服务器(WatchServer)  说明 : 主要负责接受WEB后台的控制命令， 控制服务器。

#### 文档与教程
1.群友 Binaryhobart(450282550) 提供的视频搭建教程 [教程地址](https://www.bilibili.com/video/BV1k5411s7Vf?from=search&seid=3304544258866101487)

gitee地址: https://gitee.com/ylmbtm/GameProject3

交流QQ群 : 962315897
公众号:![public1](https://github.com/ylmbtm/resource/blob/master/20210818175029.png)
![public1](https://wx1.sinaimg.cn/mw2000/002dp2Ulgy1gvofyue87sj6065065wf602.jpg)


#### 体验客户端效果(群文件中有全部客户端代码及资源)
![login1](https://github.com/ylmbtm/resource/blob/master/01.png)
![login2](https://github.com/ylmbtm/resource/blob/master/02.png)
![login3](https://github.com/ylmbtm/resource/blob/master/03.png)
![login4](https://github.com/ylmbtm/resource/blob/master/04.png)


