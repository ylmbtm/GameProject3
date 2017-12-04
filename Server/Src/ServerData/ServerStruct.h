#ifndef __SERVER_STRUCT_H__
#define __SERVER_STRUCT_H__

struct BattleRecord
{
	UINT32			m_dwResult;		//0:未结算，1:胜利，2: 失败 3: 平局
	UINT32			m_dwKill;		//击杀数
	UINT32          m_dwDeath;		//被杀次数
	UINT32			m_dwHeal;		//治疗值
	UINT32          m_dwDamage;		//总伤害值
};


#endif //__SERVER_STRUCT_H__