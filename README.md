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




Game Engine Frame

Cross-platform multi-process game server framework , the network layer using SocketApi, Boost Asio, Libuv three ways to implement , respectively. 
The network layer is implemented using SocketApi, Boost Asio, and Libuv. The framework uses shared memory, lock-free queues, object pools, and memory pools to improve server performance.

There is also a constantly improving Demo client, the game contains a large number of complete resources, mounts, pets, partners, equipment, which can be mounted and worn, and can enter the copy of the battle, multiplayer gameplay has also been realized.
Demo Client Address: https://github.com/ylmbtm/DemoClient

#### Server Deployment Startup Instructions
#### 1.Compilation
•Windows: Open the solution in Visual Studio 2017 or later and compile directly.
•Linux: Execute the buildall.sh script to compile.
#### 2. Install MySQL
Install MySQL on the machine, version 5.7 recommended. Post-installation, to avoid configuration file changes, create a user 'root' with the password '123456' (as configured by default). Then locate the db_create.sql file in the project and run this file in the database to create the tables.
#### 3. Start the Server
After completing the steps above, there is a StartServer.bat file in the \Server directory. Run this batch file and follow the prompts.

#### Server Role Descriptions
•Login Server (LoginServer): Manages player login connections and login request processing.
•Account Server (AccountServer): Manages account login verification, new account creation, and account data storage in the database.
•Center Server (CenterServer): Facilitates cross-server activities and requirements for cross-server combat.
•Logic Server (LogicServer): Manages player character logic data and general logic functions.
•Game Server (GameServer): Handles player movement synchronization, skills, buffs, and other combat functions.
•Database Server (DBServer): Acts as a proxy between the logic server and MySQL database, managing periodic data writes to the database.
•Proxy Server (ProxyServer): Functions as a mediator between clients, logic servers, and battlefield servers, primarily responsible for message forwarding.
•Log Server (LogServer): Primarily responsible for writing operation logs from the logic server to the MySQL database.
•Watch Server (WatchServer): Manages commands from the WEB backend, controlling the server.

#### Documentation and Tutorials
1. Binaryhobart(450282550) provided a video tutorial on how to build the gitee [tutorialaddress]
(https://www.bilibili.com/video/BV1k5411s7Vf?from=search& seid=3304544258866101487)
gitee address: https://gitee.com/ylmbtm/GameProject3
QQ group : 962315897
Public number:! [public1] (https://github.com/ylmbtm/resource/blob/master/20210818175029.png)
! [public1] (https://wx1.sinaimg.cn/mw2000/002dp2Ulgy1gvofyue87sj6065065wf602.jpg)

#### Experience the client effect (group file has all the client code and resources)
! [login1](https://github.com/ylmbtm/resource/blob/master/01.png)
! [login2](https://github.com/ylmbtm/resource/blob/master/02.png)
! [login3](https://github.com/ylmbtm/resource/blob/master/03.png)
! [login4](https://github.com/ylmbtm/resource/blob/master/04.png)
