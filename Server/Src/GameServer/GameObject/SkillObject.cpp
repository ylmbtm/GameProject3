#include "stdafx.h"
#include "SkillObject.h"
#include "SceneObject.h"
#include "../ConfigData/ConfigStruct.h"
#include "../ConfigData/ConfigData.h"
#include "../Message/Game_Define.pb.h"
#include "../ConfigData/ConfigStruct.h"
#include "../Scene.h"

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
	if (m_dwSkillID == 0 || m_pSkillInfo == NULL)
	{
		return TRUE;
	}

	if (m_dwEventIndex >= m_pSkillInfo->vtEvents.size())
	{
		return TRUE;
	}

	UINT64 uElaspedTick = uTick - m_dwStartTick;

	if(uElaspedTick > m_pSkillInfo->vtEvents[m_dwEventIndex].TrigerTime)
	{
		ProcessEvent(m_pSkillInfo->vtEvents[m_dwEventIndex]);

		m_dwEventIndex += 1;
	}

	if (uElaspedTick > m_pSkillInfo->uDuration)
	{
		//响应技能结束
		OnSkillComplete();
	}

	return TRUE;
}

BOOL CSkillObject::OnSkillComplete()
{
	return TRUE;
}

BOOL CSkillObject::StartSkill(UINT32 dwSkillID)
{
	m_pSkillInfo = CConfigData::GetInstancePtr()->GetSkillInfo(dwSkillID, 0);

	ERROR_RETURN_FALSE(m_pSkillInfo != NULL);

	m_dwSkillID = dwSkillID;

	m_dwStartTick = CommonFunc::GetTickCount();

	m_dwEventIndex = 0;

	//计算攻击目标
	//1.直接带有目标， 2.需要自己计算目标
	//2.只给自己加buff的技能
	//3.只给目标加buff的技能
	//4.加血的技能
	//5.位移技能
	//6.波次技能(闪电链)
	//7.产生子弹的技能

	//先依据阵营和规则取出伤害的目标
	//std::vector<CSceneObject*>& vtTargets = m_pCastObject->GetAffectTargets();

	//for (int i = 0; i < vtTargets.size(); i++)
	//{
	//	SkillFight(vtTargets.at(i));
	//}

	return TRUE;
}


BOOL CSkillObject::StopSkill()
{
	m_dwSkillID = 0;
	m_pSkillInfo = 0;
	m_dwEventIndex = 0;
	m_vtTargets.clear();
	return TRUE;
}

BOOL CSkillObject::SetCastObject(CSceneObject* pObject)
{
	m_pCastObject = pObject;

	return TRUE;
}

BOOL CSkillObject::AddTargetObject(CSceneObject* pObject)
{
	m_vtTargets.push_back(pObject);

	return TRUE;
}

BOOL CSkillObject::SkillFight(StSkillEvent& SkillEvent, CSceneObject* pTarget)
{
	ERROR_RETURN_FALSE(m_pCastObject != NULL);
	ERROR_RETURN_FALSE(m_pSkillInfo != NULL);
	ERROR_RETURN_FALSE(pTarget != NULL);

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
	INT32 nHurt = SkillEvent.AttackMuti * m_pCastObject->m_Propertys[5] + SkillEvent.AttackFix;
	nHurt = nHurt - pTarget->m_Propertys[1];
	if (nHurt <= 0)
	{
		nHurt = 1;
	}
	else
	{
		nHurt = nHurt * dwFightRand / 1000;
		nHurt = nHurt * dwFinalAdd / 1000;
		if (bCriticalHit)
		{
			nHurt = nHurt * 15 / 10;
		}
	}

	pTarget->m_Propertys[HP] -= nHurt;
	if (pTarget->m_Propertys[HP] <= 0)
	{
		pTarget->m_Propertys[HP] = 0;
	}
	return TRUE;
}

BOOL CSkillObject::CalcTargetObjects(StSkillEvent& SkillEvent)
{
	ERROR_RETURN_FALSE(m_pCastObject != NULL);

	switch (SkillEvent.RangeType)
	{
		case TYPE_OBJECTS:
		{
			//什么都不需要做，直接使用客户端传过来的目标列表
		}
		break;
		case TYPE_CIRCLE:
		{
			FLOAT radius	= SkillEvent.RangeParams[0];
			FLOAT hAngle	= SkillEvent.RangeParams[1];
			FLOAT height	= SkillEvent.RangeParams[2];
			FLOAT offsetX	= SkillEvent.RangeParams[3];
			FLOAT offsetZ	= SkillEvent.RangeParams[4];



		}
		break;
		case TYPE_CYLINDER:
		{
			FLOAT radius	= SkillEvent.RangeParams[0];
			FLOAT hAngle	= SkillEvent.RangeParams[1];
			FLOAT height	= SkillEvent.RangeParams[2];
			FLOAT offsetX	= SkillEvent.RangeParams[3];
			FLOAT offsetZ	= SkillEvent.RangeParams[4];
		}
		break;
		case TYPE_BOX:
		{
			FLOAT length	= SkillEvent.RangeParams[0];
			FLOAT width		= SkillEvent.RangeParams[1];
			FLOAT height	= SkillEvent.RangeParams[2];
			FLOAT offsetX	= SkillEvent.RangeParams[3];
			FLOAT offsetZ	= SkillEvent.RangeParams[4];
		}
		break;
	}

	return TRUE;
}

BOOL CSkillObject::ProcessEvent(StSkillEvent& SkillEvent)
{
	CalcTargetObjects(SkillEvent);

	if (SkillEvent.SelfBuffID != 0)
	{
		m_pCastObject->AddBuff(SkillEvent.SelfBuffID);
	}

	for (auto itor = m_vtTargets.begin(); itor != m_vtTargets.end(); itor++)
	{
		CSceneObject* pTempObject = *itor;

		if (SkillEvent.TargetBuffID != 0)
		{
			pTempObject->AddBuff(SkillEvent.TargetBuffID);
		}

		SkillFight(SkillEvent, pTempObject);
	}

	for (INT32 nIndex = 0; nIndex < SkillEvent.vtBullets.size(); nIndex++)
	{
		StBulletInfo& data = SkillEvent.vtBullets.at(nIndex);

		CScene* pScene = m_pCastObject->GetScene();

		ERROR_RETURN_FALSE(pScene != NULL);

		pScene->CreateBullet(data.BulletID, m_pCastObject->m_ft + data.Angle, data.BulletType, data.AttackFix, data.AttackMuti);
	}

	return TRUE;
}

