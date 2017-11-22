#ifndef __SKILL_STRUCT_H__
#define __SKILL_STRUCT_H__

#include "../ServerData/ServerDefine.h"

enum ECenterType
{
	TYPE_TARGET_POS		= 1,//以客户端选定的位置为中心
	TYPE_CASTER_POS		= 2,//以施法者为中心
	TYPE_CASTER_OFFSET	= 3,//以施法者为中心的偏移位置为中心
};

enum ERangeType
{
	TYPE_OBJECTS		= 1,//客户端指定的目标
	TYPE_CYLINDER		= 2,//扇形圆柱
	TYPE_CIRCLE			= 3,//圆形圆柱
	TYPE_BOX			= 4,//矩形区域
};


enum EBulletType
{
	TYPE_CHASE			= 0,//追踪型飞弹
	TYPE_FIXDIRECTION	= 1,//固定方向型飞弹
	TYPE_FIXTARGETPOS	= 2,//固定目标点飞弹
	TYPE_POINT			= 3,//固定点飞弹
	TYPE_LINK			= 4,//连接飞弹
	TYPE_ANNULAR		= 5,//环形飞弹
	TYPE_BACK			= 6,//回旋飞弹
	TYPE_EXTRACT		= 7,//抽取飞弹
	TYPE_BOUNDCE		= 8,//弹跳飞弹
};

enum EHitTargetType
{
	HIT_TARGET_RELATION_TYPE_NONE = 0,// 无
	HIT_TARGET_RELATION_TYPE_FRIEND = 1,// 需要目标是友方才能释放
	HIT_TARGET_RELATION_TYPE_ENEMY = 2,// 需要目标是敌方才能释放
	HIT_TARGET_RELATION_TYPE_APPOINTNPC = 3,// 需要目标是指定Npc才能释放
};

enum ESelectTargetPolicy
{
	TYPE_SELECT_DEFAULT = 0,//默认
	TYPE_SELECT_BY_MOREHEALTH = 1,//按血量比例最高
	TYPE_SELECT_BY_LESSHEALTH = 2,//按血量比例最低
	TYPE_SELECT_BY_MOREDISTANCE = 3,//按距离比例最高
	TYPE_SELECT_BY_LESSDISTANCE = 4,//按距离比例最低
};

struct StBullet
{
	UINT32 BulletID;	//子弹ID
	FLOAT  Angle;		//角度
	UINT32 BulletType;	//子弹类型
	FLOAT  AttackFix;	//固定伤害
	FLOAT  AttackMuti;	//加成伤害
};

struct StSkillEvent
{
	UINT64 TrigerTime;					//触发时间
	UINT32 ActionID;					//受击动作, 击退，击飞， 击倒
	UINT32 SelfBuffID;					//自己的BuffID
	UINT32 TargetBuffID;				//目标的BuffID
	FLOAT  AttackFix;					//固定伤害
	FLOAT  AttackMuti;					//加成伤害
	FLOAT  RangeParams[5];				//范围参数
	UINT32 RangeType;					//范围类型
	std::vector<StBullet> vtBullets;	//子弹列表
};

struct StSkillInfo
{
	UINT32 SkillID;						//技能ID
	UINT32 Level;						//技能类型
	UINT32 CD;							//技能CD
	UINT32 CostMp;						//消耗魔法值
	std::vector<StSkillEvent> vtEvents; //技能事件列表
};

struct StBuffInfo
{
	UINT32 BuffID;						//BuffID
	UINT32 BuffEffect;					//Buff目标需要加的特效
	INT32  PtyValue[PROPERTY_NUM];		//加属性值
	INT32  PtyPercent[PROPERTY_NUM];	//加属性百分比
	INT32  TotalTime;                   //总时长(ms)
	INT32  Interval;					//间隔时长
	UINT32 ChangeStaute;				//修改玩家的状态
	BOOL   OverLay;				        //是否可以叠加
	std::string   LuaAdd;				//是否有lua事件
	std::string   LuaTick;				//是否有lua事件
	std::string   LuaRemove;			//是否有lua事件
};

struct StBulletInfo
{
	UINT32 BulletID;					//子弹
	FLOAT  Speed;						//速度
	FLOAT  AceSpeed;					//加速度
	FLOAT  Angle;						//角度
	FLOAT  LifeTime;					//生存时间
};

#endif //__CONFIG_STRUCT_H__