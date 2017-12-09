#ifndef __SKILL_STRUCT_H__
#define __SKILL_STRUCT_H__

#include "../ServerData/ServerDefine.h"

enum ECenterType
{
	TYPE_TARGET_POS		= 1,//以客户端选定的位置为中心
	TYPE_CASTER_POS		= 2,//以施法者为中心
	TYPE_CASTER_OFFSET	= 3,//以施法者为中心的偏移位置为中心
	TYPE_NO_CENTER		= 4,//无特定中心(中心在玩家)
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


enum ESelectPolicy
{
	ESP_DEFAULT			= 0,//默认
	ESP_BY_MOREHEALTH	= 1,//按血量比例最高
	ESP_BY_LESSHEALTH	= 2,//按血量比例最低
	ESP_BY_MOREDISTANCE = 3,//按距离比例最高
	ESP_BY_LESSDISTANCE = 4,//按距离比例最低
};

// enum EObjectStatus
// {
// 	EOS_DEAD			= 1,//死亡
// 	EOS_NOT_MOVE		= 2,//不能移动
// 	EOS_NOT_CAST		= 4,//不能放技能
// 	EOS_NOT_BEHURT		= 8,//不能被攻击
// 	EOS_NOT_BECONTROL	= 16,//无法被控制
// 	EOS_STEALTH			= 32,//隐身
// 	EOS_BLIND			= 64,//瞎的
// };

struct StBullet
{
	UINT32 BulletID;	//子弹ID
	FLOAT  Angle;		//角度
	FLOAT  Speed;       //速度
	FLOAT  AccSpd;		//加速度
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
	UINT32 CenterType;					//中心点类型
	std::vector<StBullet> vtBullets;	//子弹列表
};

struct StSkillInfo
{
	UINT32		SkillID;		//技能ID
	UINT32		Level;			//技能类型
	UINT32		CD;				//技能CD
	UINT32		CostMp;			//消耗魔法值
	UINT64		uDuration;		//技能持续总时间
	FLOAT		AttackFix;		//固定伤害
	FLOAT		AttackMuti;		//加成伤害
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
	UINT32 ChangeStatus;				//修改玩家的状态
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

/*

BOOL LoadSkillData()
{
	std::string xmlName = "";
	rapidxml::xml_document<char>* pXmlDoc = new rapidxml::xml_document<char>();
	ERROR_RETURN_FALSE(pXmlDoc != NULL);

	for ()
	{

	}

	rapidxml::xml_node<char>* pXmlRoot = pXmlDoc->first_node("Root");
	ERROR_RETURN_FALSE(pXmlRoot != NULL);


	for (auto pSkillNode = pXmlRoot->first_node("Skill"); pSkillNode != NULL; pSkillNode = pSkillNode->next_sibling("Skill"))
	{

	}
}


BOOL ParseDocument(rapidxml::xml_document<char>* pXmlDoc)
{


	return TRUE;
}

BOOL ParseSkill(rapidxml::xml_node<char>* pXmlNode)
{


	return TRUE;
}


BOOL ParseActScope(rapidxml::xml_node<char>* pXmlNode)
{
	for (auto pResultNode = pXmlNode->first_node(); pResultNode != NULL; pResultNode = pResultNode->next_sibling())
	{
		if (pResultNode->name() == "ActDamage")
		{

		}
		else if (pResultNode->name() == "ActDamage")
		{

		}
		else if (pResultNode->name() == "ActDamage")
		{

		}
		else if (pResultNode->name() == "ActDamage")
		{

		}
		else if (pResultNode->name() == "ActDamage")
		{

		}
		else if (pResultNode->name() == "ActDamage")
		{

		}
		else if (pResultNode->name() == "ActDamage")
		{

		}
	}

	return TRUE;
}
*/
#endif //__CONFIG_STRUCT_H__