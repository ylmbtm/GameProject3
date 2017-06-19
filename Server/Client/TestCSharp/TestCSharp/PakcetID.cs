using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


public enum ConnectionType
{
    TYPE_UNKNOW = 0,
    TYPE_SVR_CENTER, //
    TYPE_SVR_GAME, //
    TYPE_SVR_PROXY,
    TYPE_SVR_LOGIN,
    TYPE_SVR_DATABASE,
    TYPE_CLT_PLAYER
};


public enum CmdHandler_ID
{
    CMDH_SENCE = 1,				//普通的游戏场影处理
    CMDH_SVR_CON,				//服务器间的连接处理
};

public enum Command_ID
{
    CMD_BEGIN_TAG,
    //服务器的连接通知
    CMD_CONNECT_NOTIFY = 1,		//连接通知
    CMD_DISCONNECT_NOTIFY,		//断开连接通知

    //用于服务器间连络的命令
    CMD_SVR_REGISTER_TO_CENTER,		//注册自己到中心服务器
    CMD_SVR_ACTIVE_SERVER_LIST,		//中心服务器来的数据

    CMD_SVR_RUNNING_STATE_REPORT,

    //玩家版本验证命令
    CMD_CHAR_VERIFY_VERSION,	//玩家发版本请求验证
    CMD_CHAR_GMAE_MANAGER,      //GM命令

    //玩家登录命令
    CMD_CHAR_NEW_ACCOUNT_REQ,	//玩家注册账号的命令
    CMD_CHAR_NEW_ACCOUNT_ACK,   //玩家注册账号命令的回包

    CMD_CHAR_NEW_CHAR_REQ,		//新建一个角色
    CMD_CHAR_NEW_CHAR_ACK,		//新建一个角色的回包

    CMD_CHAR_DEL_CHAR_REQ,		//删除一个角色
    CMD_CHAR_DEL_CHAR_ACK,		//删除一个角色的回包

    CMD_CHAR_LOGIN_REQ,			//玩家登录
    CMD_CHAR_LOGIN_ACK,			//登录返回包

    CMD_CHAR_PICK_CHAR_REQ,		//选择角色请求
    CMD_CHAR_PICK_CHAR_ACK,		//选择角色回复

    //玩家进入游戏命令
    CMD_CHAR_ENTER_GAME_REQ,	//玩家进入游戏请求
    CMD_CHAR_ENTER_GAME_ACK,	//玩家进入游戏请求回复

    CMD_CHAR_LEAVE_GAME_REQ,	//玩家退出游戏请求
    CMD_CHAR_LEAVE_GAME_ACK,    //玩家退出游戏请求回复

    //玩家同步数据
    CMD_CHAR_NEARBY_ADD,		//添加周围的对象
    CMD_CHAR_NEARBY_UPDATE,     //更新周围的对象
    CMD_CHAR_NEARBY_REMOVE,		//删除周围的对象

    CMD_CHAR_UPDATE_MYSELF,     //更新自己的数据

    CMD_CHAR_HEART_BEAT_REQ,	//玩家心跳和对时消息回复
    CMD_CHAR_HEART_BEAT_ACK,	//玩家心跳和对时消息

    //下面是数据库服务器的命令
    CMD_DB_NEW_ACCOUNT_REQ,		//新建账号
    CMD_DB_NEW_ACCOUNT_ACK,		//新建账号回复

    CMD_DB_NEW_CHAR_REQ,		//新建角色
    CMD_DB_NEW_CHAR_ACK,		//新建角色回复

    CMD_DB_DEL_CHAR_REQ,		//删除角色
    CMD_DB_DEL_CHAR_ACK,		//删除角色回复

    CMD_DB_LOGIN_REQ,			//登录请求
    CMD_DB_LOGIN_ACK,			//登录请求回复

    CMD_DB_PICK_CHAR_REQ,		//选择角色
    CMD_DB_PICK_CHAR_ACK,		//选择角色回复

    CMD_DB_LOAD_CHAR_REQ,		//加载角色
    CMD_DB_LOAD_CHAR_ACK,		//加载角色回复

    CMD_DB_SAVE_CHAR_REQ,		//保存角色
    CMD_DB_SAVE_CHAR_ACK,		//保存角色回复

    ///////////////////////////////////////////////////
    CMD_SVR_ENTER_SCENE_REQ,	//玩家从世界服发到场景服
    CMD_SVR_ENTER_SCENE_ACK,    //玩家从世界服发到场景服回复

    //GameServer向世界服务器汇报的命令
    CMD_SVR_CREATE_SCENE_REQ,	//创建一个场景(副本)
    CMD_SVR_CREATE_SCENE_ACK,	//创建一个场景(副本)回复
    //
    CMD_SVR_CHAR_WILL_ENTER,	//通知代理服，玩家准备进入

    CMD_CHAR_MOVE_REQ,			//玩家移动请求

    CMD_END_TAG
};

