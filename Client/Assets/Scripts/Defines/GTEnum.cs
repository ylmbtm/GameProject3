using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

public enum ELanguage
{
    Chinese,
    English
}

public enum EAction
{
    None   = 0,
    Manual = 1,
    Energy = 2,
}

public enum EPosType
{
    BagItem,
    BagGem,
    BagFashion,
    BagRune,
    BagChip,
    RoleEquip,
    RoleGem,
    RoleRune,
    RoleFashion,
    RoleMoney,
    RoleAction,
    Empty,
}

public enum EDataKeyType
{
    Id,
    Instance,
    Pos,
    Carrer,
}

public enum EMoney
{
    Gold    = 1,
    Diam    = 2,
    Crystal = 3,
    Glyph   = 4,
    Honor   = 5,
    Hoon    = 6,
    Fame    = 7,
    Mete    = 8,
    Apple   = 9,
    Soul    = 10,
    Goul    = 11,
    Poul    = 12
}

public enum EItemType
{
    EQUIP    = 1,
    DRUG     = 2,
    MAT      = 3,
    BOX      = 4,
    KEY      = 5,
    CHIP     = 6,
    GEM      = 7,
    FASHION  = 8,
    MONEY    = 9,
    ACTION   =10,
    EXP      =11,
    RUNE     =12,
    PETSOUL  =13,  
    TASK     =14,
}

public enum EBagType
{
    NONE    = 0,
    ITEM    = 1,
    GEM     = 2,
    FASHION = 3,
    CHIP    = 4,
    RUNE    = 5,
    TASK    = 6,
    SOUL    = 7,
    CARD    = 8  //英雄卡
}

public enum ERuneType
{
    LIT = 1,//小雕文
    MID = 2,//中雕文
    BIG = 3 //大雕文
}

public enum ECopyType
{
    TYPE_LOAD  = -1,
    TYPE_INIT  =  0,
    TYPE_LOGIN =  1,
    TYPE_ROLE  =  2,
    TYPE_CITY  =  3,
    TYPE_PVE   =  4,
    TYPE_WORLD =  5,
    TYPE_AREA  =  6,
}

public enum EStarCondition
{
    TYPE_PASSCOPY    = 0,
    TYPE_MAIN_HEALTH = 1,
    TYPE_TIME_LIMIT  = 2,
}

public enum ECompare
{
    EQ = 0,//等于
    GT = 1,//大于
    LT = 2,//小于
    GE = 3,//大于等于
    LE = 4,//小于等于
    NO = 5,//不等于
}

public enum EAffect
{
    Self = 1,//影响自己
    Enem = 2,//影响敌方
    Ally = 3,//影响友方
    Each = 4,//影响所有
    Boss = 5,//影响Boss
    Host = 6,//影响主人
}

public enum EBattleCamp
{
    CT_NONE         = 0,
    CT_PVE_PLAYER   = 1,
    CT_PVE_ENEMY    = 2,
    CT_PVE_NEUTRAL  = 3,
    CT_PVP_PLAYER1  = 4,
    CT_PVP_PLAYER2  = 5,
    CT_PVP_PLAYER3  = 6,
    CT_PVP_PLAYER4  = 7,
    CT_PVP_PLAYER5  = 8,
}

public enum EDropType
{
    ONERAN = 1,//随机掉落一定数量的单个物品
    MULFIX = 2,//固定掉落多个物品
    MULRAN = 3,//随机掉落多个物品
}

public enum ERecvType
{
    TYPE_RECV_ALL,//领取所有的
    TYPE_RECV_ONE,//几选一
}

public enum ERewardState
{
    NOT_RECEIVE = 0,
    CAN_RECEIVE = 1,
    HAS_RECEIVE = 2,
}

public enum EDialoguePos
{
    LF = 0,
    RT = 1
}

public enum EDialogueContentShowType
{
    Normal = 0,
    Effect = 1
}

public enum EDialogueContentType
{
    TYPE_NONE   = 0,
    TYPE_PLAYER = 1,
    TYPE_ACTOR  = 2,
    TYPE_ITEM   = 3,
    TYPE_MAP    = 4
}

public enum ETaskType
{
    NONE,
    THREAD = 1,   //主线任务
    BRANCH = 2,   //支线任务
    DAILY  = 3,   //日常任务
}

