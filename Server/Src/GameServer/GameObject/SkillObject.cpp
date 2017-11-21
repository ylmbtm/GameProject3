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
	m_pCastObject = NULL;
	m_dwStartTick = 0;
	m_dwSkillID = 0;
	m_pSkillInfo = NULL;
}

CSkillObject::~CSkillObject()
{
	m_pCastObject = NULL;
	m_dwStartTick = 0;
	m_dwSkillID = 0;
	m_pSkillInfo = NULL;
}


BOOL CSkillObject::OnUpdate( UINT64 uTick )
{
	if(uTick - m_dwStartTick)
	{

	}

	return TRUE;
}

BOOL CSkillObject::StartSkill(UINT32 dwSkillID)
{
	m_dwSkillID = dwSkillID;

	m_pSkillInfo = CConfigData::GetInstancePtr()->GetSkillInfo(dwSkillID, 0);

	m_dwStartTick = CommonFunc::GetTickCount();

	//计算攻击目标
	//1.直接带有目标， 2.需要自己计算目标
	//2.只给自己加buff的技能
	//3.只给目标加buff的技能
	//4.加血的技能
	//5.位移技能
	//6.波次技能(闪电链)
	//7.产生子弹的技能

	//先依据阵营和规则取出伤害的目标
	std::vector<CSceneObject*>& vtTargets = m_pCastObject->GetAffectTargets();

	for (int i = 0; i < vtTargets.size(); i++)
	{
		SkillFight(vtTargets.at(i));
	}

	return TRUE;
}


BOOL CSkillObject::SetCastObject(CSceneObject* pObject)
{
	m_pCastObject = pObject;

	return TRUE;
}

BOOL CSkillObject::SkillFight(CSceneObject* pTarget)
{
	ERROR_RETURN_FALSE(m_pCastObject != NULL);
	ERROR_RETURN_FALSE(m_pSkillInfo != NULL);
	ERROR_RETURN_FALSE(pTarget != NULL);

	//给自己添加buff
	m_pCastObject->AddBuff(m_pSkillInfo->SelfBuffID);

	//给敌人添加buff
	pTarget->AddBuff(m_pSkillInfo->TargetBuffID);

	UINT32 dwRandValue = CommonFunc::GetRandNum(1);
	//先判断是否命中
	if (dwRandValue > (800 + m_pCastObject->m_Propertys[8] - pTarget->m_Propertys[7]) && dwRandValue > 500)
	{
		//未命中
		return TRUE;
	}

	//判断是否爆击
	dwRandValue = CommonFunc::GetRandNum(1);
	BOOL bCriticalHit = FALSE;
	if (dwRandValue < (m_pCastObject->m_Propertys[9] - m_pCastObject->m_Propertys[10]) || dwRandValue < 10)
	{
		bCriticalHit = TRUE;
	}

	//最终伤害加成
	UINT32 dwFinalAdd = m_pCastObject->m_Propertys[6] - pTarget->m_Propertys[5] + 1000;

	//伤害随机
	UINT32 dwFightRand = 900 + CommonFunc::GetRandNum(1) % 200;
	INT32 hurt = (m_pSkillInfo->Percent * (m_pCastObject->m_Propertys[5] - pTarget->m_Propertys[6]) + m_pSkillInfo->Fix);
	UINT32 dwHurt = m_pCastObject->m_Propertys[1] - pTarget->m_Propertys[1];
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

