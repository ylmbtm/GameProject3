#include "stdafx.h"
#include "CommonMath.h"
#include "BulletObject.h"
#include "SceneObject.h"
#include "../Scene.h"

CBulletObject::CBulletObject(UINT64 uGuid, StBulletInfo* pBulletInfo, CSkillObject* pSkillObject, Vector3D startPos)
{
	Reset();
	m_pBulletInfo   = pBulletInfo;
	m_pSkillObject  = pSkillObject;
	m_uStartTick    = CommonFunc::GetTickCount();
	m_uGuid         = uGuid;
	m_Pos           = startPos;
	m_InitPos       = startPos;
	m_pCastObject   = m_pSkillObject->GetCastObject();
}

CBulletObject::~CBulletObject()
{
	Reset();
}

BOOL CBulletObject::Reset()
{
	m_pCastObject = NULL;
	m_pTargetObject = NULL;
	m_bFinished = FALSE;
	m_vx = 0.0f;
	m_vz = 0.0f;
	m_accx = 0.0f;
	m_accz = 0.0f;
	m_fAngle = -1.0f;
	m_uGuid = 0;
	m_pBulletInfo = NULL;
	m_pSkillObject = NULL;
	m_uLastTick = 0;
	return TRUE;
}

BOOL CBulletObject::OnUpdate(UINT64 uTick)
{
	UpdateBulletPos(uTick);

	if (CheckTargetObjects())
	{
		for (int i = 0; i < m_vtTargets.size(); i++)
		{
			m_pSkillObject->AttackTarget(m_vtTargets[i], m_pBulletInfo->HitActionID, m_pBulletInfo->HitEffect, m_pBulletInfo->HitDistance );
		}

		FinishBullet();
	}

	CheckLifeOver(uTick);

	m_uLastTick = uTick;

	return TRUE;
}

BOOL CBulletObject::SaveNewData(BulletNewNtf& Ntf)
{
	BulletItem* pItem = Ntf.add_itemlist();

	pItem->set_objectguid(m_uGuid);
	pItem->set_bulletid(m_pBulletInfo->BulletID);
	pItem->set_x(m_Pos.m_x);
	pItem->set_y(m_Pos.m_y);
	pItem->set_z(m_Pos.m_z);
	pItem->set_angle(m_fAngle);
	pItem->set_speed(m_pBulletInfo->InitSpeed);
	pItem->set_accspeed(m_pBulletInfo->AccSpeed);
	pItem->set_lefttime(m_pBulletInfo->LifeTime / 1000.0f);
	pItem->set_casterguid(m_pCastObject->GetObjectGUID());
	if (m_pTargetObject != NULL)
	{
		pItem->set_targetguid(m_pTargetObject->GetObjectGUID());
	}

	return TRUE;
}

BOOL CBulletObject::SetCastObject(CSceneObject* pObject)
{
	m_pCastObject = pObject;

	return TRUE;
}

BOOL CBulletObject::SetTargetObject(CSceneObject* pObject)
{
	m_pTargetObject = pObject;

	return TRUE;
}

CSceneObject* CBulletObject::GetCastObject()
{
	return m_pCastObject;
}