public enum ETaskTargetType
{
    TYPE_NONE,
    TYPE_KILL_COPYBOSS  = 1,//击杀副本BOSS
    TYPE_MAIN_PASSCOPY  = 2,//通关副本（次数）
    TYPE_UPEQUIP        = 3,//升级装备
    TYPE_UPPET          = 4,//升级宠物
    TYPE_UPGEM          = 5,//升级星石
    TYPE_UPPARTNER      = 6,//升级伙伴
    TYPE_UPSKILL        = 7,//升级角色技能
    TYPE_TALK           = 8,//对话
    TYPE_ROB_TREASURE   = 9,//夺宝
    TYPE_AREAE          = 10,//竞技场战斗
    TYPE_PASS_ELITECOPY = 11,//通关精英副本
    TYPE_CHARGE_RELICE  = 12,//神器充能
    TYPE_EQUIPSTAR      = 13,//装备星级
    TYPE_XHJJC          = 14,//虚幻竞技场
    TYPE_KILLRACE       = 15,//杀死种族怪物
}


public enum ETaskCycleType
{
    TYPE_NONE,
    TYPE_DAILY,   //每日重置
    TYPE_WEEKLY,  //每周重置
    TYPE_SCENE,   //副本重置
}

public enum ETaskState
{
    QUEST_NONE,           //无类型
    QUEST_DOING,          //正在进行任务
    QUEST_CANSUBMIT,      //可提交
    QUEST_FAILED,         //任务失败
    QUEST_HASSUBMIT,      //已经提交
}

public enum Resp : byte
{
    TYPE_YES                  = 0,  //成功
    TYPE_NO                   = 1,  //不做
    TYPE_CANNOT_CONTROLSELF   = 2,  //角色被控制，无法操控

    TYPE_CANNOT_MOVE          = 3,  //角色无法移动 
    TYPE_CANNOT_MOVETODEST    = 4,  //无法到达此位置 
    TYPE_HAS_DEAD             = 5,  //角色已死亡

    TYPE_SKILL_LACKHP         = 11,  //缺少HP
    TYPE_SKILL_LACKMP         = 12,  //缺少MP
    TYPE_SKILL_LACKSP         = 13,  //缺少SP
    TYPE_SKILL_CASTING        = 14,  //正在释放技能
    TYPE_SKILL_LEADING        = 15,  //正在引导技能
    TYPE_SKILL_CD             = 16,  //技能还未冷却
    TYPE_SKILL_NOTFIND        = 17,  //找不到这个技能
    TYPE_SKILL_NOTDOATSCENE   = 18,  //当前场景无法使用技能
    TYPE_SKILL_LACKXP         = 19,  //缺乏经验
    TYPE_SKILL_SILENT         = 20,  //沉默

    TYPE_RIDE_ING             = 31,  //骑乘中
    TYPE_RIDE_NOTDOATSCENE    = 32,  //当前场景无法使用坐骑
    TYPE_RIDE_NOTDOATFSM      = 33,  //当前状态无法使用坐骑 
    TYPE_RIDE_NONE            = 34,  //当前你还没有坐骑  
}

public enum EMoveType : byte
{
    SeekPosition,
    SeekTransform,
    SeekActor,
    MoveForce,
}

public enum FSMState : int
{
    FSM_EMPTY,
    FSM_BORN,                //出生
    FSM_IDLE,                //待机

    FSM_RUN,                 //快跑
    FSM_WALK,                //漫步

    FSM_SKILL,               //攻击
    FSM_DEAD,                //死亡
    FSM_REBORN,              //重生

    FSM_WOUND,               //受击
    FSM_BEATBACK,            //击退
    FSM_BEATDOWN,            //击倒
    FSM_BEATFLY,             //击飞
    FSM_FLOATING,            //浮空

    FSM_FROST,               //冰冻
    FSM_STUN,                //昏迷
    FSM_FIXBODY,             //定身
    FSM_VARIATION,           //变形
    FSM_FEAR,                //恐惧
    FSM_SLEEP,               //睡眠
    FSM_PARALY,              //麻痹
    FSM_BLIND,               //致盲

    FSM_RIDEMOVE,            //骑乘跑
    FSM_RIDEIDLE,            //骑乘待机
    FSM_RIDEATTACK,          //骑乘攻击

    FSM_TALK,                //说话
    FSM_HOOK,                //钩子
    FSM_GRAB,                //抓取
    FSM_FLY,                 //飞行
    FSM_ROLL,                //翻滚
    FSM_JUMP,                //跳跃

    FSM_DANCE,               //跳舞
    FSM_MINE,                //采集状态
    FSM_INTERACTIVE,         //交互
}

public enum EActorUnit
{
    Ground,//地面
    Heaven //天上
}

