#ifndef __SERVER_DEFINE_H__
#define __SERVER_DEFINE_H__

//货币数量
#define MONEY_NUM 15

//行动力数量
#define ACTION_NUM 4

//角色属性个数
#define PROPERTY_NUM 21

//角色的最大等级
#define MAX_ROLE_LEVEL 150

//服务器帧率
#define FPS_TIME_TICK 20

//副本战斗结果
enum ECopyResult
{
	CR_NONE = 0,	//未结算
	CR_WIN,         //胜利
	CR_LOST,		//失败
	CR_DRAW			//平局
};

enum EMouduleType
{
	MT_ROLE,
	MT_COPY,
	MT_BAG,
	MT_EQUIP,
	MT_PET,
	MT_PARTNER,
	MT_TASK,
	MT_MOUNT,
	MT_ACTIVITY,
	MT_COUNTER,
	MT_END
};

enum EAddWay
{
	Add_ = 1,
	Cost_
};

enum ESealState
{
	SS_NONE = 0,
	SS_OK   = 1,
	SS_NO   = 2
};


enum EActivtyType
{
	ACT_NONE = 0,
	ACT_LOGIN = 1,
	ACT_SINCHRAGE = 2,
	ACT_SUMCHARGE = 3,
	ACT_OPEN_FUND = 4,
	ACT_DISCOUNT_SALE = 5,

};

#endif //__SERVER_STRUCT_H__