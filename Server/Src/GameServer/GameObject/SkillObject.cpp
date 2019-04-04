#include "stdafx.h"
#include "SkillObject.h"
#include "SceneObject.h"
#include "../StaticData/StaticStruct.h"
#include "../StaticData/StaticData.h"
#include "../Message/Game_Define.pb.h"
#include "../StaticData/StaticStruct.h"
#include "../Scene.h"
#include "../Message/Msg_ID.pb.h"

CSkillObject::CSkillObject()
{
	m_pCastObject = NULL;
	ResetSkill();
}

CSkillObject::~CSkillObject()
{
	m_pCastObject	= NULL;
	ResetSkill();
}


BOOL CSkillObject::OnUpdate( UINT64 uTick )
{
	if (m_SkillStatus != ESS_RUNNING)
	{
		return TRUE;
	}

	if (m_dwSkillID == 0 || m_pSkillInfo == NULL || m_pSkillEventInfo == NULL)
	{
		return TRUE;
	}

	UINT64 uElaspedTick = uTick - m_dwStartTick;


	if (m_dwEventIndex < m_pSkillEventInfo->vtEvents.size())
	{
		if (uElaspedTick >= m_pSkillEventInfo->vtEvents[m_dwEventIndex].TrigerTime)
		{
			ProcessEvent(m_pSkillEventInfo->vtEvents[m_dwEventIndex]);

			m_dwEventIndex += 1;
		}
	}

	if (uElaspedTick > m_pSkillEventInfo->uDuration)
	{
		//响应技能结束
		OnSkillComplete();
		return TRUE;
	}

	return TRUE;
}

BOOL CSkillObject::OnSkillComplete()
{
	m_SkillStatus = ESS_FINISHED;
	//如果当前是
	if (m_bComboSkill)
	{
		m_bComboSkill = FALSE;
		UINT32 dwSkillID = m_pCastObject->GetNextComboSkill(m_dwSkillID);
		if (dwSkillID != 0)
		{
			SkillCastReq Req;
			Req.set_objectguid(m_pCastObject->GetObjectGUID());
			Req.set_skillid(dwSkillID);
			Req.set_hostx(m_pCastObject->GetPos().m_x);
			Req.set_hosty(m_pCastObject->GetPos().m_y);
			Req.set_hostz(m_pCastObject->GetPos().m_z);
			Req.set_hostft(m_pCastObject->GetFaceTo());
			m_pCastObject->GetScene()->BroadMessage(MSG_SKILL_CAST_NTF, Req);
			StartSkill(dwSkillID, m_pSkillInfo->Level);
		}

		return TRUE;
	}
	return TRUE;
}

BOOL CSkillObject::StartSkill(UINT32 dwSkillID, INT32 nLevel)
{
	m_dwSkillID = dwSkillID;

	m_dwStartTick = CommonFunc::GetTickCount();

	m_dwEventIndex = 0;

	m_pSkillInfo = CStaticData::GetInstancePtr()->GetSkillInfo(dwSkillID, nLevel);
	ERROR_RETURN_FALSE(m_pSkillInfo != NULL);

	m_pSkillEventInfo = CStaticData::GetInstancePtr()->GetSkillEventInfo(dwSkillID);
	ERROR_RETURN_FALSE(m_pSkillEventInfo != NULL);

	if (ESCT_TYPE_INSTANT == m_pSkillEventInfo->dwCastType)
	{
		m_vtTargets.clear();
	}

	m_SkillStatus = ESS_RUNNING;

	//计算攻击目标
	//1.直接带有目标， 2.需要自己计算目标
	//2.只给自己加buff的技能
	//3.只给目标加buff的技能
	//4.加血的技能
	//5.位移技能
	//6.波次技能(闪电链)
	//7.产生子弹的技能


	OnUpdate(m_dwStartTick);

	return TRUE;
}


BOOL CSkillObject::StopSkill()
{
	ResetSkill();
	return TRUE;
}