public enum EActorNature
{
    CAN_MOVE,          //可移动
    CAN_KILL,          //可击杀
    CAN_MANUALATTACK,  //可主动攻击
    CAN_TURN,          //可转向
    CAN_STUN,          //可击晕
    CAN_BEATBACK,      //可击退
    CAN_BEATFLY,       //可击飞
    CAN_BEATDOWN,      //可击倒
    CAN_WOUND,         //可受击
    CAN_REDUCESPEED,   //可减速
    CAN_FIXBODY,       //可定身
    CAN_SLEEP,         //可睡眠
    CAN_VARISTION,     //可变形
    CAN_PARALY,        //可麻痹
    CAN_FEAR,          //可恐惧
}

public enum EActorType
{
    PLAYER  = 0,  //玩家
    NPC     = 1,  //NPC
    MONSTER = 2,  //怪物
    PET     = 3,  //宠物
    MOUNT   = 4,  //坐骑
    MACHINE = 5,  //机关
    PARTNER = 6,  //伙伴
}

public enum EActorSex
{
    B,//男
    G,//女
    X,//未知
}

//怪物类型
public enum EActorSort
{
    None   = 0,
    Normal = 1,   //正常
    Elite  = 2,   //精英
    Rare   = 3,   //稀有
    Boss   = 4,   //Boss
    World  = 5,   //世界Boss
    Chest  = 6,   //宝箱
    Tower  = 7,   //水晶塔
    Cage   = 8,   //囚笼
}

//怪物种族
public enum EActorRace
{
    TYPE_NONE    = 1,    //宝箱、囚笼等
    TYPE_HUMAN   = 2,    //人类
    TYPE_SPIRIT  = 3,    //精灵
    TYPE_ORC     = 4,    //兽人
    TYPE_GHOST   = 5,    //亡灵
    TYPE_DEVIL   = 6,    //恶魔
    TYPE_ELEM    = 7,    //元素
    TYPE_GIANT   = 8,    //巨人
    TYPE_MACHINE = 9,    //机械
    TYPE_BEAST   = 10,   //野兽
    TYPE_DRAGON  = 11,   //龙类
}


public enum EAttr : int
{
    HP         = 1,   //生命值
    AP         = 2,   //攻击力
    DF         = 3,   //防御力
    CRIT       = 4,   //爆击
    CRITDAMAGE = 5,   //爆伤
    MP         = 6,   //魔法值
    SUCK       = 7,   //吸血
    HIT        = 8,   //命中
    DODGE      = 9,   //闪避
    SP         = 10,  //灵力值

    INJURY     = 11,  //免伤
    HPRECOVER  = 12,  //回血
    MPRECOVER  = 13,  //回魔
    SPRECOVER  = 14,  //回灵力
    CRITDF     = 15,  //爆防
    SPEED      = 16,  //速度

    PAP        = 17,  //攻击百分比
    PHP        = 18,  //生命百分比
    PDF        = 19,  //防御百分比
    PMP        = 20,  //魔法百分比

    EAP        = 21,  //额外伤害
    EHP        = 22,  //攻击回血
    EMP        = 23,  //攻击回魔
    ESP        = 24,  //攻击回灵力

    IAP        = 25,  //冰攻击
    FAP        = 26,  //火攻击
    BAP        = 27,  //暗攻击
    LAP        = 28,  //电攻击

    IDF        = 29,  //冰攻击
    FDF        = 30,  //火攻击
    BDF        = 31,  //暗攻击
    LDF        = 32,  //电攻击

    MAXHP      = 33,  //最大生命值 
    MAXMP      = 34,  //最大魔法值
    MAXSP      = 35,  //最大灵力值
    ABSORB     = 36,  //伤害吸收
    REFLEX     = 37,  //伤害反弹

}

public enum ETargetRelationShip
{
    TYPE_NONE    =  0,
    TYPE_ALLY    =  1,
    TYPE_ENEMY   =  2,
    TYPE_NEUTRAL =  3,
}

public enum EPartnerSort
{
    LF           = 1,
    RT           = 2,
}

public enum EDeadReason
{
    Normal       = 0,   //正常死亡
    Kill         = 1,   //机制秒杀
    Plot         = 2    //剧情杀
}


public enum ESyncDataType
{
    TYPE_ALL,
    TYPE_EQUIP,
    TYPE_BASEATTR,
    TYPE_CURRATTR,
    TYPE_AOI,
    TYPE_LEVEL,
    TYPE_NAME,
    TYPE_TITLE,
}

public enum EAIExecuteType
{
    TYPE_HANDLE     = 0,//手动
    TYPE_AUTO       = 1,//自动
    TYPE_AUTOPAUSED = 2,//自动被暂停
}