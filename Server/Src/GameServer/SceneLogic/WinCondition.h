#ifndef __BATTLE_RESULT_H__
#define __BATTLE_RESULT_H__
#include "../../ServerData/ServerStruct.h"
#include "CommonMath.h"

class CWinCondition
{
public:
	CWinCondition(EWinCondition type = EWC_NONE);

	~CWinCondition();

	EWinCondition GetConditionType();

	VOID SetConditionType(EWinCondition type);

	BOOL SetDestination(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom);

	BOOL SetNpcID(UINT32 dwNpcID);

	BOOL SetKillMonster(UINT32 dwMonsterID, UINT32 dwKillNum);
private:
	EWinCondition   m_ConditionType = EWC_NONE;//判定类型
	Rect2D          m_DestRect;
	UINT32          m_dwNpcID = 0;
	UINT32          m_dwMonsterID = 0;
	UINT32          m_dwKillNum = 0;
};


#endif //__BATTLE_RESULT_H__