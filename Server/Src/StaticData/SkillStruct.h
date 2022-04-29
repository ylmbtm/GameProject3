#ifndef __SKILL_STRUCT_H__
#define __SKILL_STRUCT_H__

#include "../Message/Game_Define.pb.h"

enum ECenterType
{
    ECT_TARGET_POS      = 1,//以客户端选定的位置为中心
    ECT_TARTGT_OBJ      = 2,//以客户端选定的对象为中心
    ECT_CASTER_POS      = 3,//以施法者为中心
    ECT_CASTER_OFFSET   = 4,//以施法者为中心的偏移位置为中心
};

enum ESkillCastType
{
    ESCT_TYPE_INSTANT       = 0,    // 瞬发（无选择）
    ESCT_TYPE_TARGET        = 1,    // 对象选取辅助（目标）
    ESCT_TYPE_DIRECTION     = 2,    // 方向贴花辅助（朝向）
    ESCT_TYPE_POS           = 3,    // 地面贴花辅助（AOE区域）
    ESCT_TYPE_DIRECTIONARC  = 4,    // 扇形方向贴花辅助（朝向）
    ESCT_TYPE_EFFECT        = 5,    // 模型特效辅助（召唤）
    ESCT_TYPE_SIGHT         = 6,    // 准星UI变化辅助（射击）
    ESCT_TYPE_PARABOLA      = 7,    // 抛物线辅助（投掷）
    ESCT_TYPE_DRAGLINE      = 8,    // 拉线辅助（墙类召唤)
    ESCT_TYPE_GESTURE       = 9,    // 鼠标手势辅助（多段线条传入点）
};

enum ERangeType
{
    ERT_OBJECTS     = 1,//客户端指定的目标
    ERT_CYLINDER    = 2,//扇形圆柱
    ERT_CIRCLE      = 3,//圆形圆柱
    ERT_BOX         = 4,//矩形区域
};

enum EBulletType
{
    EBT_CHASE           = 0,//追踪型飞弹
    EBT_FIXDIRECTION    = 1,//固定方向型飞弹
    EBT_FIXTARGETPOS    = 2,//固定目标点飞弹
    EBT_POINT           = 3,//固定点飞弹
    EBT_LINK            = 4,//连接飞弹
    EBT_ANNULAR         = 5,//环形飞弹
    EBT_BACK            = 6,//回旋飞弹
    EBT_EXTRACT         = 7,//抽取飞弹
    EBT_BOUNDCE         = 8,//弹跳飞弹
    EBT_WAVE            = 9,//冲击波
};

enum ESkillStatus
{
    ESS_INIT        = 0,    //初始状态
    ESS_RUNNING     = 1,    //正在运行
    ESS_FINISHED    = 2,    //己完成
};

enum EAffectShip
{
    EAS_Self = 0, //影响自己
    EAS_Enem = 1, //影响敌方
    EAS_Ally = 2, //影响友方
    EAS_Each = 3, //影响所有
};

enum ESelectPolicy
{
    ESP_DEFAULT         = 0,//默认
    ESP_BY_MOREHEALTH   = 1,//按血量比例最高
    ESP_BY_LESSHEALTH   = 2,//按血量比例最低
    ESP_BY_MOREDISTANCE = 3,//按距离比例最高
    ESP_BY_LESSDISTANCE = 4,//按距离比例最低
};

enum ESkillType
{
    EST_NONE = 0,
    ESTPHYSICS = 1,
    ESTMAGIC = 2
};

// enum EObjectStatus
// {
//  EOS_NONE = 0;//无
//  EOS_DEAD = 1;//死亡
//  EOS_IDLE = 2;//静止
//  EOS_WALK = 3;//走
//  EOS_RUN = 4;//跑
//  EOS_FLY = 5;//飞
//  EOS_NOT_MOVE = 6;//不能移动
//  EOS_NOT_CAST = 7;//不能放技能
//  EOS_NOT_BEHURT = 8;//不能被攻击
//  EOS_NOT_BECONTROL = 9;//无法被控制
//  EOS_STEALTH = 10;//隐身
//  EOS_BLIND = 11;//瞎的
// };





struct StBuffInfo
{
    UINT32 BuffID;                      //BuffID
    UINT32 EffectID;                  //Buff目标需要加的特效
    INT32  PtyValue[PROPERTY_NUM];      //加属性值
    INT32  PtyPercent[PROPERTY_NUM];    //加属性百分比
    INT32  TotalTime;                   //总时长(ms)
    INT32  Interval;                    //间隔时长
    UINT32 ChangeStatus;                //修改玩家的状态
    BOOL   OverLay;                     //是否可以叠加
    std::string   LuaAdd;               //是否有lua事件
    std::string   LuaTick;              //是否有lua事件
    std::string   LuaRemove;            //是否有lua事件
};

struct StBulletInfo
{
    UINT32      BulletID    = 0;                //子弹ID
    EBulletType BulletType  = EBT_FIXDIRECTION; //子弹类型
    FLOAT       InitSpeed   = 0;                //初始速度
    FLOAT       AccSpeed    = 0;                //加速度
    ERangeType  RangeType   = ERT_CIRCLE;       //范围类型
    FLOAT       RangeParams[5] = { 0 };         //范围参数
    UINT32      LifeTime    = 0;                //生命期
    UINT32      HitActionID = 0;                 //受击动作, 击退，击飞， 击倒
    UINT32      HitEffect = 0;                   //受击特效
    FLOAT       HitDistance = 0;                 //受击移动移动距离
};

struct StBulletObject
{
    UINT32 BulletID;    //子弹ID
    FLOAT  fAngle;       //角度
};


struct StGoblinInfo
{
    UINT32 ActorID;     //角色ID
};


struct StSkillEvent
{
    UINT64 TrigerTime = 0;                  //触发时间
    UINT32 SelfBuffID = 0;                  //自己的BuffID
    UINT32 TargetBuffID = 0;                //目标的BuffID
    FLOAT  RangeParams[5] = {0};            //范围参数
    ERangeType RangeType = ERT_OBJECTS;     //范围类型
    UINT32 CenterType = 0;                  //中心点类型
    std::vector<StBulletObject> vtBullets;  //子弹列表
    std::vector<StGoblinInfo> vtGoblins;    //召唤的妖精列表
    UINT32 HitActionID = 0;                 //受击动作, 击退，击飞， 击倒
    UINT32 HitEffect = 0;                   //受击特效
    FLOAT  HitDistance = 0;                 //受击移动移动距离
};

struct StSkillInfo
{
    UINT32        SkillID;        //技能ID
    UINT32        Level;          //技能等级
    UINT32        SkillType;      //技能类型,1:物理伤害,2:法术伤害
    EHitShipType  HitShipType;    //作用目标关系,
    BOOL          HitMyself;      //目标是否包含自己
    UINT32        CD;               //技能CD
    UINT64        uDuration;      //技能持续总时间
    INT32         HurtFix;        //固定伤害
    INT32         HurtMuti;       //加成伤害
};


struct StSkillEventInfo
{
    UINT32      nSkillID;                      //技能ID
    UINT64      uDuration;                      //技能持续总时间
    UINT32      dwCastType;                     //目标选择方式
    std::vector<StSkillEvent> vtEvents;         //技能事件列表
};

struct StComboSkillInfo
{
    UINT32 SkillID;                     //主技能ID
    std::vector<UINT32> vtComboSkill;   //连击技能
};


#endif //__CONFIG_STRUCT_H__