BOOL CBulletObject::CheckTargetObjects()
{
	if (m_pBulletInfo == NULL)
	{
		return FALSE;
	}

	if (m_pBulletInfo->BulletType == EBulletType::EBT_FIXDIRECTION)
	{
		FLOAT radius = m_pBulletInfo->RangeParams[0];
		Vector3D hitPoint = m_Pos;

		CScene* pScene = m_pCastObject->GetScene();

		ERROR_RETURN_FALSE(pScene != NULL);

		UINT64 uExcludeID = 0;
		if (!m_pSkillObject->m_pSkillInfo->HitMyself)
		{
			uExcludeID = m_pCastObject->GetObjectGUID();
		}

		pScene->SelectTargets(m_vtTargets, uExcludeID, m_pCastObject->GetCamp(), m_pSkillObject->m_pSkillInfo->HitShipType,
		                      m_Pos, m_fAngle, m_pBulletInfo->RangeType, m_pBulletInfo->RangeParams);
	}
	else if(m_pBulletInfo->BulletType == EBulletType::EBT_CHASE)
	{
		if (m_pTargetObject == NULL)
		{
			return TRUE;
		}

		Vector3D hitPoint = m_Pos;
		FLOAT radius = m_pBulletInfo->RangeParams[0];

		if (m_pTargetObject->IsInCircle(m_Pos, radius, 1))
		{
			m_vtTargets.push_back(m_pTargetObject);
		}
	}

	if (m_vtTargets.size() > 0)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CBulletObject::CheckLifeOver(UINT64 uTick)
{
	//检查是否该消失了
	if ((uTick - m_uStartTick) > m_pBulletInfo->LifeTime)
	{
		m_bFinished = TRUE;
	}

	return TRUE;
}

BOOL CBulletObject::FinishBullet()
{
	m_bFinished = TRUE;

	return TRUE;
}

BOOL CBulletObject::IsFinished()
{
	return m_bFinished;
}

BOOL CBulletObject::SetAngle(FLOAT fAngle)
{
	m_fAngle = fAngle;

	m_vx = sin(m_fAngle * DEG_TO_RAD) * m_pBulletInfo->InitSpeed;
	m_vz = cos(m_fAngle * DEG_TO_RAD) * m_pBulletInfo->InitSpeed;

	m_accx = sin(m_fAngle * DEG_TO_RAD) * m_pBulletInfo->AccSpeed;
	m_accz = sin(m_fAngle * DEG_TO_RAD) * m_pBulletInfo->AccSpeed;

	return TRUE;
}

BOOL CBulletObject::SetTargetPos(Vector3D& Pos)
{
	m_TargetPos = Pos;

	return TRUE;
}

BOOL CBulletObject::UpdateBulletPos(UINT64 uTick)
{
	FLOAT fAccX = 0.0f;
	FLOAT fAccZ = 0.0f;

	if (m_pBulletInfo->AccSpeed > 0.01)
	{
		fAccX = m_accx * (uTick - m_uStartTick) * (uTick - m_uStartTick) / 2000000;
		fAccZ = m_accz * (uTick - m_uStartTick) * (uTick - m_uStartTick) / 2000000;
	}

	switch (m_pBulletInfo->BulletType)
	{
		case EBT_CHASE:         //追踪型飞弹:
		{
			m_fAngle = 0.0;

			SetAngle(m_fAngle);

			m_Pos.m_x = m_Pos.m_x + m_vx * (uTick - m_uLastTick) / 1000 + fAccX;

			m_Pos.m_z = m_Pos.m_z + m_vz * (uTick - m_uLastTick) / 1000 + fAccZ;
		}
		break;

		case EBT_FIXTARGETPOS:  //固定目标点飞弹
		case EBT_FIXDIRECTION:  //固定方向型飞弹:
		{
			if (m_fAngle < 0)
			{
				m_fAngle = 0;

				SetAngle(m_fAngle);
			}

			m_Pos.m_x = m_InitPos.m_x + m_vx * (uTick - m_uStartTick) / 1000 + fAccX;

			m_Pos.m_z = m_InitPos.m_z + m_vz * (uTick - m_uStartTick) / 1000 + fAccZ;
		}
		break;
		case EBT_POINT:         //固定点飞弹
		{
		}
		break;
		case EBT_LINK:          //连接飞弹
		{
		}
		break;
		case EBT_ANNULAR:       //环形飞弹
		{
		}
		break;
		case EBT_BACK:          //回旋飞弹
		{
		}
		break;
		case EBT_EXTRACT:       //抽取飞弹
		{
		}
		break;
		case EBT_BOUNDCE:       //弹跳飞弹
		{
		}
		break;
		case EBT_WAVE:          //冲击波
		{
		}
		break;
		default:
		{

		}
	}



	return TRUE;
}

