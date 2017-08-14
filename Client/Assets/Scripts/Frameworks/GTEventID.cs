using UnityEngine;
using System.Collections;

public enum GTEventID : ushort
{
    TYPE_NETWORK_CONNECT             = 1000,   //准备连接
    TYPE_NETWORK_CONNECT_FAIL        = 1001,   //连接失败  
    TYPE_NETWORK_CONNECT_SUCCESS     = 1002,   //连接成功

    TYPE_LOGIN_ACCOUNT_REGISTER      = 1051,   //注册回调            
    TYPE_LOGIN_ACCOUNT_LOGIN         = 1052,   //登陆回调 
    TYPE_LOGIN_ROLECRATE             = 1053,   //创建角色
    TYPE_LOGIN_SELECTSERVER          = 1054,   //登陆游戏服回调
    TYPE_LOGIN_ROLELOGIN             = 1055,   //进入游戏
    TYPE_LOGIN_GETSERVERLIST         = 1056,   //获取服务器列表回调

    TYPE_CHANGE_MONEY                = 1108,   //金钱改变
    TYPE_CHANGE_ACTION               = 1109,   //行动力改变
    TYPE_CHANGE_HEROLEVEL            = 1210,   //英雄升级回调
    TYPE_CHANGE_HERONAME             = 1211,   //英雄改名回调
    TYPE_CHANGE_HEROHEAD             = 1212,   //英雄更换图像回调
    TYPE_CHANGE_HEROEXP              = 1213,   //英雄增加经验
    TYPE_CHANGE_FIGHTVALUE           = 1214,   //英雄战力改变

    TYPE_BAG_SORT_BAG                = 1301,   //背包排序
    TYPE_BAG_USE_ITEM                = 1302,   //使用道具
    TYPE_BAG_USE_BOX                 = 1303,   //使用箱子
    TYPE_BAG_DRESS_EQUIP             = 1304,   //穿上装备
    TYPE_BAG_UNLOAD_EQUIP            = 1305,   //卸下装备
    TYPE_BAG_DRESS_GEM               = 1306,   //穿上宝物
    TYPE_BAG_UNLOAD_GEM              = 1307,   //卸下宝物
    TYPE_BAG_COMPOSE_CHIP            = 1308,   //合成碎片
    TYPE_BAG_STRENGTHEN_EQUIP        = 1309,   //强化装备
    TYPE_BAG_ADVANCE_EQUIP           = 1310,   //进阶装备,
    TYPE_BAG_UPSTAR_EQUIP            = 1311,   //升星装备
    TYPE_BAG_STRENGTHEN_GEM          = 1312,   //强化宝物

    TYPE_ECTYPE_PASSCOPY             = 1401,   //过关副本
    TYPE_ECTYPE_CHAPTERAWARD         = 1402,   //领取章节奖励
    TYPE_ECTYPE_BATTLECHECK          = 1403,   //战前检查

    TYPE_ACTOR_MOVE_JOYSTICK         = 1501,   //移动摇杆
    TYPE_ACTOR_STOP_JOYSTICK         = 1502,   //停止摇杆
    TYPE_ACTOR_JUMP                  = 1503,   //请求玩家跳跃
    TYPE_ACTOR_CAST_SKILL            = 1504,   //主角释放技能
    TYPE_ACTOR_MOVE_PURSUE           = 1505,   //请求寻路
    TYPE_ACTOR_KILL_MONSTER          = 1506,   //杀死一只怪物
    TYPE_ACTOR_AVATAR_DEAD           = 1507,   //主角死亡

    TYPE_UPDATE_AVATAR_HP            = 1601,   //刷新主角生命
    TYPE_UPDATE_AVATAR_MP            = 1602,   //刷新主角能量
    TYPE_UPDATE_AVATAR_SP            = 1603,   //刷新主角SP
    TYPE_UPDATE_AVATAR_ATTR          = 1604,   //刷新主角属性
    TYPE_UPDATE_AVATAR_BUFF          = 1605,   //更新主角BUFF
    TYPE_UPDATE_PARTNER_HP           = 1606,   //刷新伙伴生命
    TYPE_UPDATE_BOSS_HP              = 1607,   //刷新BOSS血量

    TYPE_MOUNT_STARTMOUNT            = 1801,   //骑坐骑
    TYPE_MOUNT_LEAVEMOUNT            = 1802,   //卸载坐骑
    TYPE_MOUNT_DRESSMOUNT            = 1803,   //设置出战坐骑
    TYPE_MOUNT_SELECTMOUNT           = 1804,   //选中坐骑改变

    TYPE_RELICS_CHARGE               = 1901,   //神器充能
    TYPE_RELICS_UPGRADE              = 1902,   //神器升级
    TYPE_RELICS_BATTLE               = 1903,   //神器上阵
    TYPE_RELICS_UNLOAD               = 1904,   //神器卸下

    TYPE_STORE_BUYSUCCESS            = 1905,   //购买商品

    TYPE_PET_UPGRADE                 = 2001,   //升级宠物
    TYPE_PET_BATTLE                  = 2002,   //上阵宠物
    TYPE_PET_UNLOAD                  = 2003,   //收回宠物

    TYPE_PARTNER_CHANGE              = 2004,   //更换伙伴
    TYPE_PARTNER_ADVANCE             = 2005,   //进阶伙伴
    TYPE_PARTNER_UPGRADE             = 2006,   //升级伙伴

    TYPE_SECOND_TICK                 = 2401,   //每秒更新
    TYPE_FORCE_STOPJOYSTICK          = 2402,   //暴力禁止摇杆

    TYPE_TASK_STOP_AUTOTASK          = 2501,   //停止自动任务
    TYPE_TASK_SUBMIT_TASK            = 2502,   //提交日常任务
    TYPE_TASK_THREAD_STATE           = 2601,   //更新主线任务状态
    TYPE_TASK_BRANCH_STATE           = 2602,   //更新支线任务状态

    TYPE_MAP_ENTERWORLD              = 2701,   //进入世界

    TYPE_MOUSE_BUTTON                = 9001,
    TYPE_MOUSE_BUTTON_UP             = 9002,
    TYPE_MOUSE_BUTTON_DOWN           = 9003,
}