#ifndef __GAME_STRUCT_H__
#define __GAME_STRUCT_H__

struct St_SkillData
{
	UINT32 dwSkillID;
	INT32  nLevel;
	INT32  nKeyPos;
	INT64  uLastTime;
};

typedef struct St_ObjectStatus
{
	union
	{
		UINT32  dwValues;
		struct
		{
			UINT32	nDir : 9;		// 朝向
			UINT32	bDead : 1;		// 是否死亡
		};
	};
} St_ObjectStatus;



#endif //__GAME_STRUCT_H__