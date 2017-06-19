#ifndef __GAME_STRUCT_H__
#define __GAME_STRUCT_H__

//角色状态
typedef struct St_ObjectStatus
{
	union
	{
		UINT32  dwValues;
		struct
		{
			UINT32	nDir:9;			// 朝向
			UINT32	bDead:1;		// 是否死亡
		};
	};
}St_ObjectStatus;

typedef struct St_CharFeature
{
	union
	{
		UINT32  dwValues;
		struct
		{
			UINT32	Career:3;		// 职业
			UINT32	Sex:1;			// 性别
			UINT32  SkinColor:3;	// 肤色
			UINT32  HeadColor:3;	// 发色
		};
	};
}St_CharFeature;


////////////////////////////////////////////


#endif //__GAME_STRUCT_H__
