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
        UINT32   dwValues;
        struct
        {
            UINT32  nDir    : 9;    // 朝向
            UINT32  bDead   : 1;    // 是否死亡
        };
    };
} St_ObjectStatus;

typedef struct St_ChangeStatus
{
    union
    {
        UINT32   dwValue;
        struct
        {
            UINT32  bBase   : 1;    // 基础数据,属性, 行动状态, 坐标, 朝向
            UINT32  bDead   : 1;    // 是否死亡
            UINT32  bEquip  : 1;    // 装备
            UINT32  bActor  : 1;    // 角色
            UINT32  bAction : 1;    // 行动状态
            UINT32  bBuff   : 1;    // BUFF
            UINT32  bLevel  : 1;    // 等级
            UINT32  bControl: 1;    // 控制权
            UINT32  bCamp   : 1;    // 阵营
            UINT32  bMount  : 1;    // 坐骑
        };
    };
} St_ChangeStatus;


#endif //__GAME_STRUCT_H__