BOOL CSkillObject::ResetSkill()
{
	m_dwStartTick = 0;
	m_dwSkillID = 0;
	m_pSkillInfo = NULL;
	m_pSkillEventInfo = NULL;
	m_vtTargets.clear();
	m_dwEventIndex = 0;
	m_SkillStatus = ESS_INIT;
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
	if (dwRandValue > (8000 + m_pCastObject->m_Propertys[HIT_RATE] - pTarget->m_Propertys[DODGE]) && dwRandValue > 5000)
	{
		//未命中
		m_pCastObject->NotifyHitEffect(pTarget, FALSE, 0);
		return TRUE;
	}

	//判断是否爆击
	dwRandValue = CommonFunc::GetRandNum(1);
	BOOL bCriticalHit = FALSE;
	if (dwRandValue < (m_pCastObject->m_Propertys[CRIT_HIT] - m_pCastObject->m_Propertys[CRIT_DEF]) || dwRandValue < 100)
	{
		bCriticalHit = TRUE;
	}

	//最终伤害加成
	INT32 nFinalAdd = m_pCastObject->m_Propertys[MORE_HURT] - pTarget->m_Propertys[LESS_HURT];
	if (nFinalAdd < 0)
	{
		nFinalAdd = 0;
	}

	//伤害随机
	INT32 nFightRand = 9000 + CommonFunc::GetRandNum(1) % 2000;

	INT32 nDefendValue = (m_pSkillInfo->SkillType == ESTMAGIC) ? pTarget->m_Propertys[MAGIC_DEF] : pTarget->m_Propertys[PHYSIC_DEF];

	INT32 nHurt = m_pCastObject->m_Propertys[ATTACK] - nDefendValue;
	if (nHurt < 0)
	{
		nHurt = 1;
	}

	nHurt = nHurt * nFightRand / 10000;
	nHurt += nHurt * nFinalAdd / 10000;
	if (bCriticalHit)
	{
		nHurt = nHurt * 3 / 2;
	}

	pTarget->SubHp(nHurt);

	m_pCastObject->NotifyHitEffect(pTarget, bCriticalHit, -nHurt);

	return TRUE;
}

BOOL CSkillObject::CalcTargetObjects(StSkillEvent& SkillEvent)
{
	ERROR_RETURN_FALSE(m_pCastObject != NULL);

	switch (SkillEvent.RangeType)
	{
		case ERT_OBJECTS:
		{
			//什么都不需要做，直接使用客户端传过来的目标列表
		}
		break;
		case ERT_CIRCLE:
		{
			FLOAT radius	= SkillEvent.RangeParams[0];
			FLOAT hAngle	= SkillEvent.RangeParams[1];
			FLOAT height	= SkillEvent.RangeParams[2];
			FLOAT offsetX	= SkillEvent.RangeParams[3];
			FLOAT offsetZ	= SkillEvent.RangeParams[4];

			Vector3D hitPoint = m_pCastObject->m_Pos;
			hitPoint = hitPoint + Vector3D(offsetX, 0, offsetZ);

			CScene* pScene = m_pCastObject->GetScene();
			ERROR_RETURN_FALSE(pScene != NULL);

			pScene->SelectTargetsInCircle(m_vtTargets, hitPoint, radius, height);
		}
		break;
		case ERT_CYLINDER:
		{
			FLOAT radius	= SkillEvent.RangeParams[0];
			FLOAT hAngle	= SkillEvent.RangeParams[1];
			FLOAT height	= SkillEvent.RangeParams[2];
			FLOAT offsetX	= SkillEvent.RangeParams[3];
			FLOAT offsetZ	= SkillEvent.RangeParams[4];

			Vector3D hitPoint = m_pCastObject->m_Pos;
			hitPoint = hitPoint + Vector3D(offsetX, 0, offsetZ);

			FLOAT hitDir = m_pCastObject->m_ft;

			CScene* pScene = m_pCastObject->GetScene();
			ERROR_RETURN_FALSE(pScene != NULL);

			pScene->SelectTargetsInSector(m_vtTargets, hitPoint, hitDir, radius, hAngle);
		}
		break;
		case ERT_BOX:
		{
			FLOAT length	= SkillEvent.RangeParams[0];
			FLOAT width		= SkillEvent.RangeParams[1];
			FLOAT height	= SkillEvent.RangeParams[2];
			FLOAT offsetX	= SkillEvent.RangeParams[3];
			FLOAT offsetZ	= SkillEvent.RangeParams[4];

			Vector3D hitPoint = m_pCastObject->m_Pos;
			hitPoint = hitPoint + Vector3D(offsetX, 0, offsetZ);

			FLOAT hitDir = m_pCastObject->m_ft;

			CScene* pScene = m_pCastObject->GetScene();
			ERROR_RETURN_FALSE(pScene != NULL);

			pScene->SelectTargetsInSquare(m_vtTargets, hitPoint, hitDir, length, width);
		}
		case ERT_LINK:
		{

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

		if (pTempObject == m_pCastObject)
		{
			continue;
		}

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

		pScene->CreateBullet(&data, m_pCastObject->m_ft + data.Angle);
	}

	return TRUE;
}

ESkillStatus CSkillObject::GetSkillStatus()
{
	return m_SkillStatus;
}

BOOL CSkillObject::SetComboSkill(BOOL bCombo)
{
	m_bComboSkill = bCombo;

	return TRUE;
}

UINT32 CSkillObject::GetSkillID()
{
	return m_dwSkillID;
}

