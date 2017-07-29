#ifndef __SERVER_DEFINE_H__
#define __SERVER_DEFINE_H__

//副本战斗结果
enum COPY_RESULT
{
	CR_NONE = 0,	//未结算
	CR_WIN,         //胜利
	CR_LOST,		//失败
	CR_DRAW			//平局
};

enum MouduleType
{
	MT_ROLE,
	MT_COPY,
	MT_BAG,
	MT_EQUIP,
	MT_PET,
	MT_END
};

//货币数量
#define MAX_MONEY_NUM 15

//行动力数量
#define MAX_ACTION_NUM 4

//服务器帧率
#define FPS_TIME_TICK 20

enum AddWay
{
	Add_ = 1,
	Cost_
};


#endif //__SERVER_STRUCT_H__