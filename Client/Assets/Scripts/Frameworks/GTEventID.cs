using UnityEngine;
using System.Collections;

public enum GTEventID : ushort
{
    TYPE_CONNECT_FAIL        = 1001,   //连接失败  

    TYPE_REGISTER_CALLBACK   = 1051,   //注册回调            
    TYPE_LOGIN_CALLBACK      = 1052,   //登陆回调 
    TYPE_CREATEROLE_CALLBACK = 1053,   //创建角色
    TYPE_LOGINGAME_CALLBACK  = 1054,   //登陆游戏服回调
    TYPE_ENTERGAME_CALLBACK  = 1055,   //进入游戏
    TYPE_GETSERVERS_CALLBACK = 1056,   //获取服务器列表回调

    TYPE_CHANGE_MONEY        = 1108,   //金钱改变
    TYPE_CHANGE_ACTION       = 1109,   //行动力改变

    TYPE_CHANGE_HEROLEVEL    = 1210,   //英雄升级回调
    TYPE_CHANGE_HERONAME     = 1211,   //英雄改名回调
    TYPE_CHANGE_HEROHEAD     = 1212,   //英雄更换图像回调
    TYPE_CHANGE_HEROEXP      = 1213,   //英雄增加经验
    TYPE_CHANGE_FIGHTVALUE   = 1214,   //英雄战力改变

    TYPE_SORT_BAG            = 1301,   //背包排序
    TYPE_USE_ITEM            = 1302,   //使用道具
    TYPE_USE_BOX             = 1303,   //使用箱子
    TYPE_DRESS_EQUIP         = 1304,   //穿上装备
    TYPE_UNLOAD_EQUIP        = 1305,   //卸下装备
    TYPE_DRESS_GEM           = 1306,   //穿上宝物
    TYPE_UNLOAD_GEM          = 1307,   //卸下宝物
    TYPE_COMPOSE_CHIP        = 1308,   //合成碎片
    TYPE_STRENGTHEN_EQUIP    = 1309,   //强化装备
    TYPE_ADVANCE_EQUIP       = 1310,   //进阶装备,
    TYPE_UPSTAR_EQUIP        = 1311,   //升星装备
    TYPE_STRENGTHEN_GEM      = 1312,   //强化宝物

    TYPE_PASS_COPY           = 1401,   //过关副本
    TYPE_GET_CHAPTERAWARD    = 1402,   //领取章节奖励
    TYPE_BATTLE_CHECK        = 1403,   //战前检查

    TYPE_MOVE_JOYSTICK       = 1501,   //移动摇杆
    TYPE_STOP_JOYSTICK       = 1502,   //停止摇杆
    TYPE_JUMP                = 1503,   //请求玩家跳跃
    TYPE_CAST_SKILL          = 1504,   //主角释放技能
    TYPE_MOVE_PURSUE         = 1505,   //请求寻路

    TYPE_UD_AVATAR_HP        = 1601,   //刷新主角生命
    TYPE_UD_AVATAR_MP        = 1602,   //刷新主角能量
    TYPE_UD_AVATAR_SP        = 1603,   //刷新主角SP
    TYPE_UD_AVATAR_ATTR      = 1604,   //刷新主角属性
    TYPE_UD_AVATAR_BUFF      = 1605,   //更新主角BUFF
    TYPE_UD_PARTNER_HP       = 1606,   //刷新伙伴生命
    TYPE_UP_BOSS_HP          = 1607,   //刷新BOSS血量

    TYPE_KILL_MONSTER        = 1701,   //杀死一只怪物
    TYPE_AVATAR_DEAD         = 1702,   //主角死亡

    TYPE_START_MOUNT         = 1801,   //骑坐骑
    TYPE_LEAVE_MOUNT         = 1802,   //卸载坐骑
    TYPE_DRESS_MOUNT         = 1803,   //设置出战坐骑
    TYPE_SELECT_MOUNT        = 1804,   //选中坐骑改变

    TYPE_CHARGE_RELICS       = 1901,   //神器充能
    TYPE_UPGRADE_RELICS      = 1902,   //神器升级
    TYPE_BATTLE_RELICS       = 1903,   //神器上阵
    TYPE_UNLOAD_RELICS       = 1904,   //神器卸下

    TYPE_BUY_STORE           = 1905,   //购买商品

    TYPE_UPGRADE_PET         = 2001,   //升级宠物
    TYPE_BATTLE_PET          = 2002,   //上阵宠物
    TYPE_UNLOAD_PET          = 2003,   //收回宠物

    TYPE_CHANGE_PARTNER      = 2004,   //更换伙伴
    TYPE_ADVANCE_PARTNER     = 2005,   //进阶伙伴
    TYPE_UPGRADE_PARTNER     = 2006,   //升级伙伴

    TYPE_GUIDE_ENTER         = 2301,   //新手引导进入
    TYPE_GUIDE_EXIT          = 2302,   //新手引导退出
    TYPE_GUIDE_FIRE          = 2303,   //新手引导触发

    TYPE_SECOND_TICK         = 2401,   //每秒更新

    TYPE_STOP_AUTOTASK       = 2501,   //停止自动任务
    TYPE_SUBMIT_TASK         = 2502,   //提交日常任务

    TYPE_THREAD_TASK_STATE   = 2601,   //更新主线任务状态
    TYPE_BRANCH_TASK_STATE   = 2602,   //更新支线任务状态

    TYPE_GATHER_MINE         = 2701,   //采集回调

    TYPE_MOUSE_BUTTON        = 9001,
    TYPE_MOUSE_BUTTON_UP     = 9002,
    TYPE_MOUSE_BUTTON_DOWN   = 9003,

}