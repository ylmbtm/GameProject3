#include "stdafx.h"
#include "SkillObject.h"
#include "SceneObject.h"
#include "Log.h"
#include "../ConfigData/ConfigStruct.h"
#include "../ConfigData/ConfigData.h"
#include "CommonFunc.h"
#include "../Message/Game_Define.pb.h"

CSkillObject::CSkillObject()
{
	m_pSceneObject = NULL;
	m_dwStartTick = 0;
	m_dwSkillID = 0;
}

CSkillObject::~CSkillObject()
{
	m_pSceneObject = NULL;
	m_dwStartTick = 0;
	m_dwSkillID = 0;
}


BOOL CSkillObject::OnUpdate( UINT64 dwTick )
{
	if(dwTick - m_dwStartTick)
	{

	}

	return TRUE;
}

BOOL CSkillObject::Start()
{
	std::vector<CSceneObject*> vtTargets;

	for(auto itor = vtTargets.begin(); itor != vtTargets.end(); itor++)
	{
		CSceneObject* pTarget = *itor;
		ERROR_CONTINUE_EX(pTarget != NULL);

		SkillFight(pTarget);


	}

	return TRUE;
}

BOOL CSkillObject::GetTargets()
{
	return TRUE;
}

BOOL CSkillObject::SetHostObject(CSceneObject* pObject)
{
	m_pSceneObject = pObject;

	return TRUE;
}

BOOL CSkillObject::SkillFight(CSceneObject* pTarget)
{
	ERROR_RETURN_FALSE(m_pSceneObject != NULL);
	ERROR_RETURN_FALSE(pTarget != NULL);

	StSkillInfo* pSkillInfo = CConfigData::GetInstancePtr()->GetSkillInfo(m_dwSkillID, 1);
	ERROR_RETURN_FALSE(pSkillInfo != NULL);

	UINT64 dwCurTime = CommonFunc::GetTickCount();

	UINT64 dwTime = m_pSceneObject->GetLastSkillTime(m_dwSkillID);
	if(dwCurTime - dwTime < pSkillInfo->CD * 1000)
	{
		CLog::GetInstancePtr()->LogError("玩家作弊!!!");

		return FALSE;
	}

	StBuffInfo* pBuffInfo = CConfigData::GetInstancePtr()->GetBuffInfo(pSkillInfo->SelfBuffID);
	ERROR_RETURN_FALSE(pBuffInfo != NULL);

	m_pSceneObject->AddBuff(pSkillInfo->SelfBuffID);

	pTarget->AddBuff(pSkillInfo->TargetBuffID);

	UINT32 dwRandValue = CommonFunc::GetRandNum(1);
	//先判断是否命中
	if (dwRandValue > (800 + m_pSceneObject->m_Propertys[8] - pTarget->m_Propertys[7]) && dwRandValue > 500)
	{
		//未命中
		return TRUE;
	}

	//判断是否爆击
	dwRandValue = CommonFunc::GetRandNum(1);
	BOOL bCriticalHit = FALSE;
	if (dwRandValue < (m_pSceneObject->m_Propertys[9] - m_pSceneObject->m_Propertys[10]) || dwRandValue < 10)
	{
		bCriticalHit = TRUE;
	}

	//最终伤害加成
	UINT32 dwFinalAdd = m_pSceneObject->m_Propertys[6] - pTarget->m_Propertys[5] + 1000;

	//伤害随机
	UINT32 dwFightRand = 900 + CommonFunc::GetRandNum(1) % 200;
	INT32 hurt = (pSkillInfo->Percent * (m_pSceneObject->m_Propertys[5] - pTarget->m_Propertys[6]) + pSkillInfo->Fix);
	UINT32 dwHurt = m_pSceneObject->m_Propertys[1] - pTarget->m_Propertys[1];
	if (dwHurt <= 0)
	{
		dwHurt = 1;
	}
	else
	{
		dwHurt = dwHurt * dwFightRand / 1000;
		dwHurt = dwHurt * dwFinalAdd / 1000;
		if (bCriticalHit)
		{
			dwHurt = dwHurt * 15 / 10;
		}
	}

	pTarget->m_Propertys[HP] -= dwHurt;
	if (pTarget->m_Propertys[HP] <= 0)
	{
		pTarget->m_Propertys[HP] = 0;
	}

	return TRUE;
}

