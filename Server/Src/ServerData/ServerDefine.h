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

enum SCENE_LOGIC_TYPE
{
	SLT_BEGIN = 0,	
	SLT_CITY,
	SLT_NORMAL,				

	SLT_END
};


enum  OBJECT_STATE
{
	BS_Static = 1,  ///停
	BS_Walk = 2,    ///移动
	BS_DIE = 3,   ///死亡
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



#define MAX_MONEY_NUM 15
#define MAX_ACTION_NUM 4


#endif //__SERVER_STRUCT_